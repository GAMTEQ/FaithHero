#include "RolePanel.h"
#include "StatusBar.h"
#include "BaseButton.h"
#include "NameBoard.h"

bool RolePanel::initSelf(PanelLayer* panelLayer) {
	if(CCSprite::init()) {
		this->initWithSpriteFrameName("role_panel2.png");
		this->setAnchorPoint(ccp(0, 0));
		// 比例 常量
		_portraitRatioPos = ccp(0.1, 0.2);
		_magebarRatioPos = ccp(0.333, 0.2);
		_lifebarRatioPos = ccp(0.333, 0.41);
		_nameboardRatioPos = ccp(0.333, 0.62);
		// 组件位置
		_panelSize = this->getContentSize();
		_portraitPos = ccp(_panelSize.width * _portraitRatioPos.x, _panelSize.height * _portraitRatioPos.y);
		_lifebarPos = ccp(_panelSize.width * _lifebarRatioPos.x, _panelSize.height * _lifebarRatioPos.y);
		_magebarPos = ccp(_panelSize.width * _magebarRatioPos.x, _panelSize.height * _magebarRatioPos.y);
		_nameboardPos = ccp(_panelSize.width * _nameboardRatioPos.x, _panelSize.height * _nameboardRatioPos.y);
		CCLog("por %f %f life %f %f mage %f %f bord %f %f", 
			_portraitPos.x, _portraitPos.y, _lifebarPos.x, _lifebarPos.y,
			_magebarPos.x, _magebarPos.y, _nameboardPos.x, _nameboardPos.y);
		// 组件
		_nameboard = NameBoard::createSelf("nameboard1.png", "Antonidas", 35);
		_nameboard->setAnchorPoint(ccp(0, 0));
		_nameboard->setPosition(_nameboardPos);
		this->addChild(_nameboard);
		_portrait = BaseButton::createSelf(panelLayer, "portrait.png");
		_portrait->setAnchorPoint(ccp(0, 0));
		_portrait->setPosition(_portraitPos);
		this->addChild(_portrait);
		_lifebar = StatusBar::createSelf("lifebar.png", ccc4f(0.0f, 1.0f, 0.0f, 1 ));
		_lifebar->setAnchorPoint(ccp(0, 0));
		_lifebar->setPosition(_lifebarPos);
		this->addChild(_lifebar);
		_magebar = StatusBar::createSelf("magebarbg.png", ccc4f(0.0f, 0.0f, 1.0f, 1 ));
		_magebar->setAnchorPoint(ccp(0, 0));
		_magebar->setPosition(_magebarPos);
		this->addChild(_magebar);
		return true;
	}
	return false;
}

void RolePanel::updateLifeAndMage(float life, float mage) {
	CCLog("RolePanel::updateLifeAndMage %f %f", life, mage);
	_lifebar->setValue(_lifebar->getValue() + life);
	_magebar->setValue(_magebar->getValue() + mage);
}

void RolePanel::setMaxLifeAndMage(float maxLife, float maxMage) {
	CCLog("RolePanel::setMaxLifeAndMage %f %f", maxLife, maxMage);
	_lifebar->setMaxValue(maxLife);
	_magebar->setMaxValue(maxMage);
}

