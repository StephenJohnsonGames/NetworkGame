
#include "Client.h"
#include "WindowManager.h"
#include "GraphicsDriver.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "TextureManager.h"

#include "GameObjectRegistry.h"
#include "Player.h"
#include "MoveList.h"

#include "NetworkManagerClient.h"
#include "SocketAddressFactory.h"

#include "WeightedTimedMovingAverage.h"
#include "PlayerClient.h"
#include "RenderManager.h"
#include "StringUtils.h"
#include "Bullet.h"
#include "BulletClient.h"

bool Client::StaticInit( )
{
	// Create the Client pointer first because it initializes SDL
	Client* client = new Client();

	if( WindowManager::StaticInit() == false )
	{
        return false;
	}

  if( GraphicsDriver::StaticInit( WindowManager::sInstance->GetMainWindow() ) == false )
	{
      return false;
  }

	TextureManager::StaticInit();
	RenderManager::StaticInit();
	InputManager::StaticInit();

	sInstance.reset( client );

	return true;
}

Client::Client()
{
	///added an extra gameobject registry here for bullet
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'PLYR', PlayerClient::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'BULL', BulletClient::StaticCreate );

	string destination = StringUtils::GetCommandLineArg( 1 );
	string name = StringUtils::GetCommandLineArg( 2 );

	SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString( destination );

	NetworkManagerClient::StaticInit( *serverAddress, name );

	NetworkManagerClient::sInstance->SetSimulatedLatency( 0.0f );

}

void Client::DoFrame()
{
	InputManager::sInstance->Update();

	Engine::DoFrame();

	NetworkManagerClient::sInstance->ProcessIncomingPackets();

	RenderManager::sInstance->Render();

	NetworkManagerClient::sInstance->SendOutgoingPackets();

}

void Client::HandleEvent( SDL_Event* inEvent )
{
	switch( inEvent->type )
	{
	case SDL_KEYDOWN:
		InputManager::sInstance->HandleInput( EIA_Pressed, inEvent->key.keysym.sym );
		break;
	case SDL_KEYUP:
		InputManager::sInstance->HandleInput( EIA_Released, inEvent->key.keysym.sym );
		break;
	default:
		break;
	}
}
