#define _SCL_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

//STL reference
#include <iostream>
#include <string>
#include <windows.h>

//CTP reference
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include "CtpTraderSpi.h"
#include "ConfigManager.h"
#include "DbConnector.h"

using namespace std;

int main()
{

  HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);   
  if (FAILED(hr))   
  {  
    cout << "Failed to initialize COM library. Error code = 0x" << hex << hr << endl;   
    return hr;  
  }
  ConfigManager config;
  if (config.init() < 0)
  {
	  cout << "Failed to initialize configuration. " << endl;
	  return 0;
  }

  DbConnector conn;
  if (conn.init(&config) < 0)
  {
	  cout << "Failed to initialize db connection. " << endl;
	  return 0;
  }

  try
  {
	  //const std::string tdflowpathstring = "C:/Users/hupanda/CTP/tdflow";
	  const char * tdflowpath = config.flowPath.c_str();
	  CThostFtdcTraderApi* api = CThostFtdcTraderApi::CreateFtdcTraderApi(tdflowpath);
	  CtpTraderSpi spi;
	  spi.set_m_ptrTraderApi(api);
	  spi.set_m_config(&config);
	  spi.set_m_conn(&conn);
	  api->RegisterSpi(&spi);
	  //api->RegisterFront("tcp://58.246.171.3:52101");
	  api->RegisterFront((char *)config.brokerHost.c_str());
	  api->SubscribePrivateTopic(THOST_TERT_RESTART);
	  api->SubscribePublicTopic(THOST_TERT_RESTART);
	  api->Init();
	  api->Join();
  }
  catch (...)
  {
	  CoUninitialize();
  }
  return 0;
}

