// #include "base/CCValue.h"
#include "Level1Scene.h"
#include "Tower.h" // We will deal with it later.
#include "DataModel.h"
#include <Vector>
#include <string>
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* Level1::createScene()
{
	// 'Level1Scene' is an autorelease object
	auto Level1Scene = Scene::create();

	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("short.wav", true);

	// 'layer' is an autorelease object
	auto layer = Level1::create();

	// add layer as a child to scene
	// Level1Scene->addChild(layer);

	// Add the HUD to the main game
	//auto myGameHUD = GameHUD::shareHUD();
	GameHUD *hud = new GameHUD;
	hud->init();
	// add gameHUD
	// Level1Scene->addChild(myGameHUD, 3);

	// Add the HUD to the main game
	// auto myGameHUD = GameHUD::shareHUD();

	Level1Scene->addChild(layer);
	Level1Scene->addChild(hud);

	DataModel *m = DataModel::getModel();
	m->_gameLayer = layer; // add this
	layer->_hud = hud;



	// return the scene
	return Level1Scene;
}

Level1::~Level1()
{
	// turretBases->release();
}

// on "init" you need to initialize your instance
bool Level1::init()
{
	/*
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch *touch, Event *unused_event)->bool {return true; };
	listener->onTouchEnded = CC_CALLBACK_2(Level1::onTouchEnded, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);*/

	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Add the TMX map
	_tileMap = new CCTMXTiledMap();
	_tileMap->initWithTMXFile("md_roomy_map1.tmx");
	// set position to middle of the screen
	// _tileMap->setPosition(Point(visibleSize.width*(0.2) + origin.x, visibleSize.height*(0.2) + origin.y));
	// set size of tilemap
	// _tileMap->setScale(visibleSize.width / 220 , visibleSize.height / 220);
	// Add the background
	_background = _tileMap->layerNamed("Background");
	_Sources = _tileMap->layerNamed("Sources");
	_Quicksand = _tileMap->layerNamed("Quicksand");
	// _buildable = _tileMap->layerNamed("buildable");

	// this->addChild(_tileMap, -1);
	// Add the walls
	// _walls = _tileMap->layerNamed("Walls");

	// this->addChild(_tileMap, 0);

	// Add the turret bases
	// _turrets = _tileMap->layerNamed("Turrets");

	this->addChild(_tileMap, -1);


	//////////////////////////////
	// 2. Make Enemy Spawn 
	TMXObjectGroup *objects = _tileMap->getObjectGroup("Enemy");
	CCASSERT(NULL != objects, "'Enemy' enemy spawn point not found.");
	auto enemySpawnPoint = objects->getObject("EnemySpawnPoint");
	CCASSERT(!enemySpawnPoint.empty(), "EnemySpawnPoint object not found.");

	int x = enemySpawnPoint["x"].asInt();
	int y = enemySpawnPoint["y"].asInt();

	// _enemyUnit1 = Sprite::create("Enemy1.png");
	// _enemyUnit1->setPosition(x + _tileMap->getTileSize().width /2, y + _tileMap->getTileSize().height / 2);
	// _enemyUnit1->setPosition(x,y);
	// _enemyUnit1->setScale(0.2);
	// addChild(_enemyUnit1);
	// setViewPointCenter(_enemyUnit1->getPosition());


	/////////////////////////////
	// 3. add turrets positions to the map so they can
	//    be added by the player later



	/////////////////////////////
	// 4. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(Level1::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);


	this->addWayPoint();
	this->addWaves();
	this->scheduleUpdate();
	this->schedule(schedule_selector(Level1::gameLogic), 1.0f);
	this->currentLevel = 0;

	// setViewPointCenter(_tileMap->getPositionX);

	return true;
}

void Level1::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void Level1::setViewPointCenter(Point position) {
	auto winSize = Director::getInstance()->getWinSize();

	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height / 2);
	auto actualPosition = Point(x, y);

	auto centerOfView = Point(winSize.width*0.6, winSize.height*0.75);
	auto viewPoint = centerOfView - actualPosition;
	this->setPosition(viewPoint);
}


void Level1::FollowPath(Node *sender)
{
	Creep *creep = (Creep *)sender;

	WayPoint *waypoint = creep->getNextWaypoint();

	int moveDuration = creep->moveDuration;
	auto actionMove = MoveTo::create(moveDuration, waypoint->getPosition());
	auto actionMoveDone = CallFuncN::create(this, callfuncN_selector(Level1::FollowPath));
	creep->stopAllActions();
	creep->runAction(Sequence::create(actionMove, actionMoveDone, NULL));
}

// Here we read the value from the class ‘creep’. And we make the enemy move. 

void Level1::addWaves()
{
	DataModel *m = DataModel::getModel();

	Wave *wave = NULL;

	wave = Wave::create()->initWithCreep(FastRedCreep::creep(), 0.7, 75);
	m->waves.pushBack(wave);
	wave = NULL;

	wave = Wave::create()->initWithCreep(StrongGreenCreep::creep(), 2.0, 10);
	m->waves.pushBack(wave);
	wave = NULL;
}

// Here, we set the parameters for the class ‘Wave’ about the creep type, spawn rate and the number of the creep.

void Level1::addWayPoint()
{
	DataModel *m = DataModel::getModel();
	auto *objects = this->_tileMap->objectGroupNamed("WayPointList");
	WayPoint *wp = NULL;

	std::string stringWithFormat = "Waypoint";
	int wayPointCounter = 0;
	ValueMap wayPoint;
	wayPoint = objects->objectNamed(stringWithFormat + std::to_string(wayPointCounter));

	while (wayPoint.begin() != wayPoint.end())
	{
		int x = wayPoint.at("x").asInt();
		int y = wayPoint.at("y").asInt();
		wp = WayPoint::create();
		wp->setPosition(Vec2(x, y));
		m->waypoints.pushBack(wp);
		wayPointCounter++;
		wayPoint = objects->objectNamed(stringWithFormat + std::to_string(wayPointCounter));
	}
	wp = NULL;
}

void Level1::addTarget()
{
	DataModel *m = DataModel::getModel();
	Wave *wave = this->getCurrentWave();

	if (wave->totalCreeps < 0)
	{
		return;
	}
	wave->totalCreeps--;

	Creep *target = NULL;
	int random = CCRANDOM_0_1() * 2;
	if (random == 0)
	{
		target = FastRedCreep::creep();
	}
	else
	{
		target = StrongGreenCreep::creep();
	}

	WayPoint *waypoint = target->getCurrentWaypoint();
	target->setPosition(waypoint->getPosition());
	waypoint = target->getNextWaypoint();
	this->addChild(target, 1);

	int moveDuration = target->moveDuration;
	auto actionMove = CCMoveTo::create(moveDuration, waypoint->getPosition());
	auto actionMoveDone = CallFuncN::create(this, callfuncN_selector(Level1::FollowPath));

	target->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));
	target->tag = 1;
	m->targets.pushBack(target);

}

void Level1::gameLogic(float dt)
{
	DataModel *m = DataModel::getModel();
	Wave *wave = this->getCurrentWave();
	static double lastTimeTargetAdded = 0;

	double now = 0;
	if (lastTimeTargetAdded == 0 || now - lastTimeTargetAdded >= wave->spawnRate)
	{
		this->addTarget();
		lastTimeTargetAdded = now;
	}
}

Wave *Level1::getCurrentWave()
{
	DataModel *m = DataModel::getModel();
	Wave *wave = (Wave *)m->waves.at(this->currentLevel);

	return wave;
}

Wave *Level1::getNextWave()
{
	DataModel *m = DataModel::getModel();

	this->currentLevel++;
	// why does this revert to level 0?
	if (this->currentLevel > 1)
	{
		this->currentLevel = 0;
	}

	Wave *wave = (Wave *)m->waves.at(this->currentLevel);

	return wave;
}

Point Level1::tileCoordForPosition(Point position)
{
	int x = position.x / this->_tileMap->getTileSize().width;
	int y = ((this->_tileMap->getMapSize().height * this->_tileMap->getTileSize().height) - position.y) / this->_tileMap->getTileSize().height;
	return Point(x, y);
}

bool Level1::canBuildOnTilePosition(Point pos)
{
	// There is a problem here.... 
	Point towerLoc = this->tileCoordForPosition(pos);
	// int tileGid = this->_background->getTileGIDAt(towerLoc);
	// and here.... 
	int tileGid = this->_background->getTileGIDAt(towerLoc);
	Value props = this->_tileMap->getPropertiesForGID(tileGid);

	// MD
	if (props.isNull())   // NULL check
	{
		return true;
	}
	ValueMap map = props.asValueMap();
	bool buildable = true;
	if (map.size() != 0)
	{

		buildable = map.at("buildable").asBool();
	}


	return buildable;
}

/*
bool Level1::canBuildOnTilePosition(Point pos)
{
Point towerLoc = this->tileCoordForPosition(pos);
int tileGid = _background->getTileGIDAt(towerLoc);
Value props = this->_tileMap->getPropertiesForGID(tileGid);
if (props.isNull()) {
return false;
}
ValueMap map = props.asValueMap();
int type_int;
if (map.size() == 0)
{
type_int = 0;
}
else
{
type_int = map.at("buildable").asInt();
}

if (type_int == 1)
{
return true;
}
return false;
}
*/

/*
void Level1::addTower(Point pos)
{
DataModel *m = DataModel::getModel();

Tower *target = NULL;
// WORKING
Point towerLoc = this->tileCoordForPosition(pos);
int tileGid = this->_background->tileGIDAt(towerLoc);
Value props = this->_tileMap->propertiesForGID(tileGid);
ValueMap map = props.asValueMap();
// bool buildable = canBuildOnTilePosition(pos);

CCLOG("Preparing to add tower to tile", "%s");



int type_int = map.at("buildable").asInt();
// int type_int = map.at("Turrets").asInt();
if (type_int == 1)
{
// Problem here....
target = MachineGunTower::tower();
// target->setPosition(Vec2((towerLoc.x * 32) + 16, this->_tileMap->getContentSize().height - (towerLoc.y * 32) - 16));
target->setPosition(Vec2((towerLoc.x * 20) + 10, this->_tileMap->getContentSize().height - (towerLoc.y * 20) - 10));
this->addChild(target, 1);
target->setTag(1);
m->towers.pushBack(target);
}
else
{
log("Tile Not Buildable");
}
}
*/

void Level1::addTower(Point pos)
{
	DataModel *m = DataModel::getModel();

	//_numCollected = 5;
	Tower *target = NULL;
	Point towerLoc = this->tileCoordForPosition(pos);
	bool buildable = canBuildOnTilePosition(pos);
	if ((buildable && _numCollected >= 5))  {
		_numCollected = _numCollected - 5;
		_hud->numCollectedChanged(_numCollected);
		Point towerLoc = this->tileCoordForPosition(pos);

		target = MachineGunTower::tower();
		target->setPosition(Vec2((towerLoc.x * 20) + 10, this->_tileMap->getContentSize().height - (towerLoc.y * 20) + 150));
		this->addChild(target, 1);
		target->setTag(1);
		m->towers.pushBack(target);

	}
	else
	{
		log("Tile Not Buildable");
	}
	/*int tileGid = this->_background->tileGIDAt(towerLoc);
	Value props = this->_tileMap->propertiesForGID(tileGid);
	ValueMap map = props.asValueMap();

	int type_int = map.at("buildable").asInt();
	// int type_int = map.at("Turrets").asInt();
	if (1 == type_int)
	{
	// Problem here....
	target = MachineGunTower::tower();
	// target->setPosition(Vec2((towerLoc.x * 32) + 16, this->_tileMap->getContentSize().height - (towerLoc.y * 32) - 16));
	target->setPosition(Vec2((towerLoc.x * 20) + 10, this->_tileMap->getContentSize().height - (towerLoc.y * 20) - 10));
	this->addChild(target, 1);
	target->setTag(1);
	m->towers.pushBack(target);
	}
	else
	{
	log("Tile Not Buildable");
	}*/
}

Point Level1::boundLayerPos(Point newPos)
{
	Size winSize = CCDirector::getInstance()->getWinSize();
	Point retval = newPos;
	retval.x = MIN(retval.x, 0);
	retval.x = MAX(retval.x, _tileMap->getContentSize().width + winSize.width);
	retval.y = MIN(0, retval.y);
	retval.y = MAX(_tileMap->getContentSize().height + winSize.height, retval.y);
	return retval;
}

void Level1::update(float dt) {
	DataModel *m = DataModel::getModel();
	Vector<Projectile*> projectilesToDelete;
	//Level1 *l = ;

	int c = 0;

	for each(Projectile *projectile in m->projectiles)
		// for (int i = 0; i < m->projectiles.size(); i++) // Use these code if your VC doesn’t support C++11.
	{
		// Projectile* projectile = (Projectile*)(m->projectiles.at(i)); // Use these code if your VC doesn’t support C++11.
		Rect projectileRect = Rect(projectile->getPositionX() - (projectile->getContentSize().width / 2),
			projectile->getPositionY() - (projectile->getContentSize().height / 2),
			projectile->getContentSize().width,
			projectile->getContentSize().height);

		Vector<Creep*> targetsToDelete;

		for each(Creep *target in m->targets)
			// for (int i = 0; i < m->targets.size(); i++) // Use these code if your VC doesn’t support C++11.
		{
			// Creep* target = (Creep*)(m->targets.at(i)); // Use these code if your VC doesn’t support C++11.

			Rect targetRect = Rect(target->getPositionX() - (target->sprite->getContentSize().width / 2),
				target->getPositionY() - (target->sprite->getContentSize().height / 2),
				target->sprite->getContentSize().width,
				target->sprite->getContentSize().height);

			if (projectileRect.intersectsRect(targetRect))
			{
				projectilesToDelete.pushBack(projectile);

				Creep *creep = target;
				creep->curHp -= 1;

				if (creep->curHp <= 0)
				{
					targetsToDelete.pushBack(creep);
					//GameHUD* score = (GameHUD*)(c);

					//m->_score=score;
				}
				break;
			}
		}

		//for each(Creep *target in targetsToDelete)

		for (int i = 0; i < targetsToDelete.size(); i++)
		{
			Creep* target = (Creep*)(targetsToDelete.at(i));


			m->targets.eraseObject(target);
			this->removeChild(target, true);
			//count++;
			_numCollected++;
			_hud->numCollectedChanged(_numCollected);

			//CCLOG("COIN = %i", count);
		}
	}

	for each(Projectile *projectile in projectilesToDelete)
		// for (int i =0; i < projectilesToDelete.size(); i++) // Use these code if your VC doesn’t support C++11.
	{
		// Projectile* projectile = (Projectile*)(projectilesToDelete.at(i)); // Use these code if your VC doesn’t support C++11.

		m->projectiles.eraseObject(projectile);
		this->removeChild(projectile, true);
	}
}

//  #endif
