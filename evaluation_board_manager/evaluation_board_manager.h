#pragma once

#include "application.h"
#include "master_console.h"
#include "network.h"
#include "file_handler.h"

#include <thread>

using gpl::Application;
using gpl::MasterConsole;
using gpl::COM;
using gpl::FileHandler;

class EvaMan : public Application, MasterConsole
{
public:	// methods
	EvaMan(std::string name);
	~EvaMan();
	// commands
	void logHandle(const bool active);
protected:	// methods
	//void startSlaveConsole() override;
	//void stopSlaveConsole() override;
	void processInput(const std::string inp) override;
	void writeCSV();
protected:	//fields
	COM* m_pComPort = nullptr;
	//
	FileHandler m_csv = FileHandler("C:/tmp/log.csv");
	// threads
	std::thread m_receivedThread;	// thread of polling with the com-port
	std::thread m_writeCSVThread;	// thread that handle the buffer and the csv
};