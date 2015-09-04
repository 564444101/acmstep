judge_daemon: judge_daemon.o 
	g++ -o judge_daemon judge_daemon.o  -L /usr/lib -l mysqlclient -l pthread
judge_daemon.o: judge_daemon.cpp
	g++ -c judge_daemon.cpp
clean:
	rm -f judge_daemon.o judge_daemon


