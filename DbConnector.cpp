#include "DbConnector.h"

int DbConnector::init(ConfigManager* instance){
	try
	{
		driver = get_driver_instance();
		con = driver->connect(instance->dbHost, instance->dbUserId,instance->dbPassword);
		con->setSchema(instance->dbSchema);
		insertStmt = con->prepareStatement("INSERT INTO trades (OrderSysID, ExchangeInstID, OffsetFlag, Side, Price, Size, TradeTime, TradeDate, TradeID) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
		queryStmt = con->prepareStatement("SELECT * FROM trades WHERE TradeDate = ? AND TradeID = ?");
	}
	catch (std::exception &e)
	{
		cerr << "Exception: " << e.what() << endl;
		return -1;
	}
}

int DbConnector::query(string date, string tradeId)
{
	sql::ResultSet  *res;
	queryStmt->setDateTime(1, date);
	queryStmt->setString(2, tradeId);
	res = queryStmt->executeQuery();
	return res->rowsCount();
}

int DbConnector::insert(CThostFtdcTradeField *pTrade) {
	string date = pTrade->TradeDate;
	string dateFormatted = date.substr(0, 4) + "-" + date.substr(4, 2) + "-" + date.substr(6, 2);
	string dateTime = dateFormatted + " " + pTrade->TradeTime;
	if (this->query(dateFormatted, pTrade->TradeID) > 0)
	{
		cout << "Trade already exists for " << dateFormatted << pTrade->TradeID << endl;
		return 0;
	}
	insertStmt->setInt(1, stoi(pTrade->OrderSysID));
	insertStmt->setString(2, pTrade->ExchangeInstID);
	insertStmt->setString(3, to_string(pTrade->OffsetFlag));
	insertStmt->setString(4, pTrade->Direction == '0' ? "B" : "S");
	insertStmt->setDouble(5, pTrade->Price);
	insertStmt->setInt(6, pTrade->Volume);
	insertStmt->setDateTime(7, dateTime);
	insertStmt->setDateTime(8, dateFormatted);
	insertStmt->setInt(9, stoi(pTrade->TradeID));
	insertStmt->execute();
	return 1;
}