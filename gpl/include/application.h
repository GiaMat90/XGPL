#pragma once

#include "gpl.h"
#include "logger.h"
#include "console.h"

#include <filesystem>
#include <memory>
#include <functional>
#include <charconv>
#include <algorithm>


namespace gpl
{
	class GPL_API Application
	{
	public:
		Application(const std::string appName = "");
		virtual ~Application() {};
		//
		void init(int argc, char* argv[]);
		void run();
	protected:
		//
		virtual void processInput(const std::string inp) = 0;
		bool processDefaultInput(const std::string inp);
		void addHelpMessageLine(const std::string& shortCmd, const std::string& longCmd, const std::string& description, const std::string& inputs = "");
		//
		bool findCommand(std::string inp, std::string shortName, std::string longName);
		void printNotValidCommand();
		void printNotValidParam(const std::string& param) const;
		void printHelp();
		//
		template <typename T>
		bool getParams(const std::string& inp, const std::string commandName, T& param)
		{
			static_assert(!std::is_same<T, std::string>::value);
			// get params
			param = T{};
			std::string sParam = inp.substr(inp.find(commandName + " ") + 1U);
			std::from_chars_result res = std::from_chars(sParam.data(), sParam.data() + sParam.size(), param);
			if (res.ec == std::errc())
			{
				return true;
			}
			else if (res.ec == std::errc::invalid_argument)
			{
				return false;
			}
			else if (res.ec == std::errc::result_out_of_range)
			{
				return false;
			}
			else
			{
				return false;
			}
		};
		// std::string specialization
		template<>
		bool Application::getParams<std::string>(const std::string& inp, const std::string commandName, std::string& param);
	protected:
		//
		Console m_console = Console(gpl::ConsoleColor::ORANGE, ">> ", true);
		Logger m_logger = Logger(std::filesystem::current_path(), "gpl_Application", "log");
		// 
		bool m_bRun = false;
	private:
		//
		void consoleCommandInit(int argc, char* argv[]);
		void configFileInit(std::filesystem::path configFilePath);
		void exit();
		//
	private:	// Fields		
		std::string m_appName = "gpl Application";
		std::string m_helpMsg = "";
		bool m_bDebugMode = false;
	};
}
