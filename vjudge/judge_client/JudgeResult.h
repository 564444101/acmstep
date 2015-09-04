#ifndef JUDGERRESULT_HEADER
#define JUDGERRESULT_HEADER
#include "vjudge.h"

class JudgeResult {
	public:
		void setJudgeStatus(int judgeStatus) {
			this->judgeStatus = judgeStatus;
		}
		int getJudgeStatus() {
			return this->judgeStatus;
		}
		void setErrorInfo(string errorInfo) {
			this->errorInfo = errorInfo;
		}
		string getErrorInfo() {
			return this->errorInfo;
		}
		void setRunTime(int runTime) {
			this->runTime = runTime;
		}
		int getRunTime() {
			return this->runTime;
		}
		void setRunMemory(int runMemory) {
			this->runMemory = runMemory;
		}
		int getRunMemory() {
			return this->runMemory;
		}
		void setRunId(int runId) {
			this->runId = runId;
		}
		int getRunId() {
			return this->runId;
		}
		void setCodeId(int codeId) {
			this->codeId = codeId;
		}
		int getCodeId() {
			return this->codeId;
		}
	private:
		int codeId;
		int runId;
		int judgeStatus;
		string errorInfo;
		int runTime;
		int runMemory;
};
#endif
