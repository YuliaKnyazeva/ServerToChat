#include <fstream>
#include <iostream>
#include <shared_mutex>
#include <string>
#pragma once
class Logger
{	
	std::fstream _stream{ "log.txt", std::ios::in | std::ios::out };
	std::shared_mutex _mutex;
public:
	Logger();
	~Logger();
	void WriteToLog(const std::string& login, const std::string& text);
	void ReadFromLog();
};

