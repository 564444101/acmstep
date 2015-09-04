#ifndef VIRTUALJUDGER_HEADER
#define VIRTUALJUDGER_HEADER
#include "vjudge.h"
#include "JudgeResult.h"
#include "JudgeInfo.h"

class VirtualJudger {
	public:
		VirtualJudger();
		void run();
		JudgeResult getJudgeResult();
		FILE *curl_file;
	protected:
		JudgeInfo judgeInfo;
		JudgeResult judgeResult;
		map<int, string> language_table;
		bool isLogin;
		CURL *easy_handle;
		CURLcode code;
		map<string, int> status_table;

		void prepareCurl();
		void performCurl(const char * sfile);
		void closeFile();
		bool isFinalResult(string);
		void judge();
		virtual void login() = 0;
		virtual void submit() = 0;
		virtual void getStatus() = 0;
		virtual string getCEinfo() = 0; 
};
#endif
