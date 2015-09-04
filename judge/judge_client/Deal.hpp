#include"common.h"
#include<string>
#include<vector>
#include<map>
#include<iostream>
using namespace std;


class Deal {
	public:
		Deal(MYSQL *conn = NULL) : _conn(conn) {
		}
		~Deal() {
		}
		void updateMysql(int code_id, int result) {
			_code_id = code_id;
			init();

			if(_type == 0) {
				mysql_add_one("user", "topic_submited_times" , "user_id", _user_id);
			}	
			else {
				mysql_add_one("user", "ladder_submited_times" , "user_id", _user_id);
			}

			if(result == OJ_AC) {
				if(_type == 0) {
					update_topic();
				}
				else {
					update_ladder();
				}
			}
		}

	protected :

		bool judge_toppic() {
			char sql[BUFFER_SIZE];
			sprintf(sql, "select problem_ids, pass_num from topic where topic_id = %d", _type_id);
			vector<string> tmp;
			mysql_select(sql, tmp);
			vector<string> problems;
			split(tmp[0], problems);
			map<string, bool> M;
			int sz = problems.size();
			for(int i = 0; i < sz; ++i) {
				M[problems[i]] = true;
				cout << problems[i] << endl;
			}

			int pass_num = atoi(tmp[1].c_str());

			printf("pass_num = %d\n", pass_num);

			sprintf(sql, "select problem_id from code where user_id = %d and judge_status = %d", _user_id, OJ_AC);

			vector<string> ac_problems;
			mysql_select(sql, ac_problems);
			sz = ac_problems.size();
			int num = 0;
			for(int i = 0; i < sz; ++i) {
				cout << "ac = " << ac_problems[i] << endl;
				if(M[ac_problems[i]]) ++num;
			}
			printf("num = %d\n", num);
			if(num >= pass_num) return true;
			else return false;
		}

		void update_topic() {
			char sql[BUFFER_SIZE];
			sprintf(sql, "update user set topic_solved_times = (select count(distinct problem_id) from code where user_id = %d and judge_status = %d and type = 0) where user_id = %d", _user_id, OJ_AC, _user_id);
			printf("%s\n", sql);
			mysql_query(_conn, sql);
			if(judge_toppic()) {
				sprintf(sql, "select pass_topics, at_topics from user where user_id = %d ", _user_id);
				vector<string> tmp;
				mysql_select(sql, tmp, 2);
				vector<string> pass_topics;
				vector<string> at_topics;
				split(tmp[0], pass_topics);
				split(tmp[1], at_topics);

				vector<string> new_topics;
				sprintf(sql, "select topic_id from topic where pre_topic = %d", _type_id);
				mysql_select(sql, new_topics);

				map<string, bool> M;
				char topic_tmp[10];
				sprintf(topic_tmp, "%d", _type_id);
				string topic_id(topic_tmp);
				cout << "current" << topic_id << endl;
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
				sprintf(sql, "update user set at_topics	= '%s', pass_topics = '%s' where user_id = %d", at_rt.c_str(), pass_rt.c_str(), _user_id);
				mysql_query(_conn, sql);
				printf("%s\n", sql);
			}
		}

		void insert_tip(const string &problem_id) {
			char sql[BUFFER_SIZE];
			time_t rawtime;
			struct tm* timeinfo;
			char time_str[80];                                                                 
			time(&rawtime);
			timeinfo  =localtime(&rawtime);
			strftime(time_str, 80,"%Y-%m-%d %I:%M:%S\n",timeinfo);
			sprintf(sql, "INSERT INTO `tip`(`user_id`, `open_time`, `problem_id`) VALUES (%d, '%s', %s)", _user_id, time_str, problem_id.c_str());
			printf("%s\n", sql);
			mysql_query(_conn, sql);
		}

		void update_ladder() {
			char sql[BUFFER_SIZE];
			sprintf(sql, "update user set ladder_solved_times = (select count(distinct problem_id) from code where user_id = %d and judge_status = %d and type = 1) where user_id = %d", _user_id, OJ_AC, _user_id);
			printf("%s\n", sql);
			mysql_query(_conn, sql);
			mysql_add_one("user", "at_ladder" , "user_id", _user_id);
			sprintf(sql, "select problem_id from ladder where ladder_id = %d", _type_id);
			vector<string> tmp;
			mysql_select(sql, tmp, 1);
			insert_tip(tmp[0]);
		}

		void init() {
			char sql[BUFFER_SIZE];
			sprintf(sql, "select user_id, problem_id, type, type_id from code where code_id = %d", _code_id);
			vector<string> result;
			mysql_select(sql, result, 4);
			_user_id = atoi(result[0].c_str());
			_problem_id = atoi(result[1].c_str());
			_type = atoi(result[2].c_str());
			_type_id = atoi(result[3].c_str());
			mysql_add_one("problem", "submit_times" , "problem_id", _user_id);
		}

		void split(string &str, vector<string> &result) {
			int sz = str.size();
			string tmp;
			for(int i = 0; i <= sz; ++i) {
				if(i == sz || str[i] == ',') {
					if(tmp.size() > 0) {
						result.push_back(tmp);
					}
					tmp.clear();
				}
				else tmp += str[i];
			}
		}

		void mysql_add_one(const char *table, const char *filed, const char *key, int value) {
			char sql[BUFFER_SIZE];
			sprintf(sql, "update %s set %s = %s + 1 where %s = %d", table, filed, filed, key, value);
			printf("%s\n", sql);
			mysql_query(_conn, sql);
		}

		void mysql_select(char *sql, vector<string> &result, int n) {
			printf("%s\n", sql);
			MYSQL_RES *res;
			MYSQL_ROW row;
			mysql_query(_conn, sql);
			res = mysql_store_result(_conn);
			while(row = mysql_fetch_row(res)) {
				for(int i = 0; i < n; ++i) {
					if(NULL == row[i]) {
						result.push_back(string());
					}
					else {
						result.push_back(string(row[i]));
					}
				}
			}
			mysql_free_result(res);
		}

		void mysql_select(char *sql, vector<string> &result) {
			printf("%s\n", sql);
			MYSQL_RES *res;
			MYSQL_ROW row;
			mysql_query(_conn, sql);
			res = mysql_store_result(_conn);
			while(row = mysql_fetch_row(res)) {
				result.push_back(string(row[0]));
			}
			mysql_free_result(res);
		}
	private:
		MYSQL *_conn;
		int _problem_id;
		int _user_id;
		int _code_id;
		int _type;
		int _type_id;

};
