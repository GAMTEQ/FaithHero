#ifndef _PARTICLE_SYS__H_
#define _PARTICLE_SYS__H_

#include "Base.h"
#include "SpriteBase.h"

class Map;

class ParticleSys : public CCObject {
// 构造
public:
	static ParticleSys* createSelf(Map* map) {
		ParticleSys* self = new ParticleSys;
		if(self && self->initSelf(map)) {
			self->autorelease();
			return self;
		}
		CC_SAFE_DELETE(self);
		return NULL;
	}
	~ParticleSys() {
		CC_SAFE_RELEASE_NULL(_star);
		CC_SAFE_RELEASE_NULL(_jet);
		CC_SAFE_RELEASE_NULL(_boom);
		CC_SAFE_RELEASE_NULL(_comet);
		CC_SAFE_RELEASE_NULL(_pickup);
		CC_SAFE_RELEASE_NULL(_warp);
	}
protected:
	bool initSelf(Map* map) {
		_map = map;
		createParticles();
		return true;
	}
	void createParticles();
// 接口
public:
	void addParticlesToMap(); // 必须再map reset之后才能调用
// 属性
protected:
	CC_SYNTHESIZE(CCParticleSystem*, _star, Star);
	CC_SYNTHESIZE(CCParticleSystem*, _jet, Jet);
	CC_SYNTHESIZE(CCParticleSystem*, _boom, Boom);
	CC_SYNTHESIZE(CCParticleSystem*, _comet, Comet);
	CC_SYNTHESIZE(CCParticleSystem*, _pickup, Pickup);
	CC_SYNTHESIZE(CCParticleSystem*, _warp, Warp);
	Map* _map;
};

#endif // _PARTICLE_SYS__H_
