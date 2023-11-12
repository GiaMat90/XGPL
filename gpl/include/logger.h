#pragma once

#include "GPL.h"

#include "console.h"

#include <string>
#include <memory>
#include <map>
#include <filesystem>

namespace gpl
{
	enum class LoggerType : unsigned int
	{
		FILE = 0,
		CONSOLE = 1,
		ALL
	};

	enum class LogLevel : unsigned int
	{
		DEBUG = 1,
		INFO = 100,
		WARNING = 200,
		ERROR = 500
	};

	class LoggerBase;

	class GPL_API Logger
	{
	public:
		explicit Logger(LoggerType type);
		Logger(std::filesystem::path filePath, std::string fileName, std::string fileFormat);
		//Logger(ConsoleColor color);
		~Logger() {};
		//
		void log(LogLevel lvl, std::string log);
	private:
		std::unique_ptr<LoggerBase> m_pLogger;
	};

	class GPL_API LoggerBase
	{
	public:
		LoggerBase() {};
		virtual ~LoggerBase() {};
		virtual void log(LogLevel lvl, std::string log) = 0;
	protected:
		const std::map<LogLevel, std::string> m_levelStringMap{
			{LogLevel::DEBUG, "DEBUG"},
			{LogLevel::INFO, "INFO"},
			{LogLevel::WARNING, "WARNING"},
			{LogLevel::ERROR, "ERROR"}
		};
	};

	class GPL_API LoggerFile : public LoggerBase
	{
		enum class ExceptionType
		{
			CANNOT_OPEN_FILE,
			CANNOT_CREATE_DIR
		};

		class LoggerFileException : public std::exception
		{	
		public:
			LoggerFileException(ExceptionType type) : m_type(type) {};
			char const* what() const override { return m_exceptionMap.at(m_type).c_str(); }
		private:
			const std::map<ExceptionType, std::string> m_exceptionMap{ 
				{ExceptionType::CANNOT_OPEN_FILE, "CANNOT_OPEN_FILE"},
				{ExceptionType::CANNOT_OPEN_FILE, "CANNOT_CREATE_DIR"}
			};
			ExceptionType m_type;
		};
	public:
		LoggerFile(std::filesystem::path filePath = std::filesystem::current_path(), std::string fileName = "tmp", std::string fileFormat = "log");
		~LoggerFile() {};
		void log(LogLevel lvl, std::string log) override;
	private:
		std::string m_File;
	private:
		void logException(std::string e);
	};
	
	class GPL_API LoggerConsole : public LoggerBase
	{
	public:
		LoggerConsole(ConsoleColor color = ConsoleColor::NATIVE) : m_console(color) {};
		~LoggerConsole() {};
		void log(LogLevel lvl, std::string log) override;
	private:
		Console m_console;
	};
}



