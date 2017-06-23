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
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
using namespace std;

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

