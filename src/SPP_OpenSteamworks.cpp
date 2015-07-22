#include "SteamPlusPlus.h"

int spp::SteamPlusPlus::initSteamworks()
{
	CreateInterfaceFn factory = m_steamLoader.GetSteam3Factory();
	if( !factory ) {
		spp::printf( spp::kPrintError, "Unable to get Steam3 Factory.\n" );
		return spp::k_EFail;
	}
		
	m_pSteamClient = (ISteamClient017*)factory( STEAMCLIENT_INTERFACE_VERSION_017, NULL );
	if( !m_pSteamClient ) {
		spp::printf( spp::kPrintError, "Unable to get the Steam Client Interface.\n" );
		return spp::k_EFail;
	}
	spp::printf( spp::kPrintBoring, "\tGot the Steam Client Interface.\n" );
	
	m_hPipe = m_pSteamClient->CreateSteamPipe();
	if( m_hPipe == -1 || !m_hPipe ) {
		spp::printf( spp::kPrintError, "Unable to create the Steam Pipe.\n" );
		return spp::k_EFail;
	}
	spp::printf( spp::kPrintBoring, "\tCreated the Steam Pipe.\n" );
	
	m_hUser = m_pSteamClient->ConnectToGlobalUser(m_hPipe);
	if( !m_hUser ) {
		spp::printf( spp::kPrintError, "Unable to connect to the Global User.\n" );
		return spp::k_EFail;
	}
	spp::printf( spp::kPrintBoring, "\tConnected to the Global User.\n" );
	
	m_pSteamUser = (ISteamUser017 *)m_pSteamClient->GetISteamUser( m_hUser, m_hPipe, STEAMUSER_INTERFACE_VERSION_017 );
	if( !m_pSteamUser ) {
		spp::printf( spp::kPrintError, "Unable to get the Steam User Interface.\n" );
		return spp::k_EFail;
	}
	spp::printf( spp::kPrintBoring, "\tGot the Steam User Interface.\n" );
	
	m_pSteamFriends = (ISteamFriends015 *)m_pSteamClient->GetISteamFriends( m_hUser, m_hPipe, STEAMFRIENDS_INTERFACE_VERSION_015 );
	if ( !m_pSteamFriends )
	{
		spp::printf( spp::kPrintError, "Unable to get the Steam Friends Interface.\n" );
		return spp::k_EFail;
	}
	spp::printf( spp::kPrintBoring, "\tGot the Steam Friends Interface.\n" );
	
	/*
	IClientEngine* pClientEngine = (IClientEngine*)factory(CLIENTENGINE_INTERFACE_VERSION, NULL);
	if( !pClientEngine ) {
		spp::printf( spp::kPrintError, "Unable to create the Client Engine Interface.\n" );
		return spp::k_EFail;
	}
	spp::printf( spp::kPrintBoring, "\tCreated the Client Engine Interface.\n" );
	
	m_pClientFriends = pClientEngine->GetIClientFriends(m_hUser, m_hPipe, CLIENTFRIENDS_INTERFACE_VERSION);
	if( !m_pClientFriends ) {
		spp::printf( spp::kPrintError, "Unable to get the Client Friends.\n" );
		return spp::k_EFail;
	}
	spp::printf( spp::kPrintBoring, "\tGot the Client Friends Interface.\n" );
	*/
	
	spp::printf( spp::kPrintNormal, "Done.\n" );
	
	m_initialized = true;
	return spp::k_EOK;
}

void spp::SteamPlusPlus::cleanupSteamworks()
{
	if(m_pSteamClient) {
		if(m_hPipe && m_hPipe != -1) {
			if(m_hUser) {
				m_pSteamClient->ReleaseUser(m_hPipe, m_hUser);
			}
			m_pSteamClient->BReleaseSteamPipe(m_hPipe);
		}
	}

	m_hPipe = 0;
	m_hUser = 0;
	m_pSteamClient = NULL;
	m_pSteamFriends = NULL;
	m_pSteamUser = NULL;
}