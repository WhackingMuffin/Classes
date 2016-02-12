#ifndef  _CREEP_H_
#define  _CREEP_H_

#include "cocos2d.h"
#include "WayPoint.h"

USING_NS_CC;

class Creep : public Sprite
{
public:
	int curHp;
	int moveDuration;
	int curWaypoint;
	int tag;
	Sprite* sprite;
	virtual bool init();
	Creep* initWithCreep(Creep* copyFrom);
	WayPoint* getNextWaypoint();
	WayPoint* getCurrentWaypoint();
	CREATE_FUNC(Creep);
};

class FastRedCreep : public Creep
{
public:
	static Creep* creep();
};

class StrongGreenCreep : public Creep
{
public:
	static Creep* creep();
};

#endif // _CREEP_H_