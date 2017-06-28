#include "DbConnector.h"

int DbConnector::init(ConfigManager* instance){
	try
	{
		driver = get_driver_instance();
		con = driver->connect(instance->dbHost, instance->dbUserId,instance->dbPassword);
		con->setSchema(instance->dbSchema);
		string statement = "INSERT INTO trades (OrderSysID, ExchangeInstID, OffsetFlag, Side, Price, Size, TradeTime, TradeDate, TradeID) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
		pstmt = con->prepareStatement(statement);
	}
	catch (std::exception &e)
	{
		cerr << "Exception: " << e.what() << endl;
		return -1;
	}
}

int DbConnector::insert(CThostFtdcTradeField *pTrade) {
	pstmt->setInt(1, stoi(pTrade->OrderSysID));
	pstmt->setString(2, pTrade->ExchangeInstID);
	pstmt->setString(3, to_string(pTrade->OffsetFlag));
	pstmt->setString(4, pTrade->Direction == '0' ? "B" : "S");
	pstmt->setDouble(5, pTrade->Price);
	pstmt->setInt(6, pTrade->Volume);
	string date = pTrade->TradeDate;
	string dateFormatted = date.substr(0, 4) + "-" + date.substr(4, 2) + "-" + date.substr(6, 2);
	string dateTime = dateFormatted + " " + pTrade->TradeTime;
	pstmt->setDateTime(7, dateTime);
	pstmt->setDateTime(8, dateFormatted);
	pstmt->setInt(9, stoi(pTrade->TradeID));
	pstmt->execute();
}