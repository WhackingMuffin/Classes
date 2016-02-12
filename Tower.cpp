#include "Tower.h"
#include "DataModel.h"
#include "SimpleAudioEngine.h"

Tower* MachineGunTower::tower()
{
	Tower* tower = Tower::create();
	tower->sprite = Sprite::create("GunStill.png");
	tower->setScale(0.5);
	tower->addChild(tower->sprite, 0);
	tower->range = 100;
	tower->schedule(schedule_selector(towerLogic), 0.2);
	return tower;
}

bool MachineGunTower::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	return true;
}

void MachineGunTower::towerLogic(float dt) {

	this->target = this->getClosestTarget();

	if (this->target != NULL)
	{
		Point shootVector = this->target->getPosition() - this->getPosition();
		float shootAngle = ccpToAngle(shootVector); // does this work seen as i changed ccps to Vec2?
		float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * shootAngle);

		float rotateSpeed = 0.5 / M_PI;
		float rotateDuration = fabs(shootAngle * rotateSpeed);

		this->runAction(Sequence::create(RotateTo::create(rotateDuration, cocosAngle),
		CallFunc::create(this, callfunc_selector(MachineGunTower::finishFiring)), NULL)); 
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(
			"Explosion.wav");
	}
}

Creep* Tower::getClosestTarget()
{
	Creep *closestCreep = NULL;
	double maxDistant = 9999; // It should be bigger than the range.
	DataModel *m = DataModel::getModel();

	for each(Sprite *target in m->targets)
		// for(Vector<Creep*>::iterator i = m->targets.begin(); i < m->targets.end(); ++i) // Use these code if your VC doesn’t support C++11.
	{
		// Sprite *target = *i;  // Use these code if your VC doesn’t support C++11.

		Creep *creep = (Creep *)target;
		double curDistance = ccpDistance(this->getPosition(), creep->getPosition());

		if (curDistance < maxDistant) {
			closestCreep = creep;
			maxDistant = curDistance;
		}
	}
	if (maxDistant < this->range)
		return closestCreep;
	return NULL;
}

void MachineGunTower::finishFiring()
{
	DataModel *m = DataModel::getModel();
	if (this->target != NULL && this->target->curHp > 0 && this->target->curHp < 100)
	{
		this->nextProjectile = Projectile::projectile();
		this->nextProjectile->setPosition(this->getPosition());

		this->getParent()->addChild(this->nextProjectile, 1);
		m->projectiles.pushBack(this->nextProjectile);

		float delta = 1.0f;
		Point shootVector = -(this->target->getPosition() - this->getPosition());
		Point normalizedShootVector = ccpNormalize(shootVector);
		Point overshotVector = normalizedShootVector * 320;
		Point offscreenPoint = (this->getPosition() - overshotVector);

		this->nextProjectile->runAction(Sequence::create(MoveTo::create(delta, offscreenPoint), 
		CallFuncN::create(this, callfuncN_selector(MachineGunTower::creepMoveFinished)), NULL));
		this->nextProjectile->setTag(2);
		this->nextProjectile = NULL;

	}
}

void MachineGunTower::creepMoveFinished(Node* sender)
{
	DataModel * m = DataModel::getModel();
	Sprite *sprite = (Sprite *)sender;
	this->getParent()->removeChild(sprite, true);

	m->projectiles.eraseObject((Projectile*)sprite);
}