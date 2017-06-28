#pragma once

#include "ThostFtdcTraderApi.h"
#include "ConfigManager.h"
#include "DbConnector.h"
#include <string>
#include <chrono>
#include <ctime>


class CtpTraderSpi :public CThostFtdcTraderSpi
{
public:
	CtpTraderSpi() {}
	virtual void OnFrontConnected();
	virtual void OnFrontDisConnected(int nReason);
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void set_m_ptrTraderApi(CThostFtdcTraderApi* instance);
	void set_m_config(ConfigManager* instance);
	void set_m_conn(DbConnector* instance);
	
private:
	CThostFtdcTraderApi* m_ptrTraderApi;
	ConfigManager* m_config;
	DbConnector* m_conn;
	std::chrono::time_point<std::chrono::system_clock> now;
	int rowIndex = 1;
};

