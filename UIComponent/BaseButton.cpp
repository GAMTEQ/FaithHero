#include "BaseButton.h"

void SkillButton::singleTouch(const CCPoint& pos) {
    CCLog("SkillButton::singleTouch %d", _skillId);
}

void SkillButton::cold() {
    _isCold = true;
    CCSequence* coldAction = CCSequence::createWithTwoActions(CCFadeIn::create(12.0f),
                                                              CCCallFuncN::create(this, callfuncN_selector(SkillButton::coldEnd)));
    this->runAction(coldAction);
}

void SkillButton::coldEnd() {
    _isCold = false;
}
/*
void SkillButton::sendEvent(float deltaLife, float deltaMage) {
	EventDecreaseLifeAndMage* newEvent = EventDecreaseLifeAndMage::createSelf(deltaLife, deltaMage);
	_mediator->sendMapEvent(newEvent);
}
*/

