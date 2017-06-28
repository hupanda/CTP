#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

//STL reference
#include <iostream>
#include <string>
#include <windows.h>
#include <chrono>
#include <ctime>
using namespace std;

class ConfigManager
{
public:
	int init();
	string flowPath;
	string brokerHost;
	string brokerId;
	string brokerUserId;
	string brokerPassword;
	string dbHost;
	string dbSchema;
	string dbUserId;
	string dbPassword;
};

