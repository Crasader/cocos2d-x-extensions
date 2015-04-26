//
//  PurchaseManager.h
//  まじょのおしごと
//
//  Created by Atsushi Yoshida on 2015/03/13.
//
//

#ifndef __zeron__PurchaseManager__
#define __zeron__PurchaseManager__

#include "ProtocolIAP.h"
#include "PluginManager.h"
using namespace cocos2d::plugin;

class PurchaseManagerResult : public cocos2d::plugin::PayResultListener
{
public:
    virtual void onPayResult(cocos2d::plugin::PayResultCode ret, const char* msg, cocos2d::plugin::TProductInfo info);
    void setCallback(ProtocolIAP::ProtocolIAPCallback& callback);
private:
    ProtocolIAP::ProtocolIAPCallback _callback;
};


class PurchaseManager
{
public:
    enum ResponseCode { SUCCESS = 0, CANCEL = 1, SERVICE_UNAVAILABLE = 2 };
    
    PurchaseManager();
    static PurchaseManager* getInstance();
    bool isInitialized();
    void init();
    void buy(const std::string& IAPId, ProtocolIAP::ProtocolIAPCallback callback);
private:
    static PurchaseManager* _singleton;
    bool _init;
    ProtocolIAP* s_protocolIAP;
    std::unique_ptr<PurchaseManagerResult> s_pRetListener;
};

#endif /* defined(__zeron__PurchaseManager__) */
