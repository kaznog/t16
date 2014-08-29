#include "HelloWorldScene.h"
#include "Constants.h"

USING_NS_CC;

Scene* HelloWorld::createScene(GameState* gameState)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create(gameState);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld* HelloWorld::create(GameState *gameState)
{
    HelloWorld* pRet = new HelloWorld();
    if (pRet && pRet->init(gameState)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

HelloWorld::HelloWorld()
{
}

// on "init" you need to initialize your instance
bool HelloWorld::init(GameState* gameState)
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    glview->setDesignResolutionSize(320, 480, ResolutionPolicy::SHOW_ALL);
    
    Size visibleSize = director->getVisibleSize();
    Vec2 origin = director->getVisibleOrigin();

    auto gamelayer = GameLayer::create(gameState);
    gamelayer->setPosition(Vec2(0, visibleSize.height - GAMELAYER_HEIGHT));
    this->addChild(gamelayer, ZORDER_TILEMAP);
    
    auto interface = InterfaceLayer::create();
    interface->setDelegate(gamelayer);
    this->addChild(interface, ZORDER_INTERFACE);
    return true;
}
