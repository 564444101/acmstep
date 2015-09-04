#include "vjudge.h"
#include "SQLFactory.h"
#include "JudgeInfo.h"
#include "JudgeResult.h"
#include "HRBUSTJudger.h"
#include "HDUJudger.h"

static int solution_id;
static char work_dir[BUFFER_SIZE];

void init_parameters(int argc, char **argv) {
	if(argc < 3) {
		//printf("parameter is wrong");
		syslog(LOG_ERR, "parameter is wrong");
		exit(0);
	}
	setgid(1538);
	setuid(1538);
	int client_id = atoi(argv[1]);
	solution_id = atoi(argv[2]);
	sprintf(work_dir, "%s/run%d", "/home/vjudge", client_id);
	chdir(work_dir);
}

int main(int argc, char **argv)
{
	cout << "start..................." << endl;
	JudgeResult judgeResult;
	init_parameters(argc, argv);
	curl_global_init(CURL_GLOBAL_ALL);

	cout << "init parametres..............." << endl;
	SQLFactory judgeDAO = SQLFactory(HOST_NAME, USER_NAME, PASSWORD, DB_NAME, 0, solution_id);
	cout << "Create SQLFactory......................" << endl;
	JudgeInfo judgeInfo = judgeDAO.getJudgeInfoById(solution_id);
	cout << "Get judgeInfo......................" << judgeInfo.getOj() << endl;
	cout << "JudgeInfo: " << judgeInfo.getOj() << "| CodeId:" << judgeInfo.getCodeId() << "| Id:" << judgeInfo.getId() << "| lang:" << judgeInfo.getLang() <<"| Code:" << judgeInfo.getCode() << endl;
	if (judgeInfo.getOj() == HRBUST) {
		cout << "Create HrbustJudger......................" << endl;
		HRBUSTJudger hrbustJudger(judgeInfo);
		cout << "HrbustJudger run....................." << endl;
		hrbustJudger.run();
		cout << "Get judgeResult.................." << endl;
		judgeResult = hrbustJudger.getJudgeResult();
	} else if (judgeInfo.getOj() == HDU) {
		cout << "Create HDUJudger......................" << endl;
		HDUJudger hduJudger(judgeInfo);
		cout << "HDUJudger run....................." << endl;
		hduJudger.run();
		cout << "Get judgeResult.................." << endl;
		judgeResult = hduJudger.getJudgeResult();
	}/* else if (judgeInfo.getOj() == POJ) {
		POJJudger pojJudger = new POJJudger(judgeInfo);
		pojJudger.run();
		judgeResult = pojJudger.getJudgeResult();
	}*/
	cout << "JudgeResult: " << judgeResult.getCodeId() << " |JudgeStatus: " << judgeResult.getJudgeStatus();
    judgeDAO.updateInfoByResult(judgeResult, judgeInfo);
	cout << "Update JudgeInfo.................." << endl;

	curl_global_cleanup();
	return 0;
}
