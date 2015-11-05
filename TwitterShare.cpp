//
//  TwitterShare.cpp
//  まじょのおしごと
//
//  Created by Atsushi Yoshida on 2015/11/03.
//
//

#include "TwitterShare.h"
#include <iomanip>
#include "ProtocolShare.h"
#include "PluginManager.h"

using namespace cocos2d;
using namespace cocos2d::plugin;

const std::string TWEET_KEY = "0oVCRe7Gume2m7PltSgzw";
const std::string TWEET_SECRET = "xgQKgxdwevGCtnIkwE0tOJsEjFcCdkB9t7L6d8E58M";

void TwitterShare::appendButton(Scene* scene)
{
    auto twitter =  dynamic_cast<plugin::ProtocolShare*>(plugin::PluginManager::getInstance()->loadPlugin("ShareTwitter"));
    
    TShareDeveloperInfo twitterInfo;
    twitterInfo["TwitterKey"] = TWEET_KEY;
    twitterInfo["TwitterSecret"] = TWEET_SECRET;
#if defined(COCOS2D_DEBUG)
    twitter->setDebugMode(true);
#endif
    twitter->configDeveloperInfo(twitterInfo);
    auto path = "Images/icon/tweetbutton.png";
    auto tweetButton = Button::create(path, path, path);
    tweetButton->addTouchEventListener([](Ref* pRef, Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            auto now = std::chrono::system_clock::now();
            time_t in_time_t = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << "witch";
            ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d.%H%M%S");
            ss << ".png";
            utils::captureScreen([](bool b, const std::string path){
                auto layer = Layer::create();
                layer->setScale(0.8f);
                auto image = ImageView::create(path);
                layer->addChild(image);
                auto buttonPath = "Images/icon/tweetbutton.png";
                auto tweet = Button::create(buttonPath, buttonPath, buttonPath);
                tweet->addTouchEventListener([path](Ref* pRef, Widget::TouchEventType type){
                    auto twitter =  dynamic_cast<plugin::ProtocolShare*>(plugin::PluginManager::getInstance()->loadPlugin("ShareTwitter"));
                    TShareInfo shareInfo;
                    shareInfo["SharedText"] = "tweet msg";
                    shareInfo["SharedImagePath"] = path;

                    twitter->setCallback([](int i, std::string& s){
                        log("%i, %s", i, s.c_str());
                    });
                    twitter->share(shareInfo);
                });
                layer->addChild(tweet);
                Director::getInstance()->getRunningScene()->addChild(layer);
                log("%s", path.c_str());
            }, ss.str());
        }
    });

    tweetButton->setPosition(Vec2(120, 240));
    scene->addChild(tweetButton);
}