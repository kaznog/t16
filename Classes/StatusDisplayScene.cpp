//
//  StatusDisplayScene.cpp
//  pf01
//
//  Created by 野口一也 on 2014/08/26.
//
//

#include "StatusDisplayScene.h"
#include "Constants.h"
#include "HelloWorldScene.h"

USING_NS_CC;

Scene* StatusDisplay::createScene(GameState* gameState)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = StatusDisplay::create(gameState);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

StatusDisplay* StatusDisplay::create(GameState *gameState)
{
    StatusDisplay* pRet = new StatusDisplay();
    if (pRet && pRet->init(gameState)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

// on "init" you need to initialize your instance
bool StatusDisplay::init(GameState* gameState)
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
    
    auto spriteCache = SpriteFrameCache::getInstance();
    spriteCache->addSpriteFramesWithFile("sprites_default.plist");
    
    auto player = Sprite::createWithSpriteFrameName("mario_00.png");
    player->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2 - player->getContentSize().height));
    addChild(player);
    auto action = Sequence::create(DelayTime::create(1), CallFunc::create([gameState](){Director::getInstance()->replaceScene(HelloWorld::createScene(gameState));}), NULL);
    
    runAction(action);
    return true;
}
