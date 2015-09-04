#include"common.h"

class Judge {
	private:
		MYSQL *conn;
		int time_lmt;
		int mem_lmt;
		char in_file[BUFFER_SIZE];
		char out_file[BUFFER_SIZE];
		int call_counter[512];
		int problem_id;
		int lang;

		void get_solution_info(int solution_id);
		void get_problem_info(); 
		void update_solution_info_from_mysql(int solution_id, int result, int used_time = 0, int used_mem = 0); 
		void prepare(int solution_id); 
		void run_solution(); 
		int get_proc_status(int pid, const char * mark); 
		int watch_solution(int &used_time, int &used_memory, pid_t pid); 
		int judge_solution(); 
		bool isb(int c);

	public:	
		Judge(MYSQL *conn = NULL);
		int get_status(int solution_id); 
};
