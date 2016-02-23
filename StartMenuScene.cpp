#include "ui/CocosGUI.h"
#include <iostream>
#include "StartMenuScene.h"
#include "Level1Scene.h"
#include "SimpleAudioEngine.h"
#include "DataModel.h"

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

	DataModel *m = DataModel::getModel();


	// DID YOU KNOW???
	// Add Did You Know?? headings
	auto didYouKnowHeadingLabel = Label::createWithTTF("Did You Know???", "fonts/ARMYRUST.ttf", 24);

	// position the label on the center of the screen
	didYouKnowHeadingLabel->setPosition(Vec2(origin.x + visibleSize.width*0.73,
		origin.y + visibleSize.height*0.29));

	// change the colour of the label to grey
	didYouKnowHeadingLabel->setColor(ccc3(81, 23, 117));

	// add the label as a child to this layer
	this->addChild(didYouKnowHeadingLabel, 3);

	// Add Did You Know?? fact text
	didYouKnowLabel = Label::createWithTTF("...", "fonts/ARMYRUST.ttf", 11, CCSizeMake(250, 70), TextHAlignment::CENTER, TextVAlignment::CENTER);
	// position the label on the center of the screen
	didYouKnowLabel->setPosition(Vec2(origin.x + visibleSize.width*0.73,
		origin.y + visibleSize.height*0.16));
	// change the colour of the label to grey
	didYouKnowLabel->setColor(ccc3(81, 23, 117));
	// didYouKnowLabel->setString(didYouKnowRandomFact.getCString());
	// add the label as a child to this layer
	this->addChild(didYouKnowLabel, 3);

	updateFactTimer(0.1f);

	this->scheduleUpdate();

	return true;
}

void StartMenu::startButtonPressed()
{
	// get startMenu scene and run it
	auto loadLevel1 = Level1::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(2, loadLevel1));
}

void StartMenu::updateFactTimer(float dt) {
	DataModel *m = DataModel::getModel();
	// Director::getInstance()->
	// StartMenu* dYKLabel = (StartMenu *)m->waves.at(this->currentLevel);

	// Did You Know??? Fact section
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	const char *factArray[] = {
		"In WW1, trenches were used to stop enemy from advancing.",
		"By the end of 1914, both sides of the war had built trenches that went from the North Sea and through Belgium and France.",
		"It is estimated there were about 2,490km of trench lines dug during WW1.",
		"Most soldiers would spend anywhere from 1 day up to 2 weeks in the trenches at a time.",
		"Trenches weren't dug in straight lines, but in zig-zag pattern.",
		"Many soldiers living in the trenches suffered from Trench Foot. Rain and bad weather would flood the trenches making them boggy, muddy and could even block weapons and make it hard to move in battle.",
		"Trenches typically had an embankment at the top and a barbed wire fence. Often, trenches in World War 1 would be reinforced with sandbags and wooden beams.",
		"In the trench itself, the bottom was covered with wooden boards called duckboards. These were meant to protect the soldiers’ feet from the water in the trenches to try and prevent Trench Foot.",
		"The land between the two enemy trench lines was called “No Man’s Land.”  No Man’s Land was sometimes covered with land mines and barbed wire. The distance between enemy trenches was anywhere from 50 to 250 yards apart.",
		"Trench Digging Method #1: Soldiers would simply dig the trenches straight into the ground – a method known as entrenching. Entrenching was fast, but the soldiers were open to enemy fire while they dug.",
		"Trench Digging Method #2: A solider would extend a trench on one end. This was called 'sapping' and was a safer method but took a lot longer.",
		"Trench Digging Method #3: Soldiers would dig a tunnel and then remove the roof to make a trench when done. This 'tunneling' method was the safest and most difficult." ,
		"The noise and uncomfortable surroundings made it very difficult to sleep in trenches. Soldiers were constantly tired and in danger of falling asleep. This is why the watch shift was kept to 2 hours to avoid men falling asleep while on watch.",
		"There were several cease fires or truces in the trenches during WWI. In 1914, around Christmas time both the British and German soldiers put down their weapons, came out of their trenches and exchanged gifts and sung carols – ceasing fire to celebrate Christmas. This is now known as the Christmas Truce."
	};
	const size_t messages_count = sizeof(factArray) / sizeof(factArray[0]);

	String didYouKnowRandomFact = factArray[rand() % messages_count];
	didYouKnowLabel->setString(didYouKnowRandomFact.getCString());
	
}

void StartMenu::update(float dt)
{
	
}

void StartMenu::onEnter() {
	CCLayer::onEnter();
	// CCLog("onEnter");
	printf("onEnter");
	this->schedule(schedule_selector(StartMenu::updateFactTimer), 3.0f);
	this->scheduleUpdate();
}

void StartMenu::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

//  #endif

