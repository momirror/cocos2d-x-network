#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace CocosDenshion;



CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    requestData();
        
    return true;
}

void HelloWorld::requestData()
{
    cocos2d::extension::CCHttpRequest* request = new cocos2d::extension::CCHttpRequest(); 
    request->setUrl("http://www.oschina.net");
    
    //请求类型
    request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpPost);
    
    std::vector<std::string> headers;
    //内容格式，编码格式
    headers.push_back("Content-Type: application/json; charset=utf-8");
    request->setHeaders(headers);
    
//    const char* postData = "catalog=2&pageIndex=1&pageSize=5";
//    request->setRequestData(postData ,strlen(postData));
    request->setResponseCallback(this, httpresponse_selector(HelloWorld::RequestCallBack));
    request->setTag("MyRequest");
    cocos2d::extension::CCHttpClient::getInstance()->send(request);
    request->release();
}

void HelloWorld::RequestCallBack(cocos2d::CCNode *sender ,void *data)
{
    CCLog("call back");
    cocos2d::extension::CCHttpResponse *response = (cocos2d::extension::CCHttpResponse*)data;
    
    if (!response)
    {
        return;
    }
    
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        CCLog("%s completed", response->getHttpRequest()->getTag());
    }
    
    int statusCode = response->getResponseCode();
    
    char statusString[64] = {};
    
    sprintf(statusString ,"Http status code:%d ,tag = %s" ,statusCode ,response->getHttpRequest()->getTag());
    
    CCLog("response code:%d" ,statusCode);
    
    if (!response->isSucceed())
    {
        CCLog("response failed");
        CCLog("error buffer:%s" ,response->getErrorBuffer());
    }
    std::vector<char> *buffer = response->getResponseData();
    printf("Http response,dump data:");
    std::string result = "";
    for (unsigned int i = 0; i < buffer->size(); i ++)
    {
        printf("%c" ,(*buffer)[i]);
    }
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
