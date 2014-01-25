#include "LayerMediator.h"
#include "PanelLayer.h"
#include "MapLayer.h"

MediatorEvent* MediatorEvent::createSelf() {
	MediatorEvent* self = new MediatorEvent;
	if(self && self->initSelf()) {
		self->autorelease();
		CCLog("MediatorEvent %d", self->retainCount());
		return self;
	}
	CC_SAFE_DELETE(self);
	return NULL;
}

void LayerMediator::sendMapEvent(MediatorEvent* mediatorEvent) {
	_panelLayer->handleEvent(mediatorEvent);
}

void LayerMediator::sendPanelEvent(MediatorEvent* mediatorEvent) {
	_mapLayer->handleEvent(mediatorEvent);
}

