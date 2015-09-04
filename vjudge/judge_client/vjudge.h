#ifndef _COMMON_H_
#define _COMMON_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <re2/re2.h>
#include <syslog.h>
#include <mysql/mysql.h>

#include "global.h"

#define HRBUST 1
#define HDU 2
#define POJ 3

#define HOST_NAME "localhost"
#define USER_NAME "root"
#define PASSWORD "vj123456"
#define DB_NAME "acmstep"

#define BUFFER_SIZE 1024

#define TOPIC 0
#define LADDER 1

#define OJ_WT0 0
#define OJ_WT1 1
#define OJ_CI 2
#define OJ_RI 3
#define OJ_AC 4
#define OJ_PE 5
#define OJ_WA 6
#define OJ_TL 7
#define OJ_ML 8
#define OJ_OL 9
#define OJ_RE 10
#define OJ_CE 11

#endif
