#include "Player.h"
#include "NetworkManagerServer.h"
#include "Bullet.h"

///similar to server version of player
///needs to inherit bullet and player for collision and position
///
///has a life cycle for if it doesn't hit an enemy player to clean up the bullet

class BulletServer : public Bullet
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new BulletServer()); }
	void HandleDying() override;

	virtual bool		HandleCollisionWithPlayer(Player* inPlayer) override;

	virtual void Update() override;

protected:
	BulletServer();

private:
	float mTimeToDie;

};