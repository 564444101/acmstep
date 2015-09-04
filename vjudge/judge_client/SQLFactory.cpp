#include "SQLFactory.h"

SQLFactory::SQLFactory(string hostName = "localhost", string user="root", string password="vj123456", string dbName="acmstep", int maxWaitTime=0, int codeId=1000):hostName(hostName), user(user), password(password), dbName(dbName), maxWaitTime(maxWaitTime), code_id(codeId){
	connection = mysql_init(NULL);
	
	if (mysql_real_connect(connection, hostName.c_str(), user.c_str(), password.c_str(), dbName.c_str(), 3306, 0, 0) == NULL) {
		syslog(LOG_ERR, "[ERROR]vjudge: Connect to DB failed! %s", mysql_error(connection));
	} else {
		mysql_set_character_set(connection, "utf8");
		char sql[BUFFER_SIZE];
		sprintf(sql, "select user_id,type,type_id from code where code_id=%d", code_id);
		mysql_query(connection, sql);
		MYSQL_RES *res;
		MYSQL_ROW row;
		res = mysql_store_result(connection);
		if ((row = mysql_fetch_row(res)) != NULL) {
			user_id = atoi(row[0]);
			type = atoi(row[1]);
			type_id = atoi(row[2]);
		} else {
			syslog(LOG_ERR, "[ERROR]: SQLFactory: Can not find info by CodeId");
		}
	}
}

SQLFactory::~SQLFactory() {
	mysql_close(connection);
}

JudgeInfo SQLFactory::getJudgeInfoById(int id) {
	if (NULL == connection) {
		syslog(LOG_ERR, "[ERROR]vjudge:GetJudgeInfo Failed!");
		return NULL;
	}
	MYSQL_RES *res;
	MYSQL_ROW row;
	char sql[BUFFER_SIZE];

	sprintf(sql, "select oj_id,outer_oj_pid from problem where problem_id in (select problem_id from code where code_id= %d)", id);
	mysql_query(connection, sql);
	res = mysql_store_result(connection);
	if ((row = mysql_fetch_row(res)) != NULL) {
		int oj = atoi(row[0]);
		int oj_pid = atoi(row[1]);

		sprintf(sql, "select lang,submit_time,code,user_id,type,type_id from code where code_id=%d", id);
		mysql_query(connection, sql);
		res = mysql_store_result(connection);
		if ((row = mysql_fetch_row(res)) != NULL) {
			JudgeInfo judgeInfo(oj, oj_pid, 0, atoi(row[0]), atoi(row[1]), row[2], atoi(row[3]), id, atoi(row[4]), atoi(row[5]));
			return judgeInfo;
		}
		return NULL;
	}
}

void SQLFactory::updateInfoByResult(JudgeResult judgeResult, JudgeInfo judgeInfo) {
	if (NULL == connection) {
		syslog(LOG_ERR, "[ERROR]vjudge:Update Failed!");
		return;
	}
	updateJudgeResult(judgeResult, judgeInfo);
	init();
	if (type == 0) {
		mysql_add_one("user", "topic_submited_times", "user_id", user_id);
	} else {
		mysql_add_one("user", "ladder_submited_times", "user_id", user_id);
	}

	if (judgeResult.getJudgeStatus() == OJ_AC) {
		if (type == 0) {
			update_topic();
		} else {
			update_ladder();
		}
	}
}

void SQLFactory::updateJudgeResult(JudgeResult judgeResult, JudgeInfo judgeInfo) {
	char sql[BUFFER_SIZE];
	sprintf(sql, "update code set judge_status=%d,ce_info='%s',run_time=%d,run_memory=%d where code_id = %d", judgeResult.getJudgeStatus(), judgeResult.getErrorInfo().c_str(), judgeResult.getRunTime(), judgeResult.getRunMemory(), judgeResult.getCodeId());
	cout << sql << endl;
	mysql_query(connection, sql);
}

bool SQLFactory::judge_topic() {
	char sql[BUFFER_SIZE];
	sprintf(sql, "select problem_ids, pass_num from topic where topic_id=%d", type_id);
	vector<string> tmp;
	mysql_select(sql, tmp);
	vector<string> problems = split(tmp[0], ',');
	map<string, bool> M;
	int sz = problems.size();
	for (int i = 0; i < sz; ++i) {
		M[problems[i]] = true;
	}
	int pass_num = atoi(tmp[1].c_str());
	sprintf(sql, "select problem_id from code where user_id = %d and judge_status=%d", user_id, OJ_AC);
	vector<string> ac_problems;
	mysql_select(sql, ac_problems);
	sz = ac_problems.size();
	int num = 0;
	for (int i = 0; i < sz; ++i) {
		if (M[ac_problems[i]]) ++num;
	}
	if (num >= pass_num) return true;
	else return false;
}

void SQLFactory::update_topic() {
	char sql[BUFFER_SIZE];
	sprintf(sql, "update user set topic_solved_times = (select count(distinct problem_id) from code where user_id = %d and judge_status = %d and type = 0) where user_id = %d", user_id, OJ_AC, user_id);
	mysql_query(connection, sql);
	if (judge_topic()) {
		cout << "Come into Judge_Topic()" << endl;
		vector<string> tmp;
		mysql_select(sql, tmp, 2);
		if (tmp.size() != 0) {
			vector<string> pass_topics;
			vector<string> at_topics;
			pass_topics = split(tmp[0], ',');
			at_topics = split(tmp[1], ',');

			vector<string> new_topics;
			sprintf(sql, "select topic_id from topic where pre_topic = %d", type_id);
			mysql_select(sql, new_topics);

			map<string, bool> M;
			char topic_tmp[10];
			sprintf(topic_tmp, "%d", type_id);
			string topic_id(topic_tmp);
			string pass_rt = string(topic_id.c_str());
			M[topic_id] = true;

			int sz = pass_topics.size();
			for(int i = 0; i < sz; ++i) {
				if(pass_topics[i] == topic_id) return;
				if(M[pass_topics[i]] == false) {
					if(pass_rt.size() > 0) pass_rt += ',';
					pass_rt += pass_topics[i];
					M[pass_topics[i]] = true;
				}
			}

			M.clear();
			M[topic_id] = true;
			string at_rt;
			sz = at_topics.size();
			for(int i = 0; i < sz; ++i) {
				if(M[at_topics[i]] == false) {
					if(at_rt.size() > 0) at_rt += ',';
					at_rt += at_topics[i];
					M[at_topics[i]] = true;
				}
			}
			sz = new_topics.size();
			for(int i = 0; i < sz; ++i) {
				if(M[new_topics[i]] == false) {
					if(at_rt.size() > 0) at_rt += ',';
					at_rt += new_topics[i];
					M[new_topics[i]] = true;

					vector<string> problems;
					sprintf(sql, "select problem_ids from topic where topic_id = %s", new_topics[i].c_str());
					mysql_select(sql, problems);
					sz = problems.size();
					for(int i = 0; i < sz; ++i) {
						insert_tip(problems[i]);
					}
				}
			}
			cout << at_rt << endl;
			cout << pass_rt << endl;
			sprintf(sql, "update user set at_topics	= '%s', pass_topics = '%s' where user_id = %d", at_rt.c_str(), pass_rt.c_str(), user_id);
			mysql_query(connection, sql);
			printf("%s\n", sql);
		}
	}
}

void SQLFactory::insert_tip(const string &problem_id) {
	char sql[BUFFER_SIZE];
	time_t rawtime;
	struct tm* timeinfo;
	char time_str[80];                                                                 
	time(&rawtime);
	timeinfo  =localtime(&rawtime);
	strftime(time_str, 80,"%Y-%m-%d %I:%M:%S\n",timeinfo);
	sprintf(sql, "INSERT INTO `tip`(`user_id`, `open_time`, `problem_id`) VALUES (%d, '%s', %s)", user_id, time_str, problem_id.c_str());
	printf("%s\n", sql);
	mysql_query(connection, sql);
}

void SQLFactory::update_ladder() {
	char sql[BUFFER_SIZE];
	sprintf(sql, "update user set ladder_solved_times = (select count(distinct problem_id) from code where user_id = %d and judge_status = %d and type = 1) where user_id = %d", user_id, OJ_AC, user_id);
	printf("%s\n", sql);
	mysql_query(connection, sql);
	mysql_add_one("user", "at_ladder" , "user_id", user_id);
	sprintf(sql, "select problem_id from ladder where ladder_id = %d", type_id);
	vector<string> tmp;
	mysql_select(sql, tmp, 1);
	insert_tip(tmp[0]);
}

void SQLFactory::init() {
	char sql[BUFFER_SIZE];
	sprintf(sql, "select user_id, problem_id, type, type_id from code where code_id = %d", code_id);
	vector<string> result;
	mysql_select(sql, result, 4);
	user_id = atoi(result[0].c_str());
	int pid = atoi(result[1].c_str());
	type = atoi(result[2].c_str());
	type_id = atoi(result[3].c_str());
	mysql_add_one("problem", "submit_times" , "problem_id", user_id);
}

void SQLFactory::mysql_add_one(const char *table, const char *filed, const char *key, int value) {
	char sql[BUFFER_SIZE];
	sprintf(sql, "update %s set %s = %s + 1 where %s = %d", table, filed, filed, key, value);
	printf("%s\n", sql);
	mysql_query(connection, sql);
}

void SQLFactory::mysql_select(char *sql, vector<string> &result, int n) {
	printf("%s\n", sql);
	MYSQL_RES *res;
	MYSQL_ROW row;
	int sql_res = mysql_query(connection, sql);
	res = mysql_store_result(connection);
	while(NULL != res && (row = mysql_fetch_row(res))) {
		for(int i = 0; i < n; ++i) {
			if(NULL == row[i]) {
				result.push_back(string());
			}
			else {
				result.push_back(string(row[i]));
			}
		}
	}
	if (res != NULL) mysql_free_result(res);
	cout << "Update topic done!" << endl;
}

void SQLFactory::mysql_select(char *sql, vector<string> &result) {
	printf("%s\n", sql);
	MYSQL_RES *res;
	MYSQL_ROW row;
	mysql_query(connection, sql);
	res = mysql_store_result(connection);
	while(row = mysql_fetch_row(res)) {
		result.push_back(string(row[0]));
	}
	mysql_free_result(res);
}

