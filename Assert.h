#pragma once
#ifndef _NSTD_ASSERT
#include "Defines.h"

_NSTD_BEGIN
class _ASS_STR_MAN {
private:
	char* str_;
	int len_;
public:
	_ASS_STR_MAN(int len) {
		str_ = new char[len];
		_NSTD_FOR_I(len)
			str_[i] = '\0';
	}
	~_ASS_STR_MAN() {
		delete[] str_;
	}
	_NODISCARD char*& get() {
		return str_;
	}
	void set_last(char c) {
		last() = c;
	}
	_NODISCARD char& last() {
		return str_[len_ - 1];
	}
	_NODISCARD char& operator[] (const int& i) {
		if (i <= len_)
			return str_[i];
	}
};
int c_str_len(const char*& c_str) {
	int out = 0;
	for (char* ptr = const_cast<char*> (c_str); *ptr != '\0'; ptr++)
		out++;
	return out;
}
int int_len(const int& in) {
	int out = 0;
	for (int i = in; i; i /= 10)
		out++;
	return out;
}
int int_list_sum(const int*& arr, const int& len) {
	int out = 0;
	_NSTD_FOR_I(len)
		out += arr[i];
	return out;
}
void copy_to_out(_NSTD _ASS_STR_MAN s_arr, const char*& arr, const int& len, int& cashe) {
	_NSTD_FOR_I(len)
		s_arr[cashe + i] = arr[i];
	cashe += len;
}
_NSTD_END

_NODISCARD _NSTD _ASS_STR_MAN assert_str(const char*&& msg, const char*&& expr, const int&& line, const char*&& file) {
	// NStandard Assert (NSTDA) string components
	static const char
		* err = "ERROR: ",
		* fr = " | FROM: ",
		* onl = " | ON LINE: ",
		* inf = " | IN FILE: ";
	// Genning lengths in case NSTDA string components are updated
	static const int
		err_len = _NSTD c_str_len(err),
		fr_len = _NSTD c_str_len(fr),
		onl_len = _NSTD c_str_len(onl),
		inf_len = _NSTD c_str_len(inf);

	const int num_args = 8; // Must be manually updated
	// Genning lengths of args
	int msg_len = _NSTD c_str_len(msg),
		expr_len = _NSTD c_str_len(expr),
		file_len = _NSTD c_str_len(file),
		line_len = _NSTD int_len(line);

	// static_cast<char*> (line);
	_NSTD _ASS_STR_MAN line_arr(line_len);

	char num_to_str_str[] = _NSTD_ITSL;

	for (int i = 0, l = line; i < line_len; i++) {
		line_arr[i] = num_to_str_str[l % 10];
		l /= 10;
	}
	char* c_l = line_arr.get();

	const char* str_args[num_args] = { err, msg, fr, expr, onl, c_l, inf, file };
	const int   args_len[num_args] = { err_len, msg_len, fr_len, expr_len, onl_len, line_len, inf_len, file_len };

	int cashe = 0;
	_NSTD _ASS_STR_MAN out(err_len + msg_len + fr_len + expr_len + onl_len + line_len + inf_len + file_len + 1);
	_NSTD_FOR_I(num_args)
		_NSTD copy_to_out(out, str_args[i], args_len[i], cashe);
	out.last() = '\0';

	return out;
}
// Structure: "ERROR: " msg " | FROM: " #expr " | ON LINE: __LINE__ | IN FILE: __FILE__ "
#define _NSTD_ASSERT(expr, msg) static_assert(!!(expr), assert_str(msg, #expr, __LINE__, __FILE__).get())
#endif // ifndef _NSTD_ASSERT