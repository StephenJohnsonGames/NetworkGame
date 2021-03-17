#include "BulletServer.h"
#include "PlayerServer.h"
#include "ClientProxy.h"
#include "Timing.h"
#include "MoveList.h"
#include "Maths.h"
#include "GameObject.h"
#include "ReplicationManagerServer.h"

BulletServer::BulletServer()
{
	mTimeToDie = Timing::sInstance.GetFrameStartTime() + 1.f;
}

void BulletServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}


void BulletServer::Update()
{
	Bullet::Update();

	if (Timing::sInstance.GetFrameStartTime() > mTimeToDie)
	{
		SetDoesWantToDie(true);
	}

}

bool BulletServer::HandleCollisionWithPlayer(Player* inPlayer)
{
	if (inPlayer->GetPlayerId() != GetPlayerId())
	{
		SetDoesWantToDie(true);

		static_cast<PlayerServer*>(inPlayer)->TakeDamage(GetPlayerId());

	}

	return false;
}


