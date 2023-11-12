#pragma once

#include "GPL.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <map>

namespace gpl
{
	class GPL_API Parser
	{
		class ParserException : public std::exception
		{
		public:
			enum ExceptionType
			{
				FORMAT_FILE_INCORRECT
			};
			ParserException(ExceptionType e) : m_exception(e) {};			
			char const* what() const override
			{
				return m_exceptionMap.at(m_exception).c_str();
			};
		private:			
			ExceptionType m_exception;
			const std::map<ExceptionType, std::string> m_exceptionMap{ 
				{ExceptionType::FORMAT_FILE_INCORRECT, "Format file incorrect"} 
			};
		};

	public:
		explicit Parser(const std::filesystem::path file = std::filesystem::path()) : m_fileToParse(file) {};
		~Parser() {};
		void parse(const std::filesystem::path file = std::filesystem::path());
		std::string getValue(const std::string key) const { return m_fieldValueMap.at(key); }
	protected:
		std::map<std::string, std::string> m_fieldValueMap{};
		std::filesystem::path m_fileToParse;
		//
		virtual void parseContent(std::fstream & stream) = 0;
	};

	class GPL_API ParserXML : public Parser
	{
	public:
	private:
		void parseContent(std::fstream & stream) override;
		size_t getTag(const std::string content, size_t begin, std::string& token, std::string& value);
	};

	class GPL_API ParserJson : public Parser
	{
	public:
	private:
		void parseContent(std::fstream & stream) override {};
	};

	class GPL_API ParserIni : public Parser
	{
	public:
	private:
		void parseContent(std::fstream & stream) override {};
	};
}
