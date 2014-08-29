//
//  GameLayer.h
//  pf01
//
//  Created by 野口一也 on 2014/08/25.
//
//

#ifndef __pf01__GameLayer__
#define __pf01__GameLayer__

#include "cocos2d.h"
#include "GameState.h"
#include "InterfaceLayer.h"

USING_NS_CC;

class GameLayer : public cocos2d::Layer, public InterfaceDelegate
{
public:
    GameLayer();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(GameState* gameState);
    virtual void update(float dt);
    
    // implement the "static create()" method manually
//    CREATE_FUNC(GameLayer);
    static GameLayer* create(GameState* gameState);
    
    virtual void TouchBegan(int CtrlState);
    virtual void TouchEnded(int CtrlState);

private:
    void setViewPointCenter(Vec2 position);
    float tileMapHeight();
    float tileMapWidth();
    bool isValidPosition(Vec2 position);
    bool isValidTileCoord(Vec2 tileCoord);
    Vec2 positionForTileCoord(Vec2 tilecoord);
    Vec2 tileCoordForPosition(Vec2 position);
    
    TMXTiledMap* _tileMap;
    TMXLayer*    _walls;
    
    Sprite*      _player;
    float          _vx;
    int          _vy;
    bool         _isJump;
    bool         _isFailed;
    bool         _isFlipped;
    void jumpPlayer(int offset, Vec2& playerPos);
    
    float        _ch_whalf;
    float        _ch_hhalf;
    
    GameState* _gameState;
    int _controlState;
    
    AnimationCache* _animCache;
    void setAnim(Sprite* sprite, const std::string& name);
};

#endif /* defined(__pf01__GameLayer__) */
