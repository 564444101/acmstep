#include"judge.h"
#include"okcalls.h"

#ifdef __i386                                                                                  
#define REG_SYSCALL orig_eax
#define REG_RET eax
#define REG_ARG0 ebx
#define REG_ARG1 ecx
#else
#define REG_SYSCALL orig_rax
#define REG_RET rax
#define REG_ARG0 rdi
#define REG_ARG1 rsi
#endif

void Judge::get_solution_info(int solution_id) {
	MYSQL_RES *res;
	MYSQL_ROW row;
	char sql[BUFFER_SIZE];
	sprintf(sql,"SELECT problem_id, lang FROM code where code_id=%d", solution_id);
	mysql_query(conn, sql);
	res = mysql_store_result(conn);
	row = mysql_fetch_row(res);
	problem_id = atoi(row[0]);
	lang = atoi(row[1]);
	mysql_free_result(res);
}

void Judge::get_problem_info() {
	char sql[BUFFER_SIZE];
	MYSQL_RES *res;
	MYSQL_ROW row;
	sprintf(sql, "SELECT time_limit, memory_limit FROM problem where problem_id=%d",problem_id);
	mysql_query(conn, sql);
	res = mysql_store_result(conn);
	row = mysql_fetch_row(res);
	time_lmt = atoi(row[0]);
	mem_lmt = atoi(row[1]);
	mysql_free_result(res);
}

void Judge::update_solution_info_from_mysql(int solution_id, int result, int used_time, int used_mem) {
	char sql[BUFFER_SIZE];
	sprintf(sql, "UPDATE code SET judge_status= %d,run_time = %d,run_memory = %d WHERE code_id= %d", result, used_time, used_mem, solution_id);
	//printf("%s\n", sql);
	mysql_query(conn, sql); 
}

void Judge::prepare(int solution_id) {
	update_solution_info_from_mysql(solution_id, OJ_RI);
	sprintf(in_file, "/var/www/acmstep/web/data/%d/%d.in", problem_id, problem_id);
	sprintf(out_file, "/var/www/acmstep/web/data/%d/%d.out",  problem_id, problem_id);
	memset(call_counter, 0, sizeof(call_counter));
	for(int i = 0; LANG_CC[i]; ++i) {
		call_counter[LANG_CV[i]] = LANG_CC[i];
	}
	printf("%s %s\n", in_file, out_file);
}

void Judge::run_solution() {
	freopen(in_file, "r", stdin);
	freopen("user.out", "w", stdout);
	freopen("error.out", "a+", stderr);
	// trace me
	ptrace(PTRACE_TRACEME, 0, NULL, NULL);
	struct rlimit LIM; // time limit, file limit& memory limit
	// time limit
	LIM.rlim_cur = (time_lmt / 1000) + 1;
	LIM.rlim_max = LIM.rlim_cur;
	setrlimit(RLIMIT_CPU, &LIM);
	alarm(0);
	alarm(time_lmt*10);
	// file limit
	LIM.rlim_max = STD_F_LIM + STD_MB;
	LIM.rlim_cur = STD_F_LIM;
	setrlimit(RLIMIT_FSIZE, &LIM);
	// proc limit
	LIM.rlim_cur=LIM.rlim_max=1;
	setrlimit(RLIMIT_NPROC, &LIM);
	// set the stack
	LIM.rlim_cur = STD_MB << 6;
	LIM.rlim_max = STD_MB << 6;
	setrlimit(RLIMIT_STACK, &LIM);
	// set the memory
	LIM.rlim_cur = STD_MB *mem_lmt/2*3;
	LIM.rlim_max = STD_MB *mem_lmt*2;
	setrlimit(RLIMIT_AS, &LIM);
	execl("./Main", "./Main", (char *)NULL);
	exit(0);
}

int Judge::get_proc_status(int pid, const char * mark) {
	FILE * pf;
	char fn[BUFFER_SIZE], buf[BUFFER_SIZE];
	int ret = 0;
	sprintf(fn, "/proc/%d/status", pid);
	pf = fopen(fn, "r");
	int m = strlen(mark);
	while (pf && fgets(buf, BUFFER_SIZE - 1, pf)) {

		buf[strlen(buf) - 1] = 0;
		if (strncmp(buf, mark, m) == 0) {
			sscanf(buf + m + 1, "%d", &ret);
		}
	}
	if (pf) fclose(pf);
	return ret;
}

int Judge::watch_solution(int &used_time, int &used_memory, pid_t pid) {
	int topmemory = 0;
	int result = OJ_AC;
	int sub = 0;
	struct rusage ruse;
	while (1) {
		int status;
		wait4(pid, &status, 0, &ruse);
		int tempmemory = (get_proc_status(pid, "VmPeak:") << 10);
		if (tempmemory > topmemory)
			topmemory = tempmemory;
		if (topmemory > mem_lmt * STD_MB) {
			result = OJ_ML;
			ptrace(PTRACE_KILL, pid, NULL, NULL);
			break;
		}
		if(WIFEXITED(status)) break;
		int sig;
		bool isstop = false;
		bool issig = false;
		if(WIFSTOPPED(status)) {
			sig = WSTOPSIG(status);
			isstop = true;
		}
		if (WIFSIGNALED(status)) {
			sig = WTERMSIG(status);
			issig = true;
		}
		if(!(isstop && sig == 5)) {
			switch(sig) {
				case SIGCHLD:
				case SIGALRM:
					alarm(0);
				case SIGKILL:
				case SIGXCPU:
					result = OJ_TL;
					break;
				case SIGXFSZ:
					result = OJ_OL;
					break;
				default:
					result = OJ_RE;
			}
			if(isstop) {
				ptrace(PTRACE_KILL, pid, NULL, NULL);
			}
			break;
		}
		struct user_regs_struct reg;
		ptrace(PTRACE_GETREGS, pid, NULL, &reg);
		if (call_counter[reg.REG_SYSCALL] == 0) { 
			result = OJ_RE;
			ptrace(PTRACE_KILL, pid, NULL, NULL);
			break;
		} else {
			if (sub == 1 && call_counter[reg.REG_SYSCALL] > 0) {
				//printf("sub reg = %d\n", reg.REG_SYSCALL);
				call_counter[reg.REG_SYSCALL]--;
			}
		}
		sub = 1 - sub;
		ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
	}
	wait(NULL);
	used_time += (ruse.ru_utime.tv_sec * 1000 + ruse.ru_utime.tv_usec / 1000);
	used_time += (ruse.ru_stime.tv_sec * 1000 + ruse.ru_stime.tv_usec / 1000);
	used_memory = topmemory;
	if(used_time > time_lmt) result = OJ_TL;
	return result;
}

int Judge::judge_solution() {
	FILE *fp1 = fopen("user.out", "r");
	FILE *fp2 = fopen(out_file, "r");
	bool pe = false;
	bool nrc2 = false;
	bool nrc1 = false;
	int c1, c2;
	while(!feof(fp1) && !feof(fp2)) {
		if(pe) {
			while(!nrc1 && (c1 = fgetc(fp1)) != EOF && isb(c1)); 
			while(!nrc2 && (c2 = fgetc(fp2)) != EOF && isb(c2)); 
			nrc1 = nrc2 = false;
			if(c1 != c2) return OJ_WA;
		}
		else {
			c1 = fgetc(fp1);
			c2 = fgetc(fp2);
			if(c1 != c2) {
				if(isb(c1) && isb(c2)) {
					pe = true;
				}
				else if(isb(c1)) { 
					pe = true;
					nrc2 = true;
				}
				else if(isb(c2)) {
					pe = true;
					nrc1 = true;
				}
				else return OJ_WA;
			}
		}
	}
	while((c1 = fgetc(fp1)) != EOF) {
		if(!isb(c1)) return OJ_WA;
	}
	while((c2 = fgetc(fp2)) != EOF) {
		if(!isb(c2)) return OJ_WA;
	}
	fclose(fp1);
	fclose(fp2);
	if(pe) return OJ_PE;
	else return OJ_AC;
}

bool Judge::isb(int c) {
	return c == ' ' || c == '	' || c == '\n';
}


Judge::Judge(MYSQL *conn) : conn(conn){
}

int Judge::get_status(int solution_id) {
	get_solution_info(solution_id);
	get_problem_info();
	prepare(solution_id);
	pid_t pid;
	int result;
	int used_time = 0;
	int used_mem = 0;
	if((pid = fork()) == 0) {
		run_solution();
	}
	else {
		result = watch_solution(used_time, used_mem, pid);
		if(result == OJ_AC) {
			result = judge_solution();
		}
	}
	if(result == OJ_TL) used_time = time_lmt;
	update_solution_info_from_mysql(solution_id, result, used_time, (used_mem >> 10));
	return result;
}
