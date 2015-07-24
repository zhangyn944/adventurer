//
//  DialogLayer.h
//  ModalDialogDemo
//
//  Created by Tom on 12-5-28.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ModalDialogDemo_DialogLayer_h
#define ModalDialogDemo_DialogLayer_h


#include "cocos2d.h"
#include "Slide.h"
#include "HelloWorldScene.h"


USING_NS_CC;

class DialogLayer: public CCLayerColor
{
    // 模态对话框菜单
    CCMenu *m_pMenu;
    // 记录菜单点击
    bool m_bTouchedMenu;
    
public:
    DialogLayer();
    ~DialogLayer();
    
    virtual bool init();
    // 初始化对话框内容
    void initDialog();
    
	CREATE_FUNC(DialogLayer);
    
	static DialogLayer * createLayer();

    void onEnter();
    void onExit();
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    void okMenuItemCallback(CCObject *pSender);
    void cancelMenuItemCallback(CCObject *pSender);
};

#endif
