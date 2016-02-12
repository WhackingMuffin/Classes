#include "ui/CocosGUI.h"
#include <iostream>
#include "StartMenuScene.h"
#include "Level1Scene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* StartMenu::createScene()
{
	// 'startMenuScene' is an autorelease object
	auto startMenuScene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = StartMenu::create();

	// add layer as a child to scene
	startMenuScene->addChild(layer);

	// return the scene
	return startMenuScene;
}

// on "init" you need to initialize your instance
bool StartMenu::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(StartMenu::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);


	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "START"
	// create and initialize a label

	// auto label = Label::createWithTTF("START", "fonts/Marker Felt.ttf", 24);

	// position the label on the 3/4 the width of the screen 
	// and at the lower part of the logo
	/*label->setPosition(Vec2(origin.x + visibleSize.width / 2 + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));*/

	// add the label as a child to this layer
	// this->addChild(label, 1);

	// add "TeamNA" splash screen" with logo
	auto sprite = Sprite::create("Main_Menu.png");
	// set the anchor point to the middle
	// Bottom Left (0,0), Top Right (1,1)
	sprite->setAnchorPoint(Vec2(0.5, 0.5));
	// position the sprite on the center of the screen
	// make it fill the screen
	/*
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	sprite->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.5));
	*/
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(sprite, 0);

	// Add a button to start
	// Add a button to start
	ui::Button* startBtn = ui::Button::create("Normal-Button.png");
	startBtn->setPosition(Vec2(visibleSize.width / 1.80 + origin.x, visibleSize.height*0.57 + origin.y));
	startBtn->setScale(0.6f);
	startBtn->setTitleText(" ");
	startBtn->setTitleFontSize(80);
	startBtn->setColor(Color3B(159, 168, 176));
	startBtn->addTouchEventListener(CC_CALLBACK_0(StartMenu::startButtonPressed, this));
	this->addChild(startBtn);

	//Add a Rankings Button
	ui::Button* rankBtn = ui::Button::create("Ranking-Button.png");
	rankBtn->setPosition(Vec2(visibleSize.width / 1.80 + origin.x, visibleSize.height*0.50 + origin.y));
	rankBtn->setScale(0.6f);
	rankBtn->setTitleText(" ");
	rankBtn->setTitleFontSize(80);
	rankBtn->setColor(Color3B(159, 168, 176));
	rankBtn->addTouchEventListener(CC_CALLBACK_0(StartMenu::startButtonPressed, this));
	this->addChild(rankBtn);

	//Add a Settings Button
	ui::Button* settingsBtn = ui::Button::create("Setting-Button.png");
	settingsBtn->setPosition(Vec2(visibleSize.width / 1.80 + origin.x, visibleSize.height*0.435 + origin.y));
	settingsBtn->setScale(0.6f);
	settingsBtn->setTitleText(" ");
	settingsBtn->setTitleFontSize(80);
	settingsBtn->setColor(Color3B(159, 168, 176));
	settingsBtn->addTouchEventListener(CC_CALLBACK_0(StartMenu::startButtonPressed, this));
	this->addChild(settingsBtn);

	//Add music
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("march.wav", true);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Sound1.wav");
	return true;
}

void StartMenu::startButtonPressed()
{
	// get startMenu scene and run it
	auto loadLevel1 = Level1::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(2, loadLevel1));
}

void StartMenu::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
//  #endif

