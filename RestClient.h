//
//  RestClient.h
//  zeron
//
//  Created by Atsushi Yoshida on 2014/07/09.
//
//

#ifndef __zeron__RestClient__
#define __zeron__RestClient__

#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include <unordered_map>
#include "Env.h"

using namespace cocos2d::network;

class RestClient
{
public:
    typedef std::unordered_map<std::string, std::string> PostData;
    static RestClient* getInstance();
    static const std::string URL_PREFIX;
    static const std::string ASSET_URL_PREFIX;

    void get( const std::string path, const ccHttpRequestCallback callback);
    void post(const std::string path, const ccHttpRequestCallback callback);
    void post(const std::string& path, PostData& postData, const ccHttpRequestCallback callback);
    void put(const std::string& path, PostData& postData, const ccHttpRequestCallback callback);
    void destroy(const std::string& path, PostData& postData, const ccHttpRequestCallback callback);
    void send(HttpRequest::Type type, const std::string& path, PostData& postData, const ccHttpRequestCallback& callback);
};

#endif /* defined(__zeron__RestClient__) */
