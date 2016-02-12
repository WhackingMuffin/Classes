#include "Creep.h"
#include "WayPoint.h"
#include "DataModel.h"

USING_NS_CC;

bool Creep::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	return true;
}

Creep* Creep::initWithCreep(Creep* copyFrom)
{
	return NULL;
}

Creep* FastRedCreep::creep()
{
	auto creep = Creep::create();
	creep->sprite = Sprite::create("invisible.png");

	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(8);
	animFrames.pushBack(SpriteFrame::create("EnSoldier03.png", Rect(0, 0, 65, 81)));
	animFrames.pushBack(SpriteFrame::create("EnSoldier02.png", Rect(0, 0, 65, 81)));
	animFrames.pushBack(SpriteFrame::create("EnSoldier01.png", Rect(0, 0, 65, 81)));
	animFrames.pushBack(SpriteFrame::create("EnSoldier02.png", Rect(0, 0, 65, 81)));
	animFrames.pushBack(SpriteFrame::create("EnSoldier03.png", Rect(0, 0, 65, 81)));
	animFrames.pushBack(SpriteFrame::create("EnSoldier04.png", Rect(0, 0, 65, 81)));
	animFrames.pushBack(SpriteFrame::create("EnSoldier03.png", Rect(0, 0, 65, 81)));
	animFrames.pushBack(SpriteFrame::create("EnSoldier02.png", Rect(0, 0, 65, 81)));

	// create the animation out of the frames
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	Animate* animate = Animate::create(animation);

	// run it and repeat it forever
	creep->runAction(RepeatForever::create(animate));
	//rotate
	auto rotateTo = RotateTo::create(2.0f, 40.0f);

	creep->runAction(rotateTo);

	// Rotates a Node clockwise by 40 degree over 2 seconds
	auto rotateBy = RotateBy::create(2.0f, 40.0f);
	creep->runAction(rotateBy);
	creep->setScale(0.3);
	creep->addChild(creep->sprite, 0);
	creep->curHp = 10;
	creep->moveDuration = 3;
	// The curWaypoint may need to be changed depending on the tmx points
	creep->curWaypoint = 0;
	return creep;
}

Creep* StrongGreenCreep::creep()
{
	auto creep = Creep::create();
	creep->sprite = Sprite::create("invisible2.png");
	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(12);
	animFrames.pushBack(SpriteFrame::create("AnTank.png", Rect(0, 0, 65, 81)));
	animFrames.pushBack(SpriteFrame::create("AnTank2.png", Rect(0, 0, 65, 81)));
	animFrames.pushBack(SpriteFrame::create("AnTank3.png", Rect(0, 0, 65, 81)));
	animFrames.pushBack(SpriteFrame::create("AnTank4.png", Rect(0, 0, 65, 81)));
	animFrames.pushBack(SpriteFrame::create("AnTank3.png", Rect(0, 0, 65, 81)));
	animFrames.pushBack(SpriteFrame::create("AnTank2.png", Rect(0, 0, 65, 81)));
	animFrames.pushBack(SpriteFrame::create("AnTank.png", Rect(0, 0, 65, 81)));
	animFrames.pushBack(SpriteFrame::create("AnTank5.png", Rect(0, 0, 65, 81)));
	animFrames.pushBack(SpriteFrame::create("AnTank6.png", Rect(0, 0, 65, 81)));
	animFrames.pushBack(SpriteFrame::create("AnTank7.png", Rect(0, 0, 65, 81)));
	animFrames.pushBack(SpriteFrame::create("AnTank6.png", Rect(0, 0, 65, 81)));
	animFrames.pushBack(SpriteFrame::create("AnTank5.png", Rect(0, 0, 65, 81)));
	// create the animation out of the frames
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	Animate* animate = Animate::create(animation);
	// run it and repeat it forever
	creep->runAction(RepeatForever::create(animate));
	creep->setScale(0.3);
	creep->addChild(creep->sprite, 0);
	creep->curHp = 30;
	creep->moveDuration = 8;
	// The curWaypoint may need to be changed depending on the tmx points
	creep->curWaypoint = 0;
	return creep;
}

WayPoint* Creep::getCurrentWaypoint()
{
	// Make use of the DataModel to get the next waypoint
	DataModel* m = DataModel::getModel();
	WayPoint* waypoint = (WayPoint *)m->waypoints.at(this->curWaypoint);
	return waypoint;
}

WayPoint* Creep::getNextWaypoint()
{
	DataModel* m = DataModel::getModel();

	// Increment the waypoint by 1 if it hasn't reached the last checkpoint
	if (this->curWaypoint != 11)
	{
		this->curWaypoint++;
	}

	else
	{
		// If we wanted to loop the waypoint movement we would uncommment this
		// this->curWaypoint = 0;
	}


	CCLOG("%d", this->curWaypoint); // For testing.

	WayPoint *waypoint = (WayPoint *)m->waypoints.at(curWaypoint);
	return waypoint;
}