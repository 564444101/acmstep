#include"compilation.h"
#include <string>
using namespace std;
void Compilation::get_solution_info_from_mysql(int solution_id) {
	MYSQL_RES *res;
	MYSQL_ROW row;
	char sql[BUFFER_SIZE];
	sprintf(sql,"SELECT problem_id, lang, code FROM code where code_id=%d", solution_id);
	mysql_query(conn, sql);
	res = mysql_store_result(conn);
	row = mysql_fetch_row(res);
	problem_id = atoi(row[0]);
	lang = atoi(row[1]);
	const char *lang_ext[] = {"c", "cpp"};
	char src_pth[BUFFER_SIZE];
	sprintf(src_pth, "Main.%s", lang_ext[lang]);
	FILE *fp_src = fopen(src_pth, "w");
	fprintf(fp_src, "%s", row[2]);
	fclose(fp_src);
	mysql_free_result(res);
}

void Compilation::update_solution_info_from_mysql(int solution_id, int result) {
	char sql[BUFFER_SIZE];
	sprintf(sql, "UPDATE code SET judge_status= %d WHERE code_id=%d", result, solution_id);
	printf("%s\n", sql);
	mysql_query(conn, sql); 
}

int Compilation::get_file_size(const char * filename) {
	struct stat f_stat;
	if (stat(filename, &f_stat) == -1) {
		return 0;
	}
	return f_stat.st_size;
}


Compilation::Compilation(MYSQL *conn) : conn(conn) {
	problem_id = 1000;
	lang = 0;
}
bool Compilation::get_status(int solution_id) {
	update_solution_info_from_mysql(solution_id, OJ_CI);
	get_solution_info_from_mysql(solution_id);
	pid_t pid;
	if((pid = fork()) == 0) {
		struct rlimit LIM;
		LIM.rlim_max = 600;
		LIM.rlim_cur = 600;
		setrlimit(RLIMIT_CPU, &LIM);
		LIM.rlim_max = 900 * STD_MB;
		LIM.rlim_cur = 900 * STD_MB;
		setrlimit(RLIMIT_FSIZE, &LIM);
		LIM.rlim_max = STD_MB<<11;
		LIM.rlim_cur = STD_MB<<11;
		setrlimit(RLIMIT_AS, &LIM);
		const char * CP_C[] = { "gcc", "Main.c", "-o", "Main", "-O2","-w", "-lm",NULL};
		const char * CP_X[] = { "g++", "Main.cpp", "-o", "Main","-O2","-w", "-lm", NULL};
		freopen("ce.txt", "w", stderr);
		switch (lang) {
			case 0:
				execvp(CP_C[0], (char * const *) CP_C);
				//system("gcc Main.c -o Main -O2 -w -static");
				break;
			case 1: 
				execvp(CP_X[0], (char * const *) CP_X);
				//system("g++ Main.cpp -o Main -O2 -w -static");
				break;
			default:
				printf("lang is wrong\n");
		}
		exit(0);
	}
	else {
		int status=0;
		bool is_ce = false;
		waitpid(pid, &status, 0);

		if(status) {
			is_ce = true;
		}
		if(get_file_size("ce.txt") > 0) {
			is_ce = true;
			FILE *fp = fopen("ce.txt", "r");
			char buf[BUFFER_SIZE];
			string ce_info;
			while(NULL != fgets(buf, BUFFER_SIZE, fp)) {
				for(int i = 0; buf[i]; ++i) ce_info += buf[i];
				ce_info += '\n';
			}
			char sql[BUFFER_SIZE];
			sprintf(sql, "update code SET ce_info = '%s' WHERE code_id=%d", ce_info.c_str(), solution_id);
			printf("%s\n", sql);
			mysql_query(conn, sql); 
		}

		if(is_ce) {
			update_solution_info_from_mysql(solution_id, OJ_CE);
		}

		return is_ce;
	}
}
