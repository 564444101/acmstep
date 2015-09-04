vjudge_client : vjudge_client.o  HRBUSTJudger.o HDUJudger.o VirtualJudger.o  SQLFactory.o JudgeInfo.o  global.o JudgeResult.o JudgeInfo.o
	g++ -g -o vjudge_client vjudge_client.o  HRBUSTJudger.o HDUJudger.o VirtualJudger.o  SQLFactory.o JudgeInfo.o  global.o JudgeResult.o JudgeInfo.o -L /usr/lib -lcurl -lre2 -lmysqlclient   
vjudge_client.o : vjudge_client.cpp 
	g++  -c -g -O2 vjudge_client.cpp
HRBUSTJudger.o : HRBUSTJudger.h HRBUSTJudger.cpp VirtualJudger.h VirtualJudger.o global.o
	g++  -c -g -O2 HRBUSTJudger.cpp
HDUJudger.o : HDUJudger.h HDUJudger.cpp VirtualJudger.h VirtualJudger.o global.o entities.o
	g++ -c -g -O2 HDUJudger.cpp
VirtualJudger.o : VirtualJudger.h VirtualJudger.cpp vjudge.h JudgeResult.o JudgeInfo.o
	g++  -c -g -O2 VirtualJudger.cpp
SQLFactory.o : SQLFactory.h SQLFactory.cpp vjudge.h JudgeResult.o JudgeInfo.o
	g++  -c -g -O2 SQLFactory.cpp
JudgeResult.o : JudgeResult.h JudgeResult.cpp vjudge.h
	g++  -c -g -O2 JudgeResult.cpp
JudgeInfo.o : JudgeInfo.h JudgeInfo.cpp vjudge.h
	g++  -c -g -O2 JudgeInfo.cpp
global.o : global.h global.cpp 
	g++  -c -g -O2 global.cpp
entities.o : entities.c entities.h
	gcc -c -g -O2 entities.c
clean:
	rm -f vjudge_client vjudge_client.o  HRBUSTJudger.o HDUJudger.o VirtualJudger.o  SQLFactory.o JudgeInfo.o  global.o JudgeResult.o entities.o 
