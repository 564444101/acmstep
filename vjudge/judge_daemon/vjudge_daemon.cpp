#include<unistd.h>
#include<stdlib.h>
#include<iostream>
#include<fcntl.h>
#include<syslog.h>
#include<string.h>
#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<errno.h>
#include<mysql/mysql.h>
#include<sys/wait.h>
#include<queue>
#include<pthread.h>
#include<sys/resource.h>
#include<vector>
#include<algorithm>
#include<cmath>
using namespace std;

#define LOCKFILE "/var/run/vjudge_daemon.pid"
#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) 

const int BUFFER_SIZE = 1024;
const int STD_MB = 1048576;

static char host_name[BUFFER_SIZE];
static char user_name[BUFFER_SIZE];
static char password[BUFFER_SIZE];
static char db_name[BUFFER_SIZE];
static int port_number;
static int max_client;

queue<int>jobs_queue;
pthread_mutex_t queuelock = PTHREAD_MUTEX_INITIALIZER;

void write_log(char *s) {
}

/*init daemon*/
int daemon_init() {
	umask(0);
	pid_t pid;
	if((pid = fork()) < 0) {
		return -1;
	}
	else if(pid != 0) exit(0);
	setsid();
	chdir("/");
	struct rlimit rl;
	getrlimit(RLIMIT_NOFILE, &rl);
	for(int i = 0; i < rl.rlim_max; ++i) {
		close(i);
	}
	int fd0 = open("/dev/null", O_RDWR);
	return 1;
}

int lockfile(int fd) {
	struct flock fl;
	fl.l_type = F_WRLCK;
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0;
	return (fcntl(fd, F_SETLK, &fl));
}

/*lockfile*/
int already_running(void) {
	int fd;
	char buf[16];
	fd = open(LOCKFILE, O_RDWR | O_CREAT, LOCKMODE);
	if(fd < 0)  {
		syslog(LOG_ERR, "can't open %s: %s", LOCKFILE, strerror(errno));
		exit(1);
	}
	if(lockfile(fd) < 0) {
		if(errno == EACCES || errno == EAGAIN) {
			close(fd);
			return 1;
		}
		syslog(LOG_ERR, "can't open %s: %s", LOCKFILE, strerror(errno));
		exit(1);
	}
	ftruncate(fd, 0);
	sprintf(buf, "%d", getpid());
	write(fd, buf, strlen(buf));
	return 0;
}

void init_conf() {
	strcpy(host_name, "localhost");
	strcpy(user_name, "root");
	strcpy(password, "vj123456");
	strcpy(db_name, "acmstep");
	port_number = 3306;
	max_client = 4;
}

MYSQL *init_mysql() {
	MYSQL *conn = mysql_init(NULL);
	if(!mysql_real_connect(conn, host_name, user_name, password, db_name, port_number, 0, 0)) {
		syslog(LOG_ERR, "mysql connect failed %s", mysql_error(conn));
		return NULL;
	}
	return conn;
}


void *get_jobs_from_mysql(void *arg) {

	MYSQL *conn = init_mysql();
	if(NULL == conn) return NULL;

	MYSQL_RES *res;
	MYSQL_ROW row;
	while(1) {
		char sql[BUFFER_SIZE];
		sprintf(sql, "%s", "select code_id from `code` where judge_status = 0 and is_inner = 0 limit 5");
		mysql_query(conn, sql);
		res = mysql_store_result(conn);
		while((row = mysql_fetch_row(res)) != NULL) {
			pthread_mutex_lock(&queuelock);
			jobs_queue.push(atoi(row[0]));
			pthread_mutex_unlock(&queuelock);
			char query[BUFFER_SIZE];
			printf("%d\n", atoi(row[0]));
			sprintf(query, "UPDATE `code` SET `judge_status`= 1 WHERE code_id = %d", atoi(row[0]));
			mysql_query(conn, query);
		}
		sleep(3);
	}
}

void run_client(int client_id, int solution_id) {

	syslog(LOG_ERR, "%d%d", client_id, solution_id);
	char cid[BUFFER_SIZE];
	char sid[BUFFER_SIZE];
	sprintf(cid, "%d", client_id);
	sprintf(sid, "%d", solution_id);

	execl("/usr/bin/vjudge_client", "/usr/bin/vjudge_client", cid, sid, (char*)NULL);
}

void work() {
	int workcnt = 0;
	int jobs[BUFFER_SIZE];
	memset(jobs, 0, sizeof(jobs));
	while(1) {
		if(jobs_queue.empty()) {
			sleep(1);
			continue;
		}
		pthread_mutex_lock(&queuelock);
		int solution_id = jobs_queue.front();
		jobs_queue.pop();
		pthread_mutex_unlock(&queuelock);
		if(workcnt < max_client) {
			int cid;
			for(cid = 0; jobs[cid]; ++cid); 
			pid_t pid;
			if((pid = fork()) == 0) {
				run_client(cid, solution_id);
				exit(0);
			}
			workcnt++;
			jobs[cid] = pid;
		}
		else {
			pid_t oldpid = waitpid(-1, NULL, 0);
			int cid;
			for(cid = 0; jobs[cid] != oldpid; ++cid);
			pid_t newpid;
			if((newpid = fork()) == 0) {
				run_client(cid, solution_id);
				exit(0);
			}
			jobs[cid] = newpid;
		}
	}
}

int main(int argc, char **argv) {
	/*if(daemon_init() == -1) {
		syslog(LOG_ERR, "daemon init failed");
		exit(1);
	}
	if(already_running()) {
		syslog(LOG_ERR, "daemon alreadyrunning");
		exit(1);
	}*/
	cout << "vjudge_daemon start...." << endl;
	init_conf();
	cout << "init start........" << endl;
	init_mysql();
	cout << "init mysql start......" << endl;
	pthread_t tid1;
	pthread_create(&tid1, NULL, get_jobs_from_mysql, 0);
	work();
	cout << "work start........" << endl;
	return 0;
}
