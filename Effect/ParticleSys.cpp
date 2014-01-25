#include "ParticleSys.h"
#include "Map.h"

void ParticleSys::addParticlesToMap() {
	_map->getTiledMap()->addChild(_jet, _map->getHighestZorder());
	_map->getTiledMap()->addChild(_boom, _map->getHighestZorder());
	_map->getTiledMap()->addChild(_comet, _map->getHighestZorder());
	_map->getTiledMap()->addChild(_pickup, _map->getHighestZorder());
	_map->getTiledMap()->addChild(_warp, _map->getHighestZorder());
	_map->getTiledMap()->addChild(_star, _map->getHighestZorder());
}

void ParticleSys::createParticles() {
	_jet = CCParticleSystemQuad::create("particle/jet.plist");
	_jet->setAngle(0); // setAngle是角度不是弧度,表示喷射的角度,0度表示x轴正向,90度表示y轴正向
	_jet->stopSystem();
	_jet->setVisible(true);
	_jet->retain();
    
	_boom = CCParticleSystemQuad::create("particle/boom.plist");
	_boom->stopSystem();
	//_boom->setSourcePosition(ccp(100, 0));
	// !!! setSourcePosition表示粒子发射点和getPosition的偏移
	// 一般必须设成ccp(0, 0)
	_boom->setVisible(false);
	_boom->retain();
    
	_comet = CCParticleSystemQuad::create("particle/comet.plist");
	_comet->stopSystem();
	_comet->setVisible(false);
	_comet->retain();
    
	_pickup = CCParticleSystemQuad::create("particle/plink.plist");
	_pickup->stopSystem();
	_pickup->setVisible(false);
	_pickup->retain();
    
	_warp = CCParticleSystemQuad::create("particle/warp.plist");
	_warp->stopSystem();
	_warp->setVisible(false);
	_warp->retain();
    
	_star = CCParticleSystemQuad::create("particle/star.plist");
	_star->stopSystem();
	_star->setVisible(false);
	_star->retain();
}