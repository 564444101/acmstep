#include "HRBUSTJudger.h"
#include <sstream>

HRBUSTJudger::HRBUSTJudger(JudgeInfo info)
{
	judgeInfo = info;
	language_table[0] = "1";
	language_table[1] = "2";
	language_table[2] = "3";
	cout << "Language table done!" << endl;
	status_table["Accepted"] = OJ_AC;
	status_table["Wrong Answer"] = OJ_WA;
	status_table["Presentation Error"] = OJ_PE;
	status_table["Compile Error"] = OJ_CE;
	status_table["Judging"] = OJ_WT0;
	status_table["Time Limit Exceeded"] = OJ_TL;
	status_table["Memory Limit Exceeded"] = OJ_ML;
	status_table["Restricted Function"] = OJ_RE;
	status_table["Runtime Error"] = OJ_RE;
	cout << "Status table done!" << endl;
	judgeResult = JudgeResult();
	cout << "JudgeResult done!" << endl;
}

HRBUSTJudger::~HRBUSTJudger() {
	closeFile();
}

void HRBUSTJudger::login() {
	prepareCurl();
	string lgin_url = "http://acm.hrbust.edu.cn/index.php?m=User&a=login";
	curl_easy_setopt(easy_handle, CURLOPT_URL, lgin_url.c_str());
	string post = (string)"m=User&a=login&user_name=acmstep&password=acmstep&ajax=1";
	curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, post.c_str()); 
	performCurl("tmpfiles/logindata.html");
	cout << "Login performCurl done!" << endl;
	string res = fileToString("tmpfiles/logindata.html");

	if (res.find("{\"status\":0") != string::npos ||
			res.find("<div class='body'>Sorry!Login Error,Please Retry!</div>") != string::npos) {
		cout << "Login failed!" << endl;
	} else cout << "Login success!" << endl;
}

void HRBUSTJudger::submit() {
//	prepareCurl();
	string sub_url = "http://acm.hrbust.edu.cn/index.php?m=ProblemSet&a=postCode";
	stringstream sStream;
	cout << "Submit start................" << endl;
	string post_par[][2] = {
		"jumpUrl=", "",
		"&language=", "2",
		"&problem_id=", "1000",
		"&source_code=", ""
	};
	cout << "Lang: " << judgeInfo.getLang() << "| id:" << judgeInfo.getId() << "| Code:" << judgeInfo.getCode() << endl;

	sStream << judgeInfo.getLang();
	sStream >> post_par[1][1];
	
	sStream << judgeInfo.getId();
	sStream >> post_par[2][1];
	post_par[3][1]  = judgeInfo.getCode();
	
	cout << "Url done!" << endl;
	string submit = post_par[0][0]+post_par[0][1]+post_par[1][0]+post_par[1][1]+post_par[2][0]+post_par[2][1]+post_par[3][0]+escapeURL(post_par[3][1]);
	cout << submit << endl;
	curl_easy_setopt(easy_handle, CURLOPT_URL, sub_url.c_str());
	curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, submit.c_str());
	performCurl("tmpfiles/submitdata.html");
	if (code != CURLE_OK) syslog(LOG_ERR, "submit failed."); 
}

void HRBUSTJudger::getStatus() {
//	prepareCurl();
	while (true) {
		stringstream sStream;
		string lang, outer_oj_id;
		char *lang_tmp, *outer_oj_id_tmp;
		sStream << judgeInfo.getLang();
		sStream >> lang;
		sStream << judgeInfo.getId();
		sStream >> outer_oj_id;
		string res_url = "http://acm.hrbust.edu.cn/index.php?m=Status&a=showStatus&problem_id=" + outer_oj_id + "&user_name=acmstep&language=" + lang; 
		curl_easy_setopt(easy_handle, CURLOPT_URL, res_url.c_str());
	cout << "Before performCurl()................." << endl;
		performCurl("tmpfiles/statusdata.html");
	cout << "After performCurl()................." << endl;
		string res, runid, result, time_used, memory_used;
		string html = fileToString("tmpfiles/statusdata.html");
		if (code != CURLE_OK) syslog(LOG_ERR, "getStatus failed.");
		else {
			if (html.find("var is_login=\"\";") != string::npos || !RE2::PartialMatch(html, "(?s)<table class=\"ojlist\".*?<tr.*?(<tr.*?</tr>)", &res)) {
				syslog(LOG_ERR, "[ERROR] HRBUSTJudger: Failed to get status row.");
			}
			if (!RE2::PartialMatch(res, "(?s)<td.*?<td>([0-9]*).*?<td.*?<td.*?>(.*?)</td>", &runid, &result)) {
				syslog(LOG_ERR, "[ERROR] HRBUSTJudger: Fail to get current result.");
			}
			result = convertResult(trim(result));
			cout << "Result is: " << result << endl;
			if (isFinalResult(result)) {
				if (!RE2::PartialMatch(res, "(?s)>([0-9]*)ms.*?>([0-9]*)k", &time_used, &memory_used)) {
					syslog(LOG_ERR, "[ERROR] HRBUSTJudger: Failed to parse details from status row.");
				}
				cout << "0---------------------------------" << endl;
				cout << "Time used: " << time_used << " Memory_used: " << memory_used<< endl;
				judgeResult.setCodeId(judgeInfo.getCodeId());
				judgeResult.setRunId(atoi(trim(runid).c_str()));
				judgeResult.setJudgeStatus(status_table[result]);
				judgeResult.setRunTime(atoi(trim(time_used).c_str()));
				judgeResult.setRunMemory(atoi(trim(memory_used).c_str()));

				cout << "[Result] Get JudgeStatus: " << judgeResult.getJudgeStatus() << endl;
				if (judgeResult.getJudgeStatus() == OJ_CE) {
					judgeResult.setErrorInfo(getCEinfo());
				} else {
					judgeResult.setErrorInfo("NULL");
				}
				break;
			} else {
				usleep(5000);
			}
		}
	}
}

string HRBUSTJudger::convertResult(string result) {
	if (result.find("Time Limit Exceeded") != string::npos) return "Time Limit Exceed";
	if (result.find("Memory Limit Exceeded") != string::npos) return "Memory Limit Exceed";
	if (result.find("Output Limit Exceeded") != string::npos) return "Output Limit Exceed";
	if (result.find("System Error") != string::npos) return "Judge Error";
	if (result.find("Compile Error") != string::npos) return "Compile Error";
	return trim(result);
}

string HRBUSTJudger::getCEinfo() {
	string outer_oj_runid;
	stringstream sStream;
//	prepareCurl();
	cout << "GET ceinfo ....." << judgeResult.getRunId() << endl;
	sStream << judgeResult.getRunId();
	sStream >> outer_oj_runid;
	string res_url = "http://acm.hrbust.edu.cn/index.php?m=Status&a=showCompileError&run_id=" + outer_oj_runid; 
	cout << "RES_URL is: " << res_url << endl;
	curl_easy_setopt(easy_handle, CURLOPT_URL, res_url.c_str());
	performCurl("tmpfiles/cedata.html");
	cout << "GET res_url done!" << endl;

	string info = fileToString("tmpfiles/cedata.html");

	cout << "[CEinfo] down info.................." << endl;
	cout << "length of info : " << info.length() << endl;
	//cout << info << endl;
	
	if (code != CURLE_OK) {
		syslog(LOG_ERR, "getCEInfo failed.");
		return "NULL";
	} else {
		cout << "[CEinfo] regex info...................." << endl;
		string result;
		if (!RE2::PartialMatch(info, "(?s)showcode_mod_info.*?>(.*?)</td>", &result)) {
			cout << "[CEinfo] CEinfo is: " << "NULL" << endl;
			return "NULL";
		}
		cout << "[CEinfo] Ceinfo is: " << result << endl;
		return result;
	}
	
	return "NULL";
}

