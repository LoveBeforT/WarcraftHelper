#pragma once
#include<windows.h>

TCHAR IniFile[MAX_PATH] = { 0 };
/*
//win.ini
BOOL WriteProfileString（LPCTSTR lpAppName, // section name
	   LPCTSTR lpKeyName, // key name
	   LPCTSTR lpString // string to write
);
//ini文件写入
BOOL WritePrivateProfileString(
	LPCTSTR lpAppName,  // INI文件中的一个字段名[节名]可以有很多个节名
	LPCTSTR lpKeyName,  // lpAppName 下的一个键名，也就是里面具体的变量名
	LPCTSTR lpString,   // 键值,也就是数据
	LPCTSTR lpFileName  // INI文件的路径
);

//读取
//数据项-string
DWORD GetPrivateProfileString(
	LPCTSTR lpAppName,        // INI文件中的一个字段名[节名]可以有很多个节名
	LPCTSTR lpKeyName,        // lpAppName 下的一个键名，也就是里面具体的变量名
	LPCTSTR lpDefault,        // 如果lpReturnedString为空,则把个变量赋给lpReturnedString
	LPTSTR lpReturnedString,  // 存放键值的指针变量,用于接收INI文件中键值(数据)的接收缓冲区
	DWORD nSize,            // lpReturnedString的缓冲区大小
	LPCTSTR lpFileName        // INI文件的路径
);

//数据项-int
UINT GetPrivateProfileInt(
	LPCTSTR lpAppName,  // INI文件中的一个字段名[节名]可以有很多个节名
	LPCTSTR lpKeyName,  // lpAppName 下的一个键名，也就是里面具体的变量名
	INT nDefault,       // 如果没有找到指定的数据返回,则把个变量值赋给返回值
	LPCTSTR lpFileName  // INI文件的路径
);
*/