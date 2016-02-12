#include "GameHUD.h"
#include "DataModel.h"
#include "Level1Scene.h"

GameHUD* GameHUD::_sharHUD;

bool GameHUD::init()
{
	DataModel *m = DataModel::getModel();
	GameHUD* score;

	//__String *tempscore = __String::createWithFormat("%i", b);

	//	scoreLabel = Label::createWithTTF(tempscore->getCString(), "Helvetica", 12);

	if (!Layer::init())
	{
		return false;
	}

	// Get the size of the game window
	Size winSize = CCDirector::getInstance()->getWinSize();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	_label = new CCLabelTTF();
	_label->initWithString("5", "Verdana-Bold", 18.0);
	_label->setColor(ccc3(0, 0, 0));

	// Draw the background of the game HUD
	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA8888);
	background = Sprite::create("hud.png");
	background->setScaleX(0.8);

	// Create an anchor point at the bottom of the screen to put the hud box
	// ignoreAnchorPointForPosition(false);
	// background->setAnchorPoint(Vec2(0.5, 0.5));
	// background->setPosition(Vec2(0.5, 1));
	background->setPosition(Point(visibleSize.width*(0.28) + origin.x, visibleSize.height*(0.15) + origin.y));
	this->addChild(background);
	//CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_Default);

	// Create 4 turret images for the hud box and add them to the stack
	Vector<String*> images;
	images.pushBack(StringMake("GunStill.png"));
	images.pushBack(StringMake("GunStill.png"));
	images.pushBack(StringMake("GunStill.png"));
	images.pushBack(StringMake("GunStill.png"));

	CCLabelTTF* ttf1 = CCLabelTTF::create("COINS = ", "Helvetica", 8,
		CCSizeMake(245, 32), kCCTextAlignmentCenter);


	//scoreLabel = Label::create(tempscore->getCString(), "Helvetica", 12,
	//CCSizeMake(245, 32), kCCTextAlignmentCenter);


	// For each image, increment the offset and add the image.
	for (int i = 0; i < images.size(); ++i)
	{
		String* image = images.at(i);
		auto *sprite = Sprite::create(image->getCString());
		float offsetFraction = ((float)(i + 1)) / (images.size() + 1);
		sprite->setScale(0.4);
		// sprite->setPosition(Point(visibleSize.width*(0.28) + origin.x, visibleSize.height*(0.35) + origin.y));
		sprite->setPosition(Vec2(winSize.width*(offsetFraction / 5), visibleSize.height*(0.15) + origin.y));
		// sprite->setPosition(Vec2(winSize.width*offsetFraction, winSize.height*0.8));
		sprite->setContentSize(Size(50, 50)); // MD
		this->addChild(sprite);
		movableSprites.pushBack(sprite);
	}


	ttf1->setPosition(Vec2(winSize.width - 300, visibleSize.height*(0.12) + origin.y));
	ttf1->setColor(ccc3(0, 0, 0));
	_label->setPosition(Vec2(winSize.width - 260, visibleSize.height*(0.12) + origin.y));
	//scoreLabel->setColor(ccc3(0, 0, 0));


	this->addChild(ttf1);
	this->addChild(_label);
	return true;
}

GameHUD* GameHUD::shareHUD()
{
	// If there isn't a _sharHUD make one
	if (_sharHUD == NULL)
	{
		_sharHUD = GameHUD::create();
	}

	return _sharHUD;
}

void GameHUD::onEnter()
{
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(GameHUD::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameHUD::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameHUD::onTouchEnded, this);

	auto dispatcher = Director::getInstance()->getEventDispatcher();

	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//dispatcher->addEventListenerWithFixedPriority(listener, 0);

}

bool GameHUD::onTouchBegan(Touch *touch, Event *event)
{
	Point touchLocation = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));

	// Should this star be to a side???
	Sprite *newSprite = NULL;
	// for each(Sprite* sprite in this->movableSprites) // MD 
	for (Sprite *sprite : movableSprites)
		//for (int i = 0; i < movableSprites.size(); i++)  // Use this if your VC doesn’t support C++11
	{
		// Sprite* sprite = (Sprite*)(movableSprites.at(i));  // Use this if your VC doesn’t support C++11

		Rect pos_rect = Rect((sprite->getPositionX() - sprite->getContentSize().width / 2), (sprite->getPositionY() - sprite->getContentSize().height / 2), sprite->getContentSize().width, sprite->getContentSize().height);
		// This determines the area which you can drag our ‘tower’
		float xMin = pos_rect.getMinX();
		float xMax = pos_rect.getMaxX();
		float yMIn = pos_rect.getMinY();
		float yMax = pos_rect.getMaxY();
		if (pos_rect.containsPoint(touchLocation))
		{
			DataModel *m = DataModel::getModel();
			//m.gestureRecognizer.enabled = NO;
			selSpriteRange = Sprite::create("Range.png");
			selSpriteRange->setScale(0.4);
			// this->addChild(selSpriteRange, -1);
			this->addChild(selSpriteRange, -1);
			selSpriteRange->setPosition(sprite->getPosition());

			newSprite = Sprite::createWithTexture(sprite->getTexture()); //sprite;
			newSprite->setPosition(sprite->getPosition());
			newSprite->setScale(0.2);
			selSprite = newSprite;
			this->addChild(newSprite);
		}
	}

	return true;
}

void GameHUD::onTouchMoved(Touch* touch, Event* event)
{
	Point touchLocation = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));

	Point oldTouchLocation = touch->getPreviousLocationInView();
	oldTouchLocation = Director::getInstance()->convertToGL(oldTouchLocation);
	oldTouchLocation = this->convertToNodeSpace(oldTouchLocation);

	Point translation = ccpSub(touchLocation, oldTouchLocation);

	if (selSprite)
	{
		Point newPos = selSprite->getPosition() + translation;
		selSprite->setPosition(newPos);
		selSpriteRange->setPosition(newPos);

		DataModel *m = DataModel::getModel();
		// Error here
		Point touchLocationInGameLayer = m->_gameLayer->convertTouchToNodeSpace(touch);

		BOOL isBuildable = m->_gameLayer->canBuildOnTilePosition(touchLocationInGameLayer);
		if (isBuildable)
		{
			selSprite->setOpacity(200);
		}
		else
		{
			selSprite->setOpacity(50);
		}
	}

	CCLOG("%s", "Reaching end of onTouchMoved");
}

void GameHUD::onTouchEnded(Touch* touch, Event* event)
{
	Point touchLocation = this->convertTouchToNodeSpace(touch);
	DataModel *m = DataModel::getModel();

	if (selSprite)
	{
		Rect backgroundRect = Rect(background->getPositionX(),
			background->getPositionY(),
			background->getContentSize().width,
			background->getContentSize().height);

		if (!backgroundRect.containsPoint(touchLocation) && m->_gameLayer->canBuildOnTilePosition(touchLocation))
		{
			Point touchLocationInGameLayer = m->_gameLayer->convertTouchToNodeSpace(touch);
			m->_gameLayer->addTower(touchLocationInGameLayer);
		}

		this->removeChild(selSprite, true);
		selSprite = NULL;
		this->removeChild(selSpriteRange, true);
		selSpriteRange = NULL;
	}

}

void GameHUD::numCollectedChanged(int numCollected)
{
	CCString *labelCollected = new CCString();
	labelCollected->initWithFormat("%d", numCollected);
	_label->setString(labelCollected->getCString());
}