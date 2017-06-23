#include "CtpTraderSpi.h"
#include <iostream>
#include <thread>
#include "windows.h"
#include <chrono>
#include <ctime>
#include <fstream>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
using namespace std;

const int COL_OFFSET = 43;
const int ROW_OFFSET = 0;

void CtpTraderSpi::set_m_ptrTraderApi(CThostFtdcTraderApi* instance)
{
	m_ptrTraderApi = instance;
}

void CtpTraderSpi::OnFrontConnected()
{
	try
	{
		cerr << "CTPTrader: connected ." << endl;
		CThostFtdcReqUserLoginField reqUserLogin;
		strcpy_s(reqUserLogin.BrokerID, brokerId);
		strcpy_s(reqUserLogin.UserID, userId);
		strcpy_s(reqUserLogin.Password, pwd);
        m_ptrTraderApi->ReqUserLogin(&reqUserLogin, 0);
		rowIndex = 1;
	}
	catch (std::exception &e)
	{
		cerr << "CTPTraderSpi: exception: " << e.what() << endl;
	}
}

void CtpTraderSpi::OnFrontDisConnected(int nReason)
{
	cerr << "CTPTraderSpi: disconnected ." << endl;
}

void CtpTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "CTPTraderSpi: OnRspUserLogin. Error id:" << pRspInfo->ErrorID << " . CTPTrader: error message: " << pRspInfo->ErrorMsg << endl;
	if (pRspInfo->ErrorID != 0)
	{
		exit(-1);
	}
}

void CtpTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	try
	{
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement *pstmt;
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "admin", "admin");
		con->setSchema("ctp");
		string statement = "INSERT INTO trades (OrderSysID, ExchangeInstID, OffsetFlag, Side, Price, Size, TradeTime, TradeDate, TradeID) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
		pstmt = con->prepareStatement(statement);
		pstmt->setInt(1, 1);
		pstmt->setString(2, "a");
		pstmt->setString(3, "a");
		pstmt->setString(4, "a");
		pstmt->setDouble(5, 1.1);
		pstmt->setInt(6, 1);
		pstmt->setDateTime(7, "2017-06-22 22:22:22");
		pstmt->setDateTime(8, "2017-06-22");
		pstmt->setInt(9, 3);
		pstmt->execute();
		delete pstmt;
		delete con;
		++rowIndex;
		/*
		trades.append(pTrade->OrderSysID).append(",");
		trades.append(pTrade->ExchangeInstID).append(",");
		trades.append(std::to_string(pTrade->OffsetFlag)).append(",");
		trades.append(pTrade->Direction == '0' ? "B" : "S").append(",");
		trades.append(std::to_string(pTrade->Price)).append(",");
		trades.append(std::to_string(pTrade->Volume)).append(",");
		trades.append(pTrade->TradeTime).append(",");
		trades.append(pTrade->TradeDate).append(",");
		trades.append(pTrade->TradeID).append("\n");
*/
		

	}
	catch (std::exception &e)
	{
		cerr << "CTPTraderSpi: OnRtnTrade(): exception: " << e.what() << endl;
	}
}

void CtpTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	cerr << "error id:" << pRspInfo->ErrorID << endl;
	cerr << "error message:" << pRspInfo->ErrorMsg << endl;
}