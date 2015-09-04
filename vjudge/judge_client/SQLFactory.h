#ifndef SQLFACTORY_HEADER
#define SQLFACTORY_HEADER
#include "vjudge.h"
#include "JudgeInfo.h"
#include "JudgeResult.h"

class SQLFactory {
	public:
		SQLFactory(string, string, string, string, int, int);
		~SQLFactory();

		JudgeInfo getJudgeInfoById(int);
		void updateInfoByResult(JudgeResult, JudgeInfo);

		void setHostName(string _hostName) {
			this->hostName = _hostName;
		}
		string getHostName() {
			return this->hostName;
		}
		void setUser(string _user) {
			this->user = _user;
		}
		string getUser() {
			return this->user;
		}
		void setPassword(string _password) {
			this->password = _password;
		}
		string getPassword() {
			return this->password;
		}
		void setDbName(string _dbName) {
			this->dbName = _dbName;
		}
		string getDbName() {
			return this->dbName;
		}
		void setMaxWaitTime(int _maxWaitTime) {
			this->maxWaitTime = _maxWaitTime;
		}
		int getMaxWaitTime() {
			return this->maxWaitTime;
		}
		MYSQL* getConnect() {
			return this->connection;
		}
		void setUserId(int userId) {
			this->user_id = userId;
		}
		void setCodeId(int codeId) {
			this->code_id = codeId;
		}
		void setType(int type) {
			this->type = type;
		}
		void setTypeId(int typeId) {
			this->type_id = typeId;
		}


	protected:
		void updateJudgeResult(JudgeResult, JudgeInfo);
		bool judge_topic();
		void update_topic();
		void insert_tip(const string &);
		void update_ladder();
		void init();
		void mysql_add_one(const char*, const char*, const char*, int);
		void mysql_select(char *, vector<string>&result, int n);
		void mysql_select(char*, vector<string> &result);
		
		
	private:
		string hostName;
		string user;
		string password;
		string dbName;
		MYSQL* connection;

		int maxWaitTime;
		int user_id;
		int code_id;
		int type;
		int type_id;
};
#endif
