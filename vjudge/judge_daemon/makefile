vjudge_daemon: vjudge_daemon.o 
	g++ -o vjudge_daemon vjudge_daemon.o  -L /usr/lib -l mysqlclient -l pthread
vjudge_daemon.o: vjudge_daemon.cpp
	g++ -c vjudge_daemon.cpp
clean:
	rm -f vjudge_daemon.o vjudge_daemon


