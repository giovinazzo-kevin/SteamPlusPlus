#include "SteamPlusPlus.h"

spp::SteamPlusPlus::~SteamPlusPlus()
{
	for( auto it = m_scripts.begin(); it != m_scripts.end(); ++it )
	{
		killScript(it->first);
	}
}