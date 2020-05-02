#pragma once

#ifdef _DEBUG
class Log
{
public:
	static void print(const char* status, const char* msg);
	static void log(const char* msg);
	static void err(const char* error);
};
#endif

