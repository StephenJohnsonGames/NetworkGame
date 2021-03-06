
#include "Bullet.h"
#include "SpriteComponent.h"

class BulletClient : public Bullet
{
public:
	static	GameObjectPtr	StaticCreate() { return GameObjectPtr(new BulletClient() ); }

	virtual void		Read(InputMemoryBitStream& inInputStream) override;

protected:
	BulletClient();

private:

	SpriteComponentPtr	mSpriteComponent;
};
typedef shared_ptr< BulletClient >	BulletClientPtr;