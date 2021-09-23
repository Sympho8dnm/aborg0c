#pragma once
#include <time.h>
#include "help_ex.h"

// 日期框属性_状态
#define DBL_STATE 0

struct datebox_s
{
	HEXOBJ hObj;
	INT nProcessTime;
	HEXFONT hFont;
	INT Year;				//年
	INT Mon;				//月
	INT Mday;				//日
	INT Wday;				//星期
	INT nCalendar;			//是否显示农历
	INT nSohwType;			//显示类型
	INT lpYear;				//临时年
	INT lpMon;				//临时月
	LPVOID Items;			//数据
};



/*
 * @brief 获取调整的时间戳，即0表示当前
 * @param timestamp 0表示会返回当前时间戳，否则原样返回
 * @return 调整过的时间戳
 */
inline time_t get_fixed_time(time_t timestamp) {
	return timestamp == 0 ? time(0) : timestamp;
}


/*
 * @brief 获取当前日期的天
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return 当前日期的天，1-31
 */
inline int64_t get_day(time_t timestamp = 0) {
	struct tm ptm;
	timestamp = get_fixed_time(timestamp);
	localtime_s(&ptm, &timestamp);
	return ptm.tm_mday;
}

/*
 * @brief 获取当前日期的星期数
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return 当前日期的星期数，0-6，周日为0
 */
inline int64_t get_weekday(time_t timestamp = 0) {
	struct tm ptm;
	timestamp = get_fixed_time(timestamp);
	localtime_s(&ptm, &timestamp);
	return ptm.tm_wday;
}

/*
 * @brief 获取当前日期的月数
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return 当前日期的月数，1-12
 */
inline int64_t get_month(time_t timestamp = 0) {
	struct tm ptm;
	timestamp = get_fixed_time(timestamp);
	localtime_s(&ptm, &timestamp);
	return (int64_t)ptm.tm_mon + 1;
}

/*
 * @brief 获取当前日期的年份
 * @param timestamp 要获取的时间戳，0表示当前时间
 * @return 当前日期的年份
 */
inline int64_t get_year(time_t timestamp = 0) {
	struct tm ptm;
	timestamp = get_fixed_time(timestamp);
	localtime_s(&ptm, &timestamp);
	return (int64_t)ptm.tm_year + 1900;
}

void _datebox_register();
time_t _datebox_gettimestamp();
void _datebox_settitle(HEXOBJ hObj, datebox_s* pOwner, int type);
void _datebox_setstatic(HEXOBJ hObj, datebox_s* pOwner);
void _datebox_init(HEXOBJ hObj, int nYear, int nMon);
void _datebox_settime(datebox_s* pOwner, int type, int index, int year, int mon, int Mday, int Wday);
void _datebox_show(HEXOBJ hObj, datebox_s* pOwner, int type);
LRESULT CALLBACK _datebox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK _datebox_onwndmsgproc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK _datebox_onbuttonproc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK _datebox_onlistproc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);