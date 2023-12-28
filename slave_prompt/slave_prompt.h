#pragma once

#include "application.h"
#include "slave_console.h"

using gpl::Application;
using gpl::SlaveConsole;
using gpl::Event;

class SlavePromptApp : public Application, SlaveConsole
{
public:
	SlavePromptApp(std::string name);
	void update(const gpl::Event& event) override; // override from Listener
protected:
	void processInput(const std::string inp);
};