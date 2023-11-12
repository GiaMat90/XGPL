#include "file_handler.h"

#include <fstream>

namespace gpl
{
	FileHandler::FileHandler(std::filesystem::path file) : m_file(file) {};

	// empty destructor
	FileHandler::~FileHandler() {};

	void FileHandler::writeLine(const std::string& line)
	{
		try
		{
			std::fstream fileStream;
			std::string fileString = m_file.string();
			fileStream.open(fileString, std::fstream::out | std::fstream::app);
			if (fileStream.is_open())
			{
				if ('\n' == line.back())
				{
					fileStream << line;
				}
				else
				{
					fileStream << line << std::endl;
				}
				fileStream.close();
			}
			else
			{
				throw(FileHandlerException(ExceptionType::CANNOT_OPEN_FILE));
			}
		}
		catch (std::exception& e)
		{
			throw e;
		}
		catch (...)
		{
			// ops!
		}
	}
}