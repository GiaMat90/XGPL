#pragma once

#include "application.h"

using gpl::Application;

class TestApp : public Application
{
public:	// methods
	TestApp(std::string name);
	~TestApp();
protected:	// methods
	void processInput(const std::string inp) override;
protected:	//fields
};