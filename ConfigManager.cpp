#include "ConfigManager.h"
using namespace std;

int ConfigManager::init()
{
	try
	{
		boost::property_tree::ptree pt;
		boost::property_tree::ini_parser::read_ini("CTP.ini", pt);
		this->flowPath = pt.get<string>("General.FlowPath"); // "C:/Users/hupanda/CTP/tdflow";
		this->brokerHost = pt.get<string>("Broker.Host"); //"tcp://58.246.171.3:52101";
		this->brokerId = pt.get<string>("Broker.Id"); //"8000";
		this->brokerUserId = pt.get<string>("Broker.UserId"); //"70801517";
		this->brokerPassword = pt.get<string>("Broker.Password"); //"287122";
		this->dbHost = pt.get<string>("Db.Host"); // "tcp://127.0.0.1:3306";
		this->dbUserId = pt.get<string>("Db.UserId"); //"admin";
		this->dbPassword = pt.get<string>("Db.Password"); //"admin";
		this->dbSchema = pt.get<string>("Db.Schema"); //"ctp";
		return 1;
	}
	catch (std::exception &e)
	{
		cerr << "Exception: " << e.what() << endl;
		return -1;
	}
}
