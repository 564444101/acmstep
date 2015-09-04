#include "global.h"
#include <fstream>
#include <sstream>
char dec2hexChar(short int n) {
	if (0 <= n && n <= 9) return char( short('0') + n);
	else if (10 <= n && n <= 15)return char( short('A') + n - 10);
	else return char(0);
}

/**
 * URL escape a string
 * @param URL   Orignal string
 * @return URL escaped string
 */
string escapeURL(const string &URL) {
	string result = "";
	for (unsigned int i = 0; i < URL.size(); i++) {
		char c = URL[i];
		if (
				('0' <= c && c <= '9') ||
				('a' <= c && c <= 'z') ||
				('A' <= c && c <= 'Z') ||
				c == '/' || c == '.'
		   ) result += c;
		else {
			int j = (short int) c;
			if (j < 0) j += 256;
			int i1, i0;
			i1 = j / 16;
			i0 = j - i1 * 16;
			result += '%';
			result += dec2hexChar(i1);
			result += dec2hexChar(i0);
		}
	}
	return result;
}

string fileToString(string filename)
{	    
	int tried = 0;
    string res="", tmps;
	ifstream fin;

	fin.open(filename.c_str(), ios::in);

	/*
	while (fin.fail() && tried++ < 10)
		fin.open(filename.c_str(), ios::in);
		*/

	if (fin.fail()){
		fin.close();
		return res;
	}

	fin.clear(ios::goodbit);
	//getline(fin, res);

	while (getline(fin, tmps)) {
		if (res != "") res += "\n";
		res += tmps;
		if (fin.eof()) break;
	}

	res += '\0';
	fin.close();
	return res;
}

string fileToString2(char* filename)
{
	ifstream ifile(filename);
	//将文件读入到ostringstream对象buf中
	ostringstream buf;
	char ch;
	while(buf&&ifile.get(ch))
	buf.put(ch);
	////返回与流对象buf关联的字符串
	return buf.str();
}

vector<string> split(string str, char c) {
	int sz = str.size();
	vector<string> result;
	string tmp;
	for (int i = 0; i <= sz; ++i) {
		if (i == sz || str[i] == c) {
			if (tmp.size() > 0) {
				result.push_back(tmp);
			}
			tmp.clear();
		} else tmp += str[i];
	}
	return result;
}

string trim(string str) {
	string spaces = " \t\n\r";
	size_t start = str.find_first_not_of(spaces);
	size_t end = str.find_last_not_of(spaces);
	if (start == string::npos || end == string::npos) {
		return str;
	}
	return str.substr(start, end - start + 1);
}

string toLowerCase(string str) {
	    for (size_t i = 0; i < str.length(); ++i) str[i] = tolower(str[i]);
		    return str;
}

void charset_convert(const char * from_charset, const char * to_charset, char * inbuf, size_t inlen, char * outbuf, size_t outlen) {
	iconv_t cd;
	char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset, from_charset);
	if (cd == 0) {
		syslog(LOG_ERR, "Invalid charset conversion");
	}
	memset(outbuf, 0, outlen);
	if (iconv(cd, pin, &inlen, pout, &outlen) == -1) {
		syslog(LOG_ERR, "Charset conversion Failed");
	}
	iconv_close(cd);
}

