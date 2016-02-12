#include "HelloWorldScene.h"
#include "StartMenuScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
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
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
	/*
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
	*/

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Accept.mp3");
	auto label = Label::createWithTTF("Presents", "fonts/ARMYRUST.ttf", 34);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width*0.8,
                            origin.y + visibleSize.height*0.05));

	// change the colour of the label to grey
	label->setColor(ccc3(84,84,84));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("TeamNA_logo.png");

	// set the anchor point to the middle
	sprite->setAnchorPoint(Vec2(0.5, 0.5));

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	// scale the image to fill the screen
	float scale = MIN(visibleSize.width / sprite->getContentSize().width, visibleSize.height / sprite->getContentSize().height);
	sprite->setScale(scale);

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    return true;
}

void HelloWorld::onEnter() {
	CCLayer::onEnter();
	// CCLog("onEnter");
	printf("onEnter");
	this->scheduleOnce(schedule_selector(HelloWorld::finishSplash), 1.0f);
}

void HelloWorld::finishSplash(float dt) {
	// CCDirector::sharedDirector()->replaceScene(StartMenu::scene());
	auto startMenuScene = StartMenu::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(2, startMenuScene));
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
