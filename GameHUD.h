#pragma once
#include "cocos2d.h"

USING_NS_CC;

class GameHUD : public Layer
{
public:
	Sprite* background;
	Sprite* selSpriteRange;
	Sprite* selSprite;
	Vector<Sprite*> movableSprites;
	static GameHUD* _sharHUD;
	static CCScene scene();
	virtual bool init();
	static GameHUD* shareHUD();
	CREATE_FUNC(GameHUD);

	virtual void onEnter();
	bool onTouchBegan(Touch *touch, Event *event);
	void onTouchMoved(Touch *touch, Event *event);
	void onTouchEnded(Touch* touch, Event* event);

	void numCollectedChanged(int numCollected);

private:
	cocos2d::Label *scoreLabel;
	CCLabelTTF *_label;
};