#ifndef __okeya__valuemapwriter__
#define __okeya__valuemapwriter__

#include "json/writer.h"
#include "cocos2d.h"

using namespace cocos2d;
namespace rapidjson{

class ValueMapWriter
{
public:
    typedef typename UTF8<>::Ch Ch;
    typedef typename cocos2d::Value Value;
    
    ValueMapWriter(Value& v) : _keyStack{{}}, _containerStack({&v}), _nestLevel(0) {};
    ~ValueMapWriter(){
        _nestLevel = 0;
        _containerStack.pop();
    };
    
	bool Null(){
        insertValue(nullptr);
        return true;
    }
    
	bool Bool(bool b){
        insertValue(b);
        return true;
    }
	bool Int(int i){
        insertValue(i);
        return true;
    }
	bool Uint(unsigned u){
        return true;
    }
	bool Int64(int64_t i64){
        return true;
    }
	bool Uint64(uint64_t u64){
        return true;
    }
	bool Double(double d){
        return true;
    }
    
	bool String(const Ch* str, SizeType length, bool copy = false){
        if(_containerStack.top()->getType() == Value::Type::VECTOR){
            insertValue(str);
            return true;
        }

        if(_keyStack.size() < _nestLevel){
            _keyStack.push(std::string(str));
        }else{
            insertValue(str);
        }
		return true;
	}
	bool StartObject(){
        if(_nestLevel < 1){
            _nestLevel ++;
            return true;
        }

        ValueMap pVmap;
        Value* pV = new Value(std::move(pVmap));
        _containerStack.push(pV);
        _nestLevel ++;
		return true;
	}
	bool EndObject(SizeType memberCount = 0){
        if(_containerStack.size() > 1){
            auto pContainer = _containerStack.top();
            _containerStack.pop();
            insertContainer(pContainer);
            _nestLevel --;
        }
		return true;
	}
    
	bool StartArray() {
        ValueVector pVvec;
        Value* pV = new Value(std::move(pVvec));
        _containerStack.push(pV);
		return true;
	}
    
	bool EndArray(SizeType elementCount = 0) {
        if(_containerStack.size() > 1){
            auto pContainer = _containerStack.top();
            _containerStack.pop();
            insertContainer(pContainer);
        }
		return true;
	}
    
    template<typename T>
    void insertValue(T v){
        auto pContainer = _containerStack.top();
        switch(pContainer->getType()){
            case Value::Type::MAP : {
                auto k = _keyStack.top();
                pContainer->asValueMap()[k] = v;
                _keyStack.pop();
                break;
            }
            case Value::Type::VECTOR : {
                auto pVec = &pContainer->asValueVector();
                pVec->push_back(Value(v));
                break;
            }
            case Value::Type::NONE    :
            case Value::Type::BYTE    :
            case Value::Type::INTEGER :
            case Value::Type::FLOAT   :
            case Value::Type::DOUBLE  :
            case Value::Type::BOOLEAN :
            case Value::Type::STRING  :
            case Value::Type::INT_KEY_MAP: {
                assert(false);
                // pContainer is always MAP or VECTOR.
                break;
            }
        }
    }
    
    template<typename T>
    void insertContainer(T c){
        auto k = _keyStack.top();
        auto pContainer = _containerStack.top();
        switch(pContainer->getType()){
            case Value::Type::MAP : {
                auto& vMap = pContainer->asValueMap();
                vMap[k] = (*c);
                _keyStack.pop();
                break;
            }
            case Value::Type::VECTOR : {
                auto& vVec = pContainer->asValueVector();
                vVec.push_back(*c);
                break;
            }
            case Value::Type::NONE    :
            case Value::Type::BYTE    :
            case Value::Type::INTEGER :
            case Value::Type::FLOAT   :
            case Value::Type::DOUBLE  :
            case Value::Type::BOOLEAN :
            case Value::Type::STRING  :
            case Value::Type::INT_KEY_MAP: {
                assert(false);
                // pContainer is always MAP or VECTOR.
                break;
            }
        }
    }
    bool Key(const Ch* str, SizeType length, bool copy = false) { return String(str, length, copy); }

    std::stack<const std::string> _keyStack;
    std::stack<Value*> _containerStack;
    int _nestLevel;
};

}

#endif /* defined(__okeya__valuemapwriter__) */
