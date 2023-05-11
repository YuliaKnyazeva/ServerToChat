#pragma once
#include <iostream>
#include <mysql.h>

class Database
{
public:
	void Connect();
	void Disconnect();
	void CreateTables();
	void addUserInDb(const std::string& login, const std::string& password, const std::string& name);
	void addMessageInDb(const std::string& from, const std::string& to, const std::string& text);
	bool checkLogin(const std::string& login);
	bool checkUser(const std::string& login, const std::string& password);
	std::string showUsers();
	std::string showMessages(const std::string& login);
	std::string GetName(const std::string& login, const std::string& password);
private:
	MYSQL _mysql{ nullptr };
	
};

