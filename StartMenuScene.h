#ifndef __STARTMENU_SCENE_H__
#define __STARTMENU_SCENE_H__

#include "cocos2d.h"
#include <string>

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
	void update(float dt);
	void updateFactTimer(float dt);
	void onEnter();
	void menuCloseCallback(cocos2d::Ref* pSender);

private:
	cocos2d::Label* didYouKnowLabel;

};

#endif // __STARTMENU_SCENE_H__
