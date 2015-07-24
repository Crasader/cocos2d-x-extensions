//
//  PurchaseManager.cpp
//  まじょのおしごと
//
//  Created by Atsushi Yoshida on 2015/03/13.
//
//

#include "platform/CCCommon.h"
#include "PurchaseManager.h"
#include "Env.h"

using namespace cocos2d;
using namespace cocos2d::plugin;

PurchaseManager::PurchaseManager()
: _init(false)
, s_pRetListener(new PurchaseManagerResult)
{
}

bool PurchaseManager::isInitialized()
{
    return _init;
}

PurchaseManager* PurchaseManager::getInstance()
{
    static PurchaseManager s_instance;
    return &s_instance;
}

void PurchaseManager::init()
{
    if(_init){
        return;
    }

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    TIAPDeveloperInfo pGoogleInfo;
    pGoogleInfo["GooglePlayAppKey"] = GOOGLE_APPKEY;
    
    if(pGoogleInfo.empty()) {
        char msg[256] = { 0 };
        sprintf(msg, "Google App Key info is empty. PLZ fill your Google App Key info in %s(nearby line %d)", __FILE__, __LINE__);
        MessageBox(msg, "Google IAP Warning");
    }
    s_protocolIAP = dynamic_cast<ProtocolIAP*>(PluginManager::getInstance()->loadPlugin("IAPGooglePlay"));
    s_protocolIAP->configDeveloperInfo(pGoogleInfo);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    TIAPDeveloperInfo info;
    s_protocolIAP = dynamic_cast<ProtocolIAP*>(PluginManager::getInstance()->loadPlugin("IOSIAP"));
    info["iapKeys"] = IOS_PRODUCT_KEYS;
    s_protocolIAP->configDeveloperInfo(info);
#endif

#if COCOS2D_DEBUG
    s_protocolIAP->setDebugMode(true);
#endif
    
    _init = true;
}

void PurchaseManager::buy(const std::string& IAPId, ProtocolIAP::ProtocolIAPCallback callback)
{
    s_pRetListener->setCallback(callback);
    s_protocolIAP->setResultListener(s_pRetListener.get());
    s_protocolIAP->onPayResult(PayResultCode::kPayCancel, "Start Paying");
    TProductInfo info;
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    info["IAPId"] = IAPId;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    info["productId"] = IAPId;
#endif
    
    s_protocolIAP->payForProduct(info);
}

void PurchaseManagerResult::setCallback(ProtocolIAP::ProtocolIAPCallback& callback)
{
    _callback = callback;
}

void PurchaseManagerResult::onPayResult(PayResultCode ret, const char* msg, TProductInfo info)
{
    std::string str(msg);
     _callback(ret, str);
}
