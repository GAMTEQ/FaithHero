#ifndef _USER_RECORD_SQLITE__H_
#define _USER_RECORD_SQLITE__H_

#include "Base.h"
#include "sqlite3.h"
//#include "sqlite3ext.h"

#pragma comment(lib, "sqlite3.lib")

// 是不是要先启动sqlite3
//extern const sqlite3_api_routines *sqlite3_api;

class UserRecordSqlite : public CCObject {
public:
	UserRecordSqlite()
		: _userId(51)
		, _coin(1)
		, _exp(2)
	{
	}
	void saveToSqlite(const char* table = "UserRecord2", const char* dbFilename = "sql.db") {
		char sql[1024];
		sqlite3* pDb = prepareTableInDb(table, dbFilename);
		if(pDb) {
			int count = 0; // 代表这个表中有几条记录
			char* errorMsg = NULL;
			sprintf(sql, "select count(*) from %s where id = %d", table, getUserId());
			CCLog("sql: %s", sql);
			sqlite3_exec(pDb, sql, loadRecordCount, &count, &errorMsg);
			if(0) {
				CCLog("exec sql %s failed with msg %s", sql, errorMsg);
				sqlite3_close(pDb);
				return;
			}
			if(count) { // 如果已有记录,则插入一条
				sprintf(sql, "update %s set coin = %d, experience = %d where id = %d",
					table, getCoin(), getExp(), getUserId());
			}
			else { // 如果已有记录,则修改
				sprintf(sql, "insert into %s values (%d, %d, %d)",
					table, getUserId(), getCoin(), getExp());
			}
			CCLog("sql: %s", sql);
			sqlite3_exec(pDb, sql, NULL, NULL, &errorMsg);
			if(errorMsg) {
				CCLog("exec sql %s failed with msg %s", sql, errorMsg);
				sqlite3_close(pDb);
				return;
			}
		}
		sqlite3_close(pDb);
	}
	void loadFromSqlite(const char* table = "UserRecord2", const char* dbFilename = "sql.db") {
		char sql[1024];
		sqlite3* pDb = prepareTableInDb(table, dbFilename);
		if(pDb) {
			char* errorMsg = NULL;
			sprintf(sql, "select * from %s where id = %d", table, getUserId());
			CCLog("sql: %s", sql);
			sqlite3_exec(pDb, sql, loadUserRecord, this, &errorMsg);
			if(errorMsg) {
				CCLog("exec sql %s failed with msg %s", sql, errorMsg);
				sqlite3_close(pDb);
				return;
			}
		}
		sqlite3_close(pDb);
	}
private:
	sqlite3* prepareTableInDb(const char* table, const char* dbFilename) {
		sqlite3* pDb = NULL;
		char* errorMsg = NULL;
		if(sqlite3_open(dbFilename, &pDb) != SQLITE_OK) {
			CCLog("open sql file failed!");
			return NULL;
		}
		string sql = "create table if not exists " + string(table) + "(id integer primary key, coin integer, experience integer)";
		CCLog("create sql: %s", sql.c_str());
		if(sqlite3_exec(pDb, sql.c_str(), NULL, NULL, &errorMsg) != NULL) {
			sqlite3_close(pDb);
			return NULL;
		}
		return pDb;
	}
	static int loadRecordCount(void* para, int column, char** columnValue, char** columnName) {
		int* pCount = (int*)para;
		sscanf(columnValue[0], "%d", pCount);
		return 0;
	}
	static int loadUserRecord(void* para, int column, char** columnValue, char** columnName) {
		UserRecordSqlite* record = (UserRecordSqlite*)para;
		int coin = 0;
		int experience = 0;
		sscanf(columnValue[1], "%d", &coin);
		sscanf(columnValue[2], "%d", &experience);
		record->setCoin(coin);
		record->setExp(experience);
		CCLog("loadUserRecord %d %d", record->getCoin(), record->getExp());
		return 0;
	}
protected:
	CC_SYNTHESIZE_PASS_BY_REF(int, _userId, UserId);
	CC_SYNTHESIZE_PASS_BY_REF(int, _coin, Coin);
	CC_SYNTHESIZE_PASS_BY_REF(int, _exp, Exp);
};

#endif // _USER_RECORD_SQLITE__H_