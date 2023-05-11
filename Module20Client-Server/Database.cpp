#include "Database.h"

void Database::Connect()
{
	mysql_init(&_mysql);
	if (&_mysql == nullptr) {
		// Если дескриптор не получен — выводим сообщение об ошибке
		std::cout << "Error: can't create MySQL-descriptor" << std::endl;
	}
	// Подключаемся к серверу
	if (!mysql_real_connect(&_mysql, "localhost", "root", "poison", "testdb", NULL, NULL, 0)) {
		// Если нет возможности установить соединение с БД выводим сообщение об ошибке
		std::cout << "Error: can't connect to database " << mysql_error(&_mysql) << std::endl;
	}
	else {
		// Если соединение успешно установлено выводим фразу — "Success!"
		std::cout << "Success!" << std::endl;
	}
}

void Database::Disconnect()
{
	mysql_close(&_mysql);
}

void Database::CreateTables() // Создание таблиц для теста
{
	mysql_query(&_mysql, "CREATE TABLE IF NOT EXISTS users(id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(255), login VARCHAR(255), password VARCHAR(255))");
	mysql_query(&_mysql, "INSERT INTO users(id, name, login, password) values(default, 'Daria', 'Daria', '123')");
	mysql_query(&_mysql, "INSERT INTO users(id, name, login, password) values(default, 'Lida', 'Lida', '234')");
	mysql_query(&_mysql, "INSERT INTO users(id, name, login, password) values(default, 'Nastya', 'Nastya', '345')");
	mysql_query(&_mysql, "CREATE TABLE IF NOT EXISTS messages(id INT AUTO_INCREMENT PRIMARY KEY, from_id VARCHAR(255), to_id VARCHAR(255), text VARCHAR(255))");
	mysql_query(&_mysql, "INSERT INTO messages(id, from_id, to_id, text) values(default, 'Lida', 'Daria', 'Hi!')");
	mysql_query(&_mysql, "INSERT INTO messages(id, from_id, to_id, text) values(default, 'Daria', 'all', 'Hi all!')");
}

void Database::addUserInDb(const std::string& login, const std::string& password, const std::string& name) // Добавить нового пользователя в БД
{
	std::string temp;
	temp = "INSERT INTO users (id, name, login, password) VALUES (default,' " + name + "','" + login + "','" + password + "')";
	mysql_query(&_mysql, temp.c_str());
}

void Database::addMessageInDb(const std::string& from, const std::string& to, const std::string& text) // Добавить новое сообщение в БД
{
	std::string temp;
	temp = "INSERT INTO messages (id, from_id, to_id, text) VALUES (default,' " + from + "','" + to + "','" + text + "')";
	mysql_query(&_mysql, temp.c_str());
}

bool Database::checkLogin(const std::string& login) // Проверка, свободен ли логин
{
	MYSQL_RES* res;
	MYSQL_ROW row;
	std::string temp = "SELECT * FROM users WHERE login='" + login + "'";
	mysql_query(&_mysql, temp.c_str());
	res = mysql_store_result(&_mysql);
	row = mysql_fetch_row(res);
	if (row==NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Database::checkUser(const std::string& login, const std::string& password) // Проверка авторизации
{
	MYSQL_RES* res;
	MYSQL_ROW row;
	int i = 0;
	std::string temp = "SELECT name FROM users WHERE login='" + login + "' and password='" + password + "'";
	mysql_query(&_mysql, temp.c_str());
	res = mysql_store_result(&_mysql);
	row = mysql_fetch_row(res);
	if (row == NULL) {
		return false;
	}
	else {
		return true;
	}
}
std::string Database::GetName(const std::string& login, const std::string& password)
{
	std::string name;
	MYSQL_RES* res;
	MYSQL_ROW row;
	int i = 0;
	std::string temp = "SELECT name FROM users WHERE login='" + login + "' and password='" + password + "'";
	mysql_query(&_mysql, temp.c_str());
	res = mysql_store_result(&_mysql);
	row = mysql_fetch_row(res);
	return name = row[0];
}

std::string Database::showUsers() // Показать зарегистрированных пользователей
{
	std::string temp;
	MYSQL_RES* res;
	MYSQL_ROW row;
	int i = 0;
	mysql_query(&_mysql, "SELECT name FROM users");
	if (res = mysql_store_result(&_mysql)) {
		while (row = mysql_fetch_row(res)) {
			for (i = 0; i < mysql_num_fields(res); i++) {
				temp += row[i];
				temp += ' ';
			}
		}
		return temp;
	}
	else
		std::cout << "Ошибка MySql номер " << mysql_error(&_mysql);

}

std::string Database::showMessages(const std::string& login) //Показать входящие сообщения
{
	std::string temp="From ";
	MYSQL_RES* res;
	MYSQL_ROW row;
	int i = 0;
	std::string sql = "SELECT from_id, text FROM messages WHERE to_id='all' OR to_id='" + login + "'";
	mysql_query(&_mysql, sql.c_str());
	if (res = mysql_store_result(&_mysql)) {
		while (row = mysql_fetch_row(res)) {
			for (i = 0; i < mysql_num_fields(res); i++) {

				temp +=row[i];
				temp += ' ';
			}
		}
		std::cout << temp;
		return temp;
	}
	else
		std::cout << "No incoming messages " << mysql_error(&_mysql);
}