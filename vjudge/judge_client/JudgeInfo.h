#ifndef JUDGEINFO_HEADER
#define JUDGEINFO_HEADER
#include <stdio.h>
#include <string>
#include "vjudge.h"

using namespace std;

class JudgeInfo {
	public:
		JudgeInfo(int _oj = HRBUST, int _id = 1000, int _maxWaitTime = 0, int _lang = 0, int _submitTime = 0, string _code = "", int _userId = 0, int _codeId = 0, int _type = 0, int _type_id = 0):oj(_oj), id(_id), maxWaitTime(_maxWaitTime), lang(_lang), submitTime(_submitTime), code(_code), user_id(_userId), code_id(_codeId), type(_type), type_id(_type_id) {}

		virtual ~JudgeInfo() {
		}

		int getMaxWaitTime() const {
			return maxWaitTime;
		}

		void setMaxWaitTime(int maxWaitTime) {
			this->maxWaitTime = maxWaitTime;
		}

		int getId() const {
			return id;
		}

		void setId(int id) {
			this->id = id;
		}

		int getOj() const {
			return oj;
		}

		void setOj(int oj) {
			this->oj = oj;
		}

		void setLang(int lang) {
			this->lang = lang;
		}
		int getLang() {
			return this->lang;
		}
		void setSubmitTime(int submitTime) {
			this->submitTime = submitTime;
		}
		int getSubmitTime() {
			return this->submitTime;
		}
		void setCode(string code) {
			this->code = code;
		}
		string getCode() {
			return this->code;
		}

		void setUserId(int userId) {
			this->user_id = userId;
		}
		int getUserId() {
			return this->user_id;
		}

		void setCodeId(int codeId) {
			this->code_id = codeId;
		}
		int getCodeId() {
			return this->code_id;
		}

		void setType(int type) {
			this->type = type;
		}
		int getType() {
			return this->type;
		}

		void setTypeId(int typeId) {
			this->type_id = typeId;
		}
		int getTypeId() {
			return this->type_id;
		}

	private:
		int codeId;
		int oj;
		int id;
		int maxWaitTime;
		int lang;
		int submitTime;
		string code;

		int user_id;
		int code_id;
		int type;
		int type_id;
};
#endif
