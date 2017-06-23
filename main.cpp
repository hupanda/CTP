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

int main()
{

  HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);   
  if (FAILED(hr))   
  {  
    std::cout << "Failed to initialize COM library. Error code = 0x"
              << std::hex << hr << std::endl;   
    return hr;  
  }
               
  try
  {
	  const std::string tdflowpathstring = "C:/Users/hupanda/CTP/tdflow";
	  const char * tdflowpath = tdflowpathstring.c_str();
	  CThostFtdcTraderApi* api = CThostFtdcTraderApi::CreateFtdcTraderApi(tdflowpath);
	  CtpTraderSpi spi;
	  spi.set_m_ptrTraderApi(api);
	  api->RegisterSpi(&spi);
	  api->RegisterFront("tcp://58.246.171.3:52101");
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

