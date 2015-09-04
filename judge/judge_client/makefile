judge_client: judge_client.o judge.o compilation.o 
	g++ -g -o  judge_client judge_client.o judge.o compilation.o -L /usr/lib -l mysqlclient -l pthread
judge_client.o: judge_client.cpp  compilation.h judge.h common.h Deal.hpp
	g++ -g -O2 -c judge_client.cpp
judge.o: judge.cpp judge.h common.h
	g++ -g -O2 -c judge.cpp
compilation.o: compilation.cpp compilation.h common.h
	g++ -g -O2 -c compilation.cpp
clean:
	rm -f judge.o compilation.o judge_client.o judge_client
