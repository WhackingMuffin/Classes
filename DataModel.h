#ifndef  _DATAMODEL_H_
#define  _DATAMODEL_H_

#include "cocos2d.h"
// #include <Vector>
#include "base/CCVector.h"
#include "WayPoint.h"
#include "Creep.h"
#include "Level1Scene.h"
// #include "GameHUD.h" // Maybe comment out?
#include "Tower.h" 
#include "Projectile.h"
// #include "Wave.h"

USING_NS_CC;

class DataModel
{
public:
	// The data model will take responsibiliy of keeping track of the 
	// gamelayer,waypoints, targets, waves, towers, projectiles and game HUD layer
	Level1* _gameLayer;
	static DataModel* getModel();
	Vector<WayPoint*> waypoints;
	Vector<Creep*> targets;
	Vector<Wave*> waves;
	Vector<Tower*> towers;    // We will deal with it later.
	Vector<Projectile*> projectiles;  // We will deal with it later.
	GameHUD* _gameHUDLayer;  // We will deal with it later.

private:
	DataModel() {};
	static DataModel * m_pInstance;
};

#endif // _DATAMODEL_H_


