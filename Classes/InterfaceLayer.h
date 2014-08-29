//
//  InterfaceLayer.h
//  pf01
//
//  Created by 野口一也 on 2014/08/20.
//
//

#ifndef __pf01__InterfaceLayer__
#define __pf01__InterfaceLayer__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum {
    CTRL_LEFT  = 0x01,
    CTRL_RIGHT = 0x02,
    CTRL_JUMP  = 0x04
};

class InterfaceDelegate
{
public:
    virtual void TouchBegan(int CtrlState) = 0;
    virtual void TouchEnded(int CtrlState) = 0;
};

class InterfaceLayer : public cocos2d::Layer
{
public:
    InterfaceLayer();
    ~InterfaceLayer();
    virtual bool init();
    
    CREATE_FUNC(InterfaceLayer);
    void setDelegate(InterfaceDelegate* pDelegate);
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    void addTargetWithActionForControlEvents(ControlButton* button, Control::Handler downAction, Control::Handler upAction);
    void leftTouchDownAction(Ref *senderz, Control::EventType controlEvent);
    void rightTouchDownAction(Ref *senderz, Control::EventType controlEvent);
    void jumpTouchDownAction(Ref *senderz, Control::EventType controlEvent);
    void leftTouchUpAction(Ref *senderz, Control::EventType controlEvent);
    void rightTouchUpAction(Ref *senderz, Control::EventType controlEvent);
    void jumpTouchUpAction(Ref *senderz, Control::EventType controlEvent);
    
    InterfaceDelegate* _delegate;
};

#endif /* defined(__pf01__InterfaceLayer__) */
