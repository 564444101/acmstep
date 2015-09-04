#ifndef GLOBAL_HEADER
#define GLOBAL_HEADER
#include <vector>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <algorithm>
#include <re2/re2.h>
#include <curl/curl.h>
#include <mysql/mysql.h>
#include <string>
#include <syslog.h>
#include <iconv.h>
extern "C" {
#include "entities.h"
}

using namespace std;

char dec2hexChar(short int n);
string escapeURL(const string &URL);
string fileToString(string filename);
string fileToString2(char *path);
void charset_convert(const char *, const char *, char *, size_t, char *, size_t);
vector<string> split(string, char);
string trim(string str);
string toLowerCase(string str);
extern "C" size_t decode_html_entities_utf8(char *dest, const char *src);
#endif
