#pragma once

#include "ThostFtdcTraderApi.h"
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
	
private:
	CThostFtdcTraderApi* m_ptrTraderApi;

	std::chrono::time_point<std::chrono::system_clock> now;

	int rowIndex = 1;
	char *brokerId = "8000";
	char *userId = "70801517";
	char *pwd = "287122";
};

