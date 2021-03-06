#include "ArrowFireBomb.h"
#include "Map.h"
#include "PathFinder.h"
#include "ParticleSys.h"

ArrowFireBomb::~ArrowFireBomb() {
    CCLog("~ArrowFireBomb");
    CC_SAFE_RELEASE_NULL(_jet);
}

bool ArrowFireBomb::initSelf(Map* map, const CCPoint& startPos, PathFinder* targetSprite) {
	if(Arrow::initSelf(map, startPos, targetSprite)) {
		_jet = CCParticleSystemQuad::create("particle/jet.plist");
		_jet->retain();
		this->addChild(_jet);
		_jet->setSourcePosition(ccp(0, 0));
		_jet->setPosition(ccp(0, 0));
		_jet->setAngle(0);
		_jet->setVisible(true);
        _attack = 30;
		return true;
	}
	return false;
}

void ArrowFireBomb::rotateToVector(CCNode* node, const CCPoint& vec) {
	Arrow::rotateToVector(node, vec);
	_jet->setAngle(atan2f(vec.y, vec.x) * 180 / 3.1416 + 180); 
	_jet->setRotation(atan2f(vec.y, vec.x) * 180 / 3.1416);
}

void ArrowFireBomb::initDisplay() {
}

void ArrowFireBomb::reachTarget() {
	Arrow::reachTarget();
	CCParticleSystem* particleBoom = _map->getParticlaSys()->getBoom();
	particleBoom->setPosition(this->getPosition());
	particleBoom->setVisible(true);
	particleBoom->resetSystem();
}



