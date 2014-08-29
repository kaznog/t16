//
//  GameLayer.cpp
//  pf01
//
//  Created by 野口一也 on 2014/08/25.
//
//

#include "GameLayer.h"
#include "Constants.h"
#include "StatusDisplayScene.h"

USING_NS_CC;

GameLayer* GameLayer::create(GameState *gameState)
{
    GameLayer* pRet = new GameLayer();
    if (pRet && pRet->init(gameState)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

GameLayer::GameLayer()
: _tileMap(nullptr)
, _walls(nullptr)
, _player(nullptr)
, _isJump(false)
, _isFailed(false)
, _isFlipped(false)
, _gameState(nullptr)
, _animCache(nullptr)
, _controlState(0x0)
{
    
}

bool GameLayer::init(GameState* gameState)
{
    if (!Layer::init()) {
        return false;
    }
    _gameState = gameState;
    setContentSize(Size(GAMELAYER_WIDTH, GAMELAYER_HEIGHT));
    auto spriteCache = SpriteFrameCache::getInstance();
    spriteCache->addSpriteFramesWithFile("sprites_default.plist");
    _animCache = AnimationCache::getInstance();
    _animCache->addAnimationsWithFile("mario_anim.plist");
    
    _tileMap = TMXTiledMap::create("level1.tmx");
    _walls   = _tileMap->getLayer("walls");
    this->addChild(_tileMap, ZORDER_TILEMAP);
//    _player  = Sprite::create("mario_00.png");
    _player  = Sprite::createWithSpriteFrameName("mario_00.png");
    setAnim(_player, "idle");
    _player->setPosition(positionForTileCoord(Vec2(3, 16)));
    this->addChild(_player, ZORDER_PLAYER);
    _ch_whalf = _player->getContentSize().width/2;
    _ch_hhalf = _player->getContentSize().height/2;
    _vx = 0.0f;
    scheduleUpdate();
    return true;
}

void GameLayer::update(float dt)
{
    float blockWidth = _tileMap->getTileSize().width;
    Vec2 playerPos = _player->getPosition();
    log("x:%f", playerPos.x);
    if (!_isFailed) {
        Vec2 forwardTileCoord = Vec2(-1, -1);
        if (_controlState & CTRL_LEFT) {
            if (_vx > 0.0f) {
                _vx = 0.0f;
            } else {
                if (_vx == 0.0f && !_isJump) {
                    setAnim(_player, "walk");
                }
                _vx -= MOVE_SPEED;
                if (_vx < -MAX_MOVE_SPEED) {
                    _vx = -MAX_MOVE_SPEED;
                }
            }
            if (!_isFlipped) {
                _isFlipped = true;
            }
            forwardTileCoord = tileCoordForPosition(Vec2(playerPos.x + _vx - _ch_whalf, playerPos.y));
        } else if (_controlState & CTRL_RIGHT) {
            if (_vx < 0.0f) {
                _vx = 0.0f;
            } else {
                if (_vx == 0.0f && !_isJump) {
                    setAnim(_player, "walk");
                }
                _vx += MOVE_SPEED;
                if (_vx > MAX_MOVE_SPEED) {
                    _vx = MAX_MOVE_SPEED;
                }
            }
            if (_isFlipped) {
                _isFlipped = false;
            }
            forwardTileCoord = tileCoordForPosition(Vec2(playerPos.x + _vx + _ch_whalf, playerPos.y));
        } else {
            _vx = 0;
            if (!_isJump) {
                setAnim(_player, "idle");
            }
        }
        if (isValidTileCoord(forwardTileCoord)) {
            int forwardGid = _walls->getTileGIDAt(forwardTileCoord);
            if (forwardGid == 0) {
                playerPos.x = playerPos.x + _vx;
                _player->setPosition(playerPos);
                _player->setFlippedX(_isFlipped);
            }
        }
    }
    float block_num = floorf((playerPos.x) / blockWidth);
    float block_offset = blockWidth * block_num;
    float protrusion = (playerPos.x) - block_offset;
    int offset = 0;
    if (protrusion <= (0.3 * blockWidth)) {
        offset = -1;
    } else if (protrusion >= (0.7 * blockWidth)) {
        offset = 1;
    }
    log("offset:%d", offset);
    if (_isJump) {
        jumpPlayer(offset, playerPos);
        _player->setPosition(playerPos);
    }
    
    setViewPointCenter(_player->getPosition());
    if (!_isFailed) {
        
        Vec2 posTileCoord  = tileCoordForPosition(Vec2(playerPos.x, playerPos.y));
        Vec2 offsetTileCoord = posTileCoord;
        offsetTileCoord.x += offset;
        posTileCoord.y  += 1;
        offsetTileCoord.y += 1;
        int posTileGid = -1;
        int offsetTileGid = -1;
        if (isValidTileCoord(posTileCoord)) {
            posTileGid  = _walls->getTileGIDAt(posTileCoord);
        }
        if (isValidTileCoord(offsetTileCoord)) {
            offsetTileGid = _walls->getTileGIDAt(offsetTileCoord);
        }
        if (posTileGid == 0 && offsetTileGid == 0) {
            if (_isJump == false) {
                _isJump = true;
                _vy = -2;
                setAnim(_player, "jump");
            }
        }
    }
}

void GameLayer::jumpPlayer(int offset, Vec2& playerPos)
{
    
//    Vec2 playerPos = _player->getPosition();
    playerPos.y = playerPos.y + _vy;
    _vy = _vy + DOWN_FORCE;
    
    if (_vy > 0) {
        /**
         * _vy > 0 == ジャンプ中
         */
        Vec2 posTileCoord  = tileCoordForPosition(Vec2(playerPos.x, playerPos.y + _ch_hhalf));
        Vec2 offsetTileCoord = posTileCoord;
        offsetTileCoord.x += offset;
        
        int posTileGid  = _walls->getTileGIDAt(posTileCoord);
        int offsetTileGid = _walls->getTileGIDAt(offsetTileCoord);
        if (posTileGid != 0 || offsetTileGid != 0) {
            _vy = 0;
        }
    } else {
        /**
         * 降下中
         */
        if (!_isFailed) {
            Vec2 posTileCoord  = tileCoordForPosition(Vec2(playerPos.x, playerPos.y - _ch_hhalf));
            Vec2 offsetTileCoord = posTileCoord;
            offsetTileCoord.x += offset;
            if (!isValidTileCoord(posTileCoord) || !isValidTileCoord(offsetTileCoord)) {
                _isFailed = true;
                auto action = Sequence::create(DelayTime::create(1), CallFunc::create([this](){
                    this->_gameState->_remaining -= 1;
                    Director::getInstance()->replaceScene(StatusDisplay::createScene(this->_gameState));
                }), NULL);
                
                runAction(action);
                
                return;
            }
            int posTileGid  = _walls->getTileGIDAt(posTileCoord);
            int offsetTileGid = _walls->getTileGIDAt(offsetTileCoord);
            if (posTileGid != 0 || offsetTileGid != 0) {
                // 降下終了
                log("着地");
                _isJump = false;
                posTileCoord.y -= 1;
                playerPos.y = positionForTileCoord(posTileCoord).y; // + _ch_hhalf;
                if (_vx == 0.0f) {
                    setAnim(_player, "idle");
                } else {
                    setAnim(_player, "walk");
                }
            }
        }
    }
//    _player->setPosition(playerPos);
}

void GameLayer::setViewPointCenter(cocos2d::Vec2 position)
{
    auto winSize = Director::getInstance()->getWinSize();
    float x = MAX(position.x, winSize.width / 2);
    float y = MAX(position.y, winSize.height / 2);
    x = MIN(x, (_tileMap->getMapSize().width * _tileMap->getTileSize().width) - winSize.width / 2);
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height / 2);
    auto actualPosition = Vec2(x, y);
    auto centerOfView = Vec2(winSize.width/2, winSize.height/2);
    auto viewPoint = centerOfView - actualPosition;
    this->setPosition(viewPoint);
}

/**
 * tile mapのスクリーン上での高さを得る
 */
float GameLayer::tileMapHeight()
{
    return _tileMap->getMapSize().height * _tileMap->getTileSize().height;
}

/**
 * tile mapのスクリーン上での幅を得る
 */
float GameLayer::tileMapWidth()
{
    return _tileMap->getMapSize().width * _tileMap->getTileSize().width;
}

/**
 * スクリーン座標がtile map内かを判定
 */
bool GameLayer::isValidPosition(cocos2d::Vec2 position)
{
    if (position.x < 0.0f
        ||  position.y < 0.0f
        ||  position.x > tileMapWidth()
        ||  position.y > tileMapHeight()) {
        return false;
    }
    return true;
}

/**
 * tile coordがtile map内かを判定
 */
bool GameLayer::isValidTileCoord(cocos2d::Vec2 tileCoord)
{
    if (tileCoord.x < 0
        ||  tileCoord.y < 0
        ||  tileCoord.x >= _tileMap->getMapSize().width
        ||  tileCoord.y >= _tileMap->getMapSize().height) {
        return false;
    }
    return true;
}

/**
 * Tile位置からスクリーン座標を得る
 */
Vec2 GameLayer::positionForTileCoord(cocos2d::Vec2 tilecoord)
{
    int x = (tilecoord.x * _tileMap->getTileSize().width) + _tileMap->getTileSize().width/2;
    int y = tileMapHeight() - (tilecoord.y * _tileMap->getTileSize().height) - _tileMap->getTileSize().height/2;
    return Vec2(x, y);
}

/**
 * スクリーン座標からTile位置を得る
 */
Vec2 GameLayer::tileCoordForPosition(cocos2d::Vec2 position)
{
    if (!isValidPosition(position)) {
        return Vec2(-1, -1);
    }
    int x = position.x / _tileMap->getTileSize().width;
    int y = (tileMapHeight() - position.y) / _tileMap->getTileSize().height;
    return Vec2(x, y);
}

void GameLayer::TouchBegan(int CtrlState)
{
    switch (CtrlState) {
        case CTRL_LEFT:
            _controlState &= ~CTRL_RIGHT;
            _controlState |= CTRL_LEFT;
            break;
            
        case CTRL_RIGHT:
            _controlState &= ~CTRL_LEFT;
            _controlState |= CTRL_RIGHT;
            break;
            
        case CTRL_JUMP:
            if ( _isJump == false ) {
                _isJump = true;
                _vy = JUMP_FORCE;
                _controlState |= CTRL_JUMP;
                setAnim(_player, "jump");
            }
            break;
            
        default:
            break;
    }
}

void GameLayer::TouchEnded(int CtrlState)
{
    switch (CtrlState) {
        case CTRL_LEFT:
            _controlState &= ~CTRL_LEFT;
            break;
            
        case CTRL_RIGHT:
            _controlState &= ~CTRL_RIGHT;
            break;
            
        case CTRL_JUMP:
            _controlState &= ~CTRL_JUMP;
            break;
            
        default:
            break;
    }
}

void GameLayer::setAnim(Sprite* sprite, const std::string& name)
{
    log("setAnim: %s", name.c_str());
    sprite->stopActionByTag(TAG_ACTION_ANIMATE);
    auto animation = _animCache->getAnimation(name);
    auto action = Animate::create(animation);
    action->setTag(TAG_ACTION_ANIMATE);
    sprite->runAction(action);
}
