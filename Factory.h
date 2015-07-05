#ifndef __okeya__Factory__
#define __okeya__Factory__

#include <map>
#include <string>


template<class Base>
class CreatorBase
{
public:
    virtual ~CreatorBase() {}
    virtual Base* create() const = 0;
    virtual Base* createWithInit() const = 0;
};

template<class Product, class Base>
class Creator : public CreatorBase<Base>
{
public:
    virtual Base * create() const
    {
        auto* product = new Product;
        product->autorelease();
        return product;
    }
    
    virtual Base * createWithInit() const
    {
        auto* product = new Product;
        product->autorelease();
        product->init();
        return product;
    }
};

template<class Base>
class Factory
{
public:
    virtual ~Factory();
    Base* create(std::string type);
    Base* createWithInit(std::string type);
    bool sign(std::string type, CreatorBase<Base> * pCreator);
private:
    typedef std::map<std::string, CreatorBase<Base> *> CreatorMap;
    CreatorMap _creatorMap;
};

template<class Base>
Factory<Base>::~Factory()
{
    for(auto& creator: _creatorMap){
        delete std::get<1>(creator);
    }
}

template<class Base>
bool Factory<Base>::sign(std::string type, CreatorBase<Base> * pCreator)
{
    typename CreatorMap::iterator it = _creatorMap.find(type);
    if (it != _creatorMap.end()) {
        delete pCreator;
        return false;
    }
    _creatorMap[type] = pCreator;
    return true;
}


template<class Base>
Base * Factory<Base>::create(std::string type)
{
    typename CreatorMap::iterator it = _creatorMap.find(type);
    if (it == _creatorMap.end()){
        return NULL;
    }
    
    CreatorBase<Base> * pCreator = (*it).second;
    return pCreator->create();
}

template<class Base>
Base * Factory<Base>::createWithInit(std::string type)
{
    typename CreatorMap::iterator it = _creatorMap.find(type);
    if (it == _creatorMap.end()){
        return NULL;
    }
    
    CreatorBase<Base> * pCreator = (*it).second;
    return pCreator->createWithInit();
}

#endif /* defined(__okeya__Factory__) */
