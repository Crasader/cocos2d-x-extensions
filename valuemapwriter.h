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
    
    ValueMapWriter(Value* v) : _keyStack{{}}, _containerStack({v}), _nestLevel(0) {};
    ~ValueMapWriter(){};
    
	ValueMapWriter& Null(){
        insertValue(nullptr);
        return *this;
    }
    
	ValueMapWriter& Bool(bool b){
        insertValue(b);
        return *this;
    }
	ValueMapWriter& Int(int i){
        insertValue(i);
        return *this;
    }
	ValueMapWriter& Uint(unsigned u){
        return *this;
    }
	ValueMapWriter& Int64(int64_t i64){
        return *this;
    }
	ValueMapWriter& Uint64(uint64_t u64){
        return *this;
    }
	ValueMapWriter& Double(double d){
        return *this;
    }
    
	ValueMapWriter& String(const Ch* str, SizeType length, bool copy = false){
        if(_containerStack.top()->getType() == Value::Type::VECTOR){
            insertValue(str);
            return *this;
        }

        if(_keyStack.size() < _nestLevel){
            _keyStack.push(std::string(str));
        }else{
            insertValue(str);
        }
		return *this;
	}
	ValueMapWriter& StartObject(){
        if(_nestLevel < 1){
            _nestLevel ++;
            return *this;
        }

        ValueMap pVmap;
        Value* pV = new Value(std::move(pVmap));
        _containerStack.push(pV);
        _nestLevel ++;
		return *this;
	}
	ValueMapWriter& EndObject(SizeType memberCount = 0){
        if(_containerStack.size() > 1){
            auto pContainer = _containerStack.top();
            _containerStack.pop();
            insertContainer(pContainer);
            _nestLevel --;
        }
		return *this;
	}
    
	ValueMapWriter& StartArray() {
        ValueVector pVvec;
        Value* pV = new Value(std::move(pVvec));
        _containerStack.push(pV);
		return *this;
	}
    
	ValueMapWriter& EndArray(SizeType elementCount = 0) {
        if(_containerStack.size() > 1){
            auto pContainer = _containerStack.top();
            _containerStack.pop();
            insertContainer(pContainer);
        }
		return *this;
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

    std::stack<const std::string> _keyStack;
    std::stack<Value*> _containerStack;
    int _nestLevel;
};

}

#endif /* defined(__okeya__valuemapwriter__) */
