#include"common.h"
class Compilation {
	private:
		MYSQL *conn;
		int problem_id;
		int lang;
		void get_solution_info_from_mysql(int solution_id); 
		void update_solution_info_from_mysql(int solution_id, int result); 
		int get_file_size(const char * filename); 

	public:
		Compilation(MYSQL *conn = NULL); 		
		bool get_status(int solution_id); 
};
