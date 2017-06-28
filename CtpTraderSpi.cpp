#include "CtpTraderSpi.h"
#include <iostream>
#include <thread>
#include "windows.h"
#include <chrono>
#include <ctime>
#include <fstream>
using namespace std;

const int COL_OFFSET = 43;
const int ROW_OFFSET = 0;

void CtpTraderSpi::set_m_ptrTraderApi(CThostFtdcTraderApi* instance)
{
	m_ptrTraderApi = instance;
}

void CtpTraderSpi::set_m_config(ConfigManager* instance)
{
	m_config = instance;
}

void CtpTraderSpi::set_m_conn(DbConnector* instance)
{
	m_conn = instance;
}

void CtpTraderSpi::OnFrontConnected()
{
	try
	{
		cerr << "CTPTrader: connected ." << endl;
		CThostFtdcReqUserLoginField reqUserLogin;
		strcpy_s(reqUserLogin.BrokerID, this->m_config->brokerId.c_str());
		strcpy_s(reqUserLogin.UserID, this->m_config->brokerUserId.c_str());
		strcpy_s(reqUserLogin.Password, this->m_config->brokerPassword.c_str());
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
		this->m_conn->insert(pTrade);
		++rowIndex;
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