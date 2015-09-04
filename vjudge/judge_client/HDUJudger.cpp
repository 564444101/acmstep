#include "HDUJudger.h"
#include <sstream>

HDUJudger::HDUJudger(JudgeInfo info)
{
	judgeInfo = info;
	language_table[0] = "2";
	language_table[1] = "1";
	language_table[5] = "3";
	cout << "Language table done!" << endl;
	status_table["Accepted"] = OJ_AC;
	status_table["Wrong Answer"] = OJ_WA;
	status_table["Presentation Error"] = OJ_PE;
	status_table["Compilation Error"] = OJ_CE;
	status_table["Queuing"] = OJ_WT1;
	status_table["Compiling"] = OJ_WT1;
	status_table["Running"] = OJ_WT1;
	status_table["Time Limit Exceeded"] = OJ_TL;
	status_table["Memory Limit Exceeded"] = OJ_ML;
	status_table["Restricted Function"] = OJ_RE;
	status_table["Runtime Error"] = OJ_RE;
	status_table["Output Limit Exceeded"] = OJ_OL;
	status_table["System Error"] = OJ_RE;
	
	cout << "Status table done!" << endl;
	judgeResult = JudgeResult();
	cout << "JudgeResult done!" << endl;
}

HDUJudger::~HDUJudger() {
//	closeFile();
}

void HDUJudger::login() {
	//prepareCurl();
	string lgin_url = "http://acm.hdu.edu.cn/userloginex.php?action=login";
	curl_easy_setopt(easy_handle, CURLOPT_URL, lgin_url.c_str());
	string post = (string)"username=newpluto&userpass=pluto&login=Sign+In";
	curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, post.c_str()); 
	cout << "Login test 0-----------------------" << endl;
	performCurl("tmpfiles/logindata.html");
	cout << "Login test 1-----------------------" << endl;
	string res = fileToString("tmpfiles/logindata.html");

    if (res.find("No such user or wrong password.") != string::npos ||res.find("<b>One or more following ERROR(s) occurred.") != string::npos || res.find("<h2>The requested URL could not be retrieved</h2>") != string::npos || res.find("<H1 style=\"COLOR: #1A5CC8\" align=center>Sign In Your Account</H1>") != string::npos || res.find("PHP: Maximum execution time of") != string::npos) {
		cout << "Login failed!" << endl;
	} else cout << "Login success!" << endl;
}

void HDUJudger::submit() {
	string sub_url = "http://acm.hdu.edu.cn/submit.php?action=submit";
	stringstream sStream;
	cout << "Submit start................" << endl;
	string post_par[][2] = {
		"check=0&problemid=", "1000",
		"&language=", "2",
		"&usercode=", "",
	};
	cout << "Lang: " << judgeInfo.getLang() << "| id:" << judgeInfo.getId() << "| Code:" << judgeInfo.getCode() << endl;
	post_par[2][1] += "#include <iostream>\r\nusing namespace std;\r\nint main()\r\n { int a, b; while (cin >> a >> b) {cout << a + b << endl;}return 0;}";
//	post_par[2][1] += "dsjahfjkshadjkfhajkshdfjkahsd";
	
	sStream << judgeInfo.getId();
	sStream >> post_par[0][1];
	sStream << judgeInfo.getLang();
	sStream >> post_par[1][1];
	post_par[2][1]  = judgeInfo.getCode();
	cout << "Url done!" << endl;
	
	string submit = post_par[0][0]+post_par[0][1]+post_par[1][0]+post_par[1][1]+post_par[2][0]+escapeURL(post_par[2][1]);
	cout << submit << endl;
	curl_easy_setopt(easy_handle, CURLOPT_URL, sub_url.c_str());
	curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, submit.c_str());
	cout << "Submit test 0------------------------" << endl;
	performCurl("tmpfiles/submitdata.html");
	cout << code << endl;
	if (code != CURLE_OK) cout << "submit failed." << endl;
	
}

void HDUJudger::getStatus() {
	while (true) {
		stringstream sStream;
		string lang, outer_oj_id;
		sStream << judgeInfo.getLang();
		sStream >> lang;
		sStream << judgeInfo.getId();
		sStream >> outer_oj_id;
		string res_url = "http://acm.hdu.edu.cn/status.php?first=&pid="+outer_oj_id+"&user=newpluto&lang=&status=0"; 
		cout << "getStatus done!" << endl;
		curl_easy_setopt(easy_handle, CURLOPT_URL, res_url.c_str());
		cout << "getStatus seturl done!" << endl;
		performCurl("tmpfiles/statusdata.html");
		cout << "getStatus perform done!" << endl;
		string res, runid, result, time_used, memory_used;
		string html = fileToString("tmpfiles/statusdata.html");
		cout << "getStatus download html done!" << endl;
		if (html.find("Connect(0) to MySQL Server failed.") != string::npos ||
				html.find("<b>One or more following ERROR(s) occurred.") != string::npos ||
				html.find("<h2>The requested URL could not be retrieved</h2>") != string::npos ||
				html.find("PHP: Maximum execution time of") != string::npos || 
				html.find("<H1 style=\"COLOR: #1A5CC8\" align=center>Sign In Your Account</H1>") != string::npos ||
				html.find("<DIV>Exercise Is Closed Now!</DIV>") != string::npos ||
				!RE2::PartialMatch(html, "(?s)<table.*?(<tr align=center.*?</font>)", &res)) {
			syslog(LOG_ERR, "[ERROR] HDUJudger: Failed to get status row.");
			cout << "Failed to get status row." << endl;
		} else {
			if (!RE2::PartialMatch(res, "(?s)<td.*?>([0-9]*)</td>.*?<font.*?>(.*)</font>", &runid, &result)) {
				syslog(LOG_ERR, "[ERROR] HDUJudger: Fail to get current result.");
			}
//			result = convertResult(trim(result));
			cout << "RunId is : " << runid << "Result is : " << result << endl;
			if (isFinalResult(result)) {
				if (!RE2::PartialMatch(res, "(?s)([0-9]*)MS.*?([0-9]*)K", &time_used, &memory_used)) {
					syslog(LOG_ERR, "[ERROR] HDUJudger: Failed to parse details from status row.");
				}
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
				sleep(1);
			}
		}
	}
}

string HDUJudger::convertResult(string result) {
	if (result.find("Time Limit Exceeded") != string::npos) return "Time Limit Exceed";
	if (result.find("Memory Limit Exceeded") != string::npos) return "Memory Limit Exceed";
	if (result.find("Output Limit Exceeded") != string::npos) return "Output Limit Exceed";
	if (result.find("Compilation Error") != string::npos) return "Compile Error";
	if (result.find("Runtime Error") != string::npos) return "Runtime Error";
	return trim(result);
}

string HDUJudger::getCEinfo() {
	string outer_oj_runid;
	stringstream sStream;
	cout << "GET ceinfo ....." << judgeResult.getRunId() << endl;
	sStream << judgeResult.getRunId();
	sStream >> outer_oj_runid;
	string res_url = "http://acm.hdu.edu.cn/viewerror.php?rid=" + outer_oj_runid; 
	curl_easy_setopt(easy_handle, CURLOPT_URL, res_url.c_str());
	performCurl("tmpfiles/cedata.html");
	string info = fileToString("tmpfiles/cedata.html");

	cout << "[CEinfo] down info.................." << endl;
	cout << "length of info : " << info.length() << endl;
	//cout << info << endl;
	
	cout << "[CEinfo] regex info...................." << endl;
	string result;
	char *ce_info = new char[info.length()+1];
	strcpy(ce_info, info.c_str());
	char *buffer = new char[info.length()*2];
	charset_convert("GBK", "UTF-8//TRANSLIT", ce_info, info.length() + 1, buffer, info.length()*2);
	if (!RE2::PartialMatch(buffer, "(?s)<pre>(.*?)</pre>", &result)) {
		cout << "[CEinfo] CEinfo is: " << "NULL" << endl;
		return "NULL";
	}
	cout << "[CEinfo] Ceinfo is: " << result << endl;
	//strcpy(buffer, result.c_str());
	//decode_html_entities_utf8(buffer, NULL);
	//result = buffer;
	delete [] ce_info;
	delete [] buffer;
	return result;
}

