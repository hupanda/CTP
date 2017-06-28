#pragma once
#include "ConfigManager.h"
#include "ThostFtdcTraderApi.h"

//STL reference
#include <iostream>
#include <string>
#include <windows.h>
#include <chrono>
#include <ctime>
using namespace std;

//MYSQL reference
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class DbConnector
{
public:
	int init(ConfigManager* instance);
	int insert(CThostFtdcTradeField *pTrade);
	sql::Driver *driver;
	sql::PreparedStatement *pstmt;
	sql::Connection *con;
};

