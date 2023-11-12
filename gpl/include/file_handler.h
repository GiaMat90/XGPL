#pragma once

#include "GPL.h"

#include <filesystem>
#include <string>
#include <map>

namespace gpl
{
	class GPL_API FileHandler
	{
		enum class ExceptionType
		{
			CANNOT_OPEN_FILE,
			CANNOT_CREATE_DIR
		};

		class FileHandlerException : public std::exception
		{
		public:
			FileHandlerException(ExceptionType type) : m_type(type) {};
			char const* what() const override { return m_exceptionMap.at(m_type).c_str(); }
		private:
			const std::map<ExceptionType, std::string> m_exceptionMap{
				{ExceptionType::CANNOT_OPEN_FILE, "CANNOT_OPEN_FILE"},
				{ExceptionType::CANNOT_OPEN_FILE, "CANNOT_CREATE_DIR"}
			};
			ExceptionType m_type;
		};

	public:
		FileHandler(std::filesystem::path file);
		~FileHandler();
		//
		void writeLine(const std::string& line);
	private:
		std::filesystem::path m_file;
	};

	class GPL_API CsvMaker : public FileHandler
	{
	public:

	private:

	};
}