//
//  InterfaceLayer.cpp
//  pf01
//
//  Created by 野口一也 on 2014/08/20.
//
//

#include "InterfaceLayer.h"

InterfaceLayer::InterfaceLayer()
: _delegate(nullptr)
{
    
}

InterfaceLayer::~InterfaceLayer()
{
    _delegate = nullptr;
}

void InterfaceLayer::setDelegate(InterfaceDelegate *pDelegate)
{
    _delegate = pDelegate;
}

bool InterfaceLayer::init()
{
    if ( !Layer::init() ) {
        return false;
    }
    auto director = Director::getInstance();
    Size visibleSize = director->getVisibleSize();
    Vec2 origin = director->getVisibleOrigin();
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(InterfaceLayer::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + visibleSize.height - closeItem->getContentSize().height/2));
    
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    auto mask = LayerColor::create(Color4B::BLACK, 320, 480 - 320);
    this->addChild(mask);
    auto spLeft = Scale9Sprite::create("leftbutton.png");
    auto leftButton = ControlButton::create(spLeft);
    leftButton->setPreferredSize(Size(100, 100));
    leftButton->setPosition(Vec2(50, 50));
    this->addChild(leftButton);
    
    auto spRight = Scale9Sprite::create("rightbutton.png");
    auto rightButton = ControlButton::create(spRight);
    rightButton->setPreferredSize(Size(100, 100));
    rightButton->setPosition(Vec2(200, 50));
    this->addChild(rightButton);
    
    auto spAccel = Scale9Sprite::create("accelbutton.png");
    auto jumpButton = ControlButton::create(spAccel);
    jumpButton->setPreferredSize(Size(100, 50));
    jumpButton->setPosition(Vec2(origin.x + visibleSize.width - 50, origin.y + 125));
    this->addChild(jumpButton);
    
    addTargetWithActionForControlEvents(leftButton, cccontrol_selector(InterfaceLayer::leftTouchDownAction), cccontrol_selector(InterfaceLayer::leftTouchUpAction));
    addTargetWithActionForControlEvents(rightButton, cccontrol_selector(InterfaceLayer::rightTouchDownAction), cccontrol_selector(InterfaceLayer::rightTouchUpAction));
    addTargetWithActionForControlEvents(jumpButton, cccontrol_selector(InterfaceLayer::jumpTouchDownAction), cccontrol_selector(InterfaceLayer::jumpTouchUpAction));
    
    return true;
}

void InterfaceLayer::menuCloseCallback(cocos2d::Ref *pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void InterfaceLayer::addTargetWithActionForControlEvents(cocos2d::extension::ControlButton *button, Control::Handler downAction, Control::Handler upAction)
{
    button->addTargetWithActionForControlEvents(this, downAction, Control::EventType::TOUCH_DOWN | Control::EventType::DRAG_ENTER);
    
    button->addTargetWithActionForControlEvents(this, upAction, Control::EventType::TOUCH_CANCEL | Control::EventType::TOUCH_UP_INSIDE | Control::EventType::TOUCH_UP_OUTSIDE | Control::EventType::DRAG_OUTSIDE | Control::EventType::DRAG_EXIT);
}

void InterfaceLayer::leftTouchDownAction(cocos2d::Ref *senderz, Control::EventType controlEvent)
{
    log("leftButton touch down");
    if (_delegate != nullptr) {
        _delegate->TouchBegan(CTRL_LEFT);
    }
}

void InterfaceLayer::rightTouchDownAction(cocos2d::Ref *senderz, Control::EventType controlEvent)
{
    log("rightButton touch down");
    if (_delegate != nullptr) {
        _delegate->TouchBegan(CTRL_RIGHT);
    }
}

void InterfaceLayer::jumpTouchDownAction(cocos2d::Ref *senderz, Control::EventType controlEvent)
{
    log("accelButton touch down");
    if (_delegate != nullptr) {
        _delegate->TouchBegan(CTRL_JUMP);
    }
}

void InterfaceLayer::leftTouchUpAction(cocos2d::Ref *senderz, Control::EventType controlEvent)
{
    log("leftButton touch up");
    //    auto button = (ControlButton*)senderz;
    if (_delegate != nullptr) {
        _delegate->TouchEnded(CTRL_LEFT);
    }
}

void InterfaceLayer::rightTouchUpAction(cocos2d::Ref *senderz, Control::EventType controlEvent)
{
    log("rightButton touch up");
    if (_delegate != nullptr) {
        _delegate->TouchEnded(CTRL_RIGHT);
    }
}

void InterfaceLayer::jumpTouchUpAction(cocos2d::Ref *senderz, Control::EventType controlEvent)
{
    log("jumpButton touch up");
    if (_delegate != nullptr) {
        _delegate->TouchEnded(CTRL_JUMP);
    }
}

