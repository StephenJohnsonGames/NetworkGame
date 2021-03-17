#include "BulletClient.h"

#include "InputMemoryBitStream.h"
#include "InputMemoryStream.h"
#include "OutputMemoryBitStream.h"
#include "OutputMemoryStream.h"
#include "TextureManager.h"
#include "GameObjectRegistry.h"
#include "NetworkManagerClient.h"
#include "StringUtils.h"
#include "NetworkManager.h"
#include "ReplicationManagerClient.h"
#include "Colors.h"


///similar to client version of player
///
///sprite is cached in our texture manager from here

BulletClient::BulletClient()
{
	mSpriteComponent.reset(new SpriteComponent(this));
	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture( "bullet" ));
}


void BulletClient::Read(InputMemoryBitStream& inInputStream)
{
	bool stateBit;

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		Vector3 location;
		inInputStream.Read(location.mX);
		inInputStream.Read(location.mY);


		Vector3 velocity;
		inInputStream.Read(velocity.mX);
		inInputStream.Read(velocity.mY);
		SetVelocity(velocity);

		///location of bullet instantiation to come from player
		SetLocation(location + velocity * NetworkManagerClient::sInstance->GetRoundTripTime());


		float rotation;
		inInputStream.Read(rotation);
		SetRotation(rotation);
	}


	inInputStream.Read(stateBit);
	if (stateBit)
	{
		Vector3 color;
		inInputStream.Read(color);
		///call on colors script to change to black
		SetColor(Colors::Black);
	}

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		inInputStream.Read(mPlayerId, 8);
	}

}

