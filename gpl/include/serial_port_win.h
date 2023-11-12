#include "network.h"
#include "GPL.h"

#include <Windows.h>

namespace gpl
{
	class GPL_API WinCom : public COM
	{
	public:
		WinCom(const std::string& name);
		~WinCom() override;
		void received() override;
		void send(const std::string& msg) override;
	private:
		HANDLE m_hSerial = NULL;
		DCB m_dcbSerialParams{ 0 };
		COMMTIMEOUTS m_timeouts{ 0 };
		bool m_bUseTimeout{ true };
	};

	COM* createComPortOS(const std::string& name);
}