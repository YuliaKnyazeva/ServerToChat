#include "Logger.h"


Logger::Logger()
{
	if (!_stream)
		_stream = std::fstream("log.txt", std::ios::in | std::ios::out | std::ios::trunc);
	if (!_stream.is_open()) {
		std::cout << "Could not open input file!" << '\n';
		exit(1);
	}
}

Logger::~Logger()
{
	_stream.close();
}

void Logger::WriteToLog(const std::string& login, const std::string& text)
{
	_mutex.lock();
	_stream << login << " " << text <<'\n';
	_mutex.unlock();
}

void Logger::ReadFromLog()
{
	std::string line;
	if (_stream.is_open()){
		while(std::getline(_stream, line))
		std::cout << line <<'\n';
	}
}
