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
#include "Env.h"
#include "BGMPlayer.h"

using namespace cocos2d;
using namespace cocos2d::plugin;

void TwitterShare::appendButton(Scene* scene, const std::string& tweetText)
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
    tweetButton->addTouchEventListener([&, tweetText, tweetButton](Ref* pRef, Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
             BGMPlayer::play2d("sound/se_button_push_01.mp3");
            auto now = std::chrono::system_clock::now();
            time_t in_time_t = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << "witch";
            ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d.%H%M%S");
            ss << ".png";
            tweetButton->setVisible(false);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            utils::captureScreen([&, tweetText](bool b, const std::string path){
                Director::getInstance()->getEventDispatcher()->setEnabled(false);
                auto twitter =  dynamic_cast<plugin::ProtocolShare*>(plugin::PluginManager::getInstance()->loadPlugin("ShareTwitter"));
                TShareInfo shareInfo;
                shareInfo["SharedText"] = tweetText;
                shareInfo["SharedImagePath"] = path;
                twitter->setCallback([&](int i, std::string& s){
                    _afterTweet();
                    Director::getInstance()->getEventDispatcher()->setEnabled(true);
                    log("TwitterPluginCallback %i, %s", i, s.c_str());
                });
                twitter->share(shareInfo);
            }, ss.str());
#else
            appendTweetLayer(tweetText, ss.str());
#endif

        }
    });
    auto winSize = Director::getInstance()->getWinSize();
    auto btnSize = tweetButton->getContentSize();
    tweetButton->setPosition(Vec2(winSize.width - btnSize.width / 2, btnSize.height / 2));
    scene->addChild(tweetButton);
}


void TwitterShare::appendTweetLayer(const std::string& tweetText, const std::string& imagePath)
{
    utils::captureScreen([&, tweetText](bool b, const std::string path){
        auto layer = Layer::create();
        layer->setScale(0.8f);
        auto image = ImageView::create(path);
        layer->addChild(image);
        auto buttonPath = "Images/icon/tweetbutton.png";
        auto tweet = Button::create(buttonPath, buttonPath, buttonPath);
        tweet->addTouchEventListener([&, path, tweetText, layer](Ref* pRef, Widget::TouchEventType type){
            Director::getInstance()->getEventDispatcher()->setEnabled(false);
            auto twitter =  dynamic_cast<plugin::ProtocolShare*>(plugin::PluginManager::getInstance()->loadPlugin("ShareTwitter"));
            TShareInfo shareInfo;
            shareInfo["SharedText"] = tweetText;
            shareInfo["SharedImagePath"] = path;
            
            twitter->setCallback([&, layer](int i, std::string& s){
                _afterTweet();
                layer->removeFromParent();
                Director::getInstance()->getEventDispatcher()->setEnabled(true);
                log("TwitterPluginCallback %i, %s", i, s.c_str());
            });
            twitter->share(shareInfo);
        });
        layer->addChild(tweet);
        Director::getInstance()->getRunningScene()->addChild(layer);
        Director::getInstance()->getEventDispatcher()->setEnabled(true);
        log("%s", path.c_str());
    }, imagePath);

}
