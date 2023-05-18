#include <winsock2.h>
#include "Database.h"
#include <iostream>
#include "Logger.h"
#define PORT 7777
#pragma comment (lib, "Ws2_32.lib")
#pragma warning(disable:4996)

int main(int argc, char* argv[])
{
	Database db;
	db.Connect();
	std::string login, password, name, from,to, text, temp;
	char buf[1024];
	WSADATA wsaData;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);
	if (newConnection == 0) {
		std::cout << "Error connection";
		exit(1);
	}
	else
	{
		std::cout << "Client connected" << std::endl;
	}
	Logger logger;
	logger.ReadFromLog();
	while(true) {
		recv(newConnection, buf, sizeof(buf), 0); // Принимаем цифру из меню клиентской версии 1 - регистрация, 2 - авторизация Check user in db
		int menu = std::atoi(buf);
		switch (menu)
		{
			case 0:
				closesocket(sListen);
				WSACleanup();
				break;
			case 1: 
			std::cout <<"Registration. Waiting login" << std::endl;
			recv(newConnection, buf, sizeof(buf), 0);
			login = buf;
			std::cout << "Login - " << login << std::endl;
				if(db.checkLogin(login)){
					std::cout << "Login free" << std::endl;
					send(newConnection, "1", sizeof("1"), 0);
					std::cout << "Waiting password" << std::endl;
					recv(newConnection, buf, sizeof(buf), 0);
					password = buf;
					std::cout << "Password - " << password << std::endl;
					recv(newConnection, buf, sizeof(buf), 0);
					name = buf;
					std::cout << "Name - " << name << std::endl;
					db.addUserInDb(login, password, name);
					}
				else
					{
					std::cout << "Login used" << std::endl;
					send(newConnection, "2", sizeof("2"), 0);
					}
				break;
			case 2:
				std::cout << "Autorization" << std::endl;
				recv(newConnection, buf, sizeof(buf), 0);
				login = buf;
				recv(newConnection, buf, sizeof(buf), 0);
				password = buf;
				if (db.checkUser(login, password))
				{
					std::cout << "User " << login<< " is online" << std::endl;
					std::string temp = db.GetName(login, password);
					send(newConnection, "1", sizeof("1"), 0);
					send(newConnection, temp.c_str(), sizeof(temp), 0);
				}
				else {
					send(newConnection, "2", sizeof("2"), 0);
				}
				break;
			case 21:
				std::cout << "User want to see incoming messages. Waiting login" << std::endl;
				recv(newConnection, buf, sizeof(buf), 0);
				from = buf;
				std::cout << buf << std::endl;
				temp = db.showMessages(from);
				std::cout << temp << std::endl;
				send(newConnection, temp.c_str(), sizeof(temp), 0);
			case 22:                                                               
				std::cout << "Waiting message" << std::endl;
				recv(newConnection, buf, sizeof(buf), 0);
				from = buf;
				recv(newConnection, buf, sizeof(buf), 0);
				to = buf;
				recv(newConnection, buf, sizeof(buf), 0);
				text = buf;
				db.addMessageInDb(from, to, text);
				logger.WriteToLog(from, text);
				std::cout << "New message add in database" << std::endl;
				break;
			case 23:
				send(newConnection, db.showUsers().c_str(), sizeof(db.showUsers()), 0);
		}
	}

	system("pause");
	db.Disconnect();
	return 0;
}