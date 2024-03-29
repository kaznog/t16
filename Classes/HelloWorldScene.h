#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "GameState.h"
#include "GameLayer.h"
#include "InterfaceLayer.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
    HelloWorld();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(GameState* gameState);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(GameState* gameState);
    
    // implement the "static create()" method manually
//    CREATE_FUNC(HelloWorld);
    static HelloWorld* create(GameState* gameState);
};

#endif // __HELLOWORLD_SCENE_H__
