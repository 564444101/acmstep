#ifndef HRBUSTJUDGER_HEADER
#define HRBUSTJUDGER_HEADER
#include "vjudge.h"
#include "VirtualJudger.h"

class HRBUSTJudger : public VirtualJudger {
	public:
		HRBUSTJudger(JudgeInfo);	
		~HRBUSTJudger();
	protected:
		virtual void login();
		virtual void submit();
		virtual void getStatus();
		virtual string getCEinfo();
		string convertResult(string);
};
#endif
