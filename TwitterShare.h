//
//  TwitterShare.h
//  まじょのおしごと
//
//  Created by Atsushi Yoshida on 2015/11/03.
//
//

#ifndef __okeya__TwitterShare__
#define __okeya__TwitterShare__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class TwitterShare
{
public:
    void appendButton(Scene* scene, const std::string& tweetText);
    inline void setBeforeScrenShot(std::function<void(void)> fn){ _beforeScreenShot = fn; }
    inline void setBeforeTweet(std::function<void(void)> fn){ _beforeTweet = fn; }
    inline void setAfterTweet(std::function<void(void)> fn){ _afterTweet = fn; }
private:
    void appendTweetLayer(const std::string& tweetText, const std::string& imagePath);
    std::function<void(void)> _beforeScreenShot;
    std::function<void(void)> _beforeTweet;
    std::function<void(void)> _afterTweet;
};

#endif /* defined(__okeya__TwitterShare__) */
