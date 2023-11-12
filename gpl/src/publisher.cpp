#include "publisher.h"

namespace gpl
{
	Publisher::Publisher() {};

	Publisher::~Publisher() 
	{
		if (m_pEvent)
		{
			delete m_pEvent;
		}
	};

	void Publisher::addListener(Listener* listener)
	{
		m_pListeners.emplace_back(listener);
	}
	
	void Publisher::notify(const Event& event)
	{
		for (const auto l : m_pListeners)
		{
			l->update(event);
		}
	}

	Listener::Listener() {};

	Listener::~Listener() {};
}