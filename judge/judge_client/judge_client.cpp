#include"common.h"
#include"compilation.h"
#include"judge.h"
#include"Deal.hpp"

static MYSQL *conn;
static char host_name[BUFFER_SIZE];
static char user_name[BUFFER_SIZE];
static char password[BUFFER_SIZE];
static char db_name[BUFFER_SIZE];
static int port_number;
static int client_id;
static int solution_id;
static char work_dir[BUFFER_SIZE];

void init_parameters(int argc, char **argv) {
	if(argc < 3) {
		printf("parameter is wrong");
		exit(0);
	}
	setgid(1536);
	setuid(1536);
	client_id = atoi(argv[1]);
	solution_id = atoi(argv[2]);
	sprintf(work_dir, "%s/run%d", "/home/judge", client_id);
	chdir(work_dir);
	system("rm -rf *");
}

void init_conf() {
	strcpy(host_name, "localhost");
	strcpy(user_name, "root");
	strcpy(password, "vj123456");
	strcpy(db_name, "acmstep");
	port_number = 3306;
}

int init_mysql() {
	conn = mysql_init(NULL);

	if(!mysql_real_connect(conn, host_name, user_name, password, db_name, port_number, 0, 0)) {
		syslog(LOG_ERR, "mysql connect failed %s", mysql_error(conn));
		return 0;
	}
	mysql_set_character_set(conn, "utf8");
	return 1;
}

void update_solution_info_from_mysql(int solution_id, int result) {
	char sql[BUFFER_SIZE];
	sprintf(sql, "UPDATE code SET judge_status= %d WHERE code_id=%d", result, solution_id);
	mysql_query(conn, sql);                                                             
}

int main(int argc, char **argv) {
	init_parameters(argc, argv);
	init_conf();
	init_mysql();

	Compilation compile(conn);
	bool is_ce = compile.get_status(solution_id);
	if(is_ce) {
		printf("compile failed\n");
		exit(0);
	}

	printf("compile success\n");

	Judge judge(conn);
	int result = judge.get_status(solution_id);

	printf("result = %d\n", result);

	Deal deal(conn);
	deal.updateMysql(solution_id, result);

	return 0;
}
