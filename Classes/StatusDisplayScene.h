//
//  StatusDisplayScene.h
//  pf01
//
//  Created by 野口一也 on 2014/08/26.
//
//

#ifndef __pf01__StatusDisplayScene__
#define __pf01__StatusDisplayScene__

#include "cocos2d.h"
#include "GameState.h"

USING_NS_CC;

class StatusDisplay : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(GameState* gameState);
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(GameState* gameState);
    
    // implement the "static create()" method manually
    static StatusDisplay* create(GameState* gameState);
};

#endif /* defined(__pf01__StatusDisplayScene__) */
