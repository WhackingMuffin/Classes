#ifndef __STARTMENU_SCENE_H__
#define __STARTMENU_SCENE_H__

#include "cocos2d.h"

class StartMenu : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(StartMenu);

	void startButtonPressed();
	void rankButtonPressed();
	void settingsButtonPressed();
	void menuCloseCallback(cocos2d::Ref* pSender);
};

#endif // __STARTMENU_SCENE_H__
