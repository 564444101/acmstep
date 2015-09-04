#ifndef HDUJUDGER_HEADER
#define HDUJUDGER_HEADER
#include "vjudge.h"
#include "VirtualJudger.h"

class HDUJudger: public VirtualJudger {
	public:
		HDUJudger(JudgeInfo);	
		~HDUJudger();
	protected:
		void login();
		void submit();
		void getStatus();
		string getCEinfo();
		string convertResult(string);
};
#endif
