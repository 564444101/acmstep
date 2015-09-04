#include "VirtualJudger.h"
#include <sstream>

VirtualJudger::VirtualJudger() {
	prepareCurl();
	isLogin = false;
}

void VirtualJudger::prepareCurl() {
	easy_handle = curl_easy_init();
	if (NULL == easy_handle)
	{
		cerr << "get a easy handle failed." << endl;
		curl_global_cleanup();
		return;
	}
	curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 90);
	curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, 90); 
	curl_easy_setopt(easy_handle, CURLOPT_COOKIEFILE, "cookies/");
	curl_easy_setopt(easy_handle, CURLOPT_COOKIEJAR, "cookies/");
/*
	curl_file = fopen("tmpfiles/data.html", "w");
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, curl_file);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, NULL);
	*/
}

void VirtualJudger::closeFile() {
	curl_easy_cleanup(easy_handle);
	FILE * fp = fopen("cookie", "w");
	fclose(fp);
}

void VirtualJudger::performCurl(const char * sfile = "tmpfiles/data.html") {
	curl_file = fopen(sfile, "w");
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, curl_file);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, NULL);
	curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1);
	code = curl_easy_perform(easy_handle); //bug	
	cout << "Perform Curl set opt done..........." << endl;
	fclose(curl_file);
	cout << "PerformCurl: " << code << CURLE_OK << endl;
	if (code != CURLE_OK) {
		syslog(LOG_ERR, ((string)"[ERROR] Curl failed, reason: " + curl_easy_strerror(code)).c_str());
		exit(0);
	}
//	curl_easy_cleanup(easy_handle);
}

bool VirtualJudger::isFinalResult(string result)
{
	result = toLowerCase(trim(result));

	// Minimum length result is "Accept"
	if (result.length() < 6) return false;
	if (result.find("waiting") != string::npos) return false;
	if (result.find("running") != string::npos) return false;
	if (result.find("judging") != string::npos) return false;
	if (result.find("presentation") == string::npos && result.find("sent") != string::npos) return false;
	if (result.find("queu") != string::npos) return false;
	if (result.find("compiling") != string::npos) return false;
	if (result.find("linking") != string::npos) return false;
	if (result.find("received") != string::npos) return false;
	if (result.find("pending") != string::npos) return false;
	if (result.find("not judged yet") != string::npos) return false;
	if (result.find("being judged") != string::npos) return false;

	return true;
}

JudgeResult VirtualJudger::getJudgeResult()
{
	return this->judgeResult;
}

void VirtualJudger::judge() {
	if (language_table.find(judgeInfo.getLang()) == language_table.end()) {
		syslog(LOG_ERR, "Unsupported language.");
	}
	judgeInfo.setLang(atoi(language_table[judgeInfo.getLang()].c_str()));
	if (!isLogin) {
		FILE * fp = fopen("cookie", "w");
		if (fp == NULL) {
			syslog(LOG_ERR, "[ERROR]Virtual Judger: Open cookies error!"); 
			cout << "Open file failed!" << endl;
			exit(0);
		}
		fclose(fp);
		cout << "Open file done!" << endl;
		login();
		cout << "Login done!" << endl;
		isLogin = true;
	}
	submit();
	cout << "submit done!" << endl;
	sleep(1); //等待外部OJ评判
}

void VirtualJudger::run() {
	judge();
	cout << "judge done!" << endl;
	getStatus();
	cout << "getStatus done!" << endl;
}
