#ifndef _USER_RECORD_DEFAULT__H_
#define _USER_RECORD_DEFAULT__H_

#include "Base.h"

class UserRecordDefault : public CCObject {
public:
	UserRecordDefault()
		: _userId("John")
		, _coin(100)
		, _exp(20)
	{
	}
	void saveToCCUserDefault() {
		char buff[100];
		sprintf(buff, "%d %d", this->getCoin(), this->getExp());
		string keyString = "UserRecord." + this->getUserId();
		const char* key = keyString.c_str();
		CCLog("key: %s %s", keyString.c_str(), key);
		CCUserDefault::sharedUserDefault()->setStringForKey(key, buff);
	}
	void loadFromCCUserDefault() {
		string keyString = "UserRecord." + this->getUserId();
		CCLog("keyString load %s", keyString.c_str());
		string buff = CCUserDefault::sharedUserDefault()->getStringForKey(keyString.c_str());
		CCLog("buff %s", buff.c_str());
		int coin = 0;  
		int exp = 0;
		sscanf(buff.c_str(), "%d %d", &coin, &exp);
		this->setCoin(coin);
		this->setExp(exp);
	}
protected:
	CC_SYNTHESIZE_PASS_BY_REF(string, _userId, UserId);
	CC_SYNTHESIZE_PASS_BY_REF(int, _coin, Coin);
	CC_SYNTHESIZE_PASS_BY_REF(int, _exp, Exp);
};

#endif // _USER_RECORD_DEFAULT__H_