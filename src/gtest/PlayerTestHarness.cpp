#include <limits.h>
#include <math.h>
#include "gtest/gtest.h"

#include "PlayerTestHarness.h"
#include "Player.h"
#include "PlayerClient.h"
#include "TextureManager.h"
#include "Maths.h"
#include "Colors.h"

#include "InputMemoryBitStream.h"
#include "OutputMemoryBitStream.h"

#include <iostream>
#include <fstream>
#include <thread>

/* Reference: http://www.yolinux.com/TUTORIALS/Cpp-GoogleTest.html */

PlayerTestHarness::PlayerTestHarness()
{
  pp = nullptr;
}

PlayerTestHarness::~PlayerTestHarness()
{
  pp.reset();
}

void PlayerTestHarness::SetUp()
{
    GameObject*	go = Player::StaticCreate();
    Player* p = static_cast<Player*>(go);
    this->pp.reset(p);
}

void PlayerTestHarness::TearDown()
{
    this->pp.reset();
    this->pp = nullptr;
}

TEST_F(PlayerTestHarness,constructor_noArgs)
{
  // Check defaults are set
  // Should be no need to do these as they were tested with the base class.
  EXPECT_TRUE(Maths::Is3DVectorEqual(pp->GetColor(), Colors::White));
  EXPECT_TRUE(Maths::Is3DVectorEqual(pp->GetLocation(), Vector3::Zero));
  EXPECT_FLOAT_EQ(pp->GetCollisionRadius(), 0.5f);
  EXPECT_FLOAT_EQ(pp->GetScale(),1.0f);
  EXPECT_FLOAT_EQ(pp->GetRotation(),0.0f);
  EXPECT_EQ(pp->GetIndexInWorld(), -1);
  EXPECT_EQ(pp->GetNetworkId(), 0);

  EXPECT_TRUE(Maths::Is3DVectorEqual(pp->GetVelocity(), Vector3::Zero));
  EXPECT_EQ(pp->GetPlayerId(), 0.0f);

  //Initial state is update all
  int check = 0x000F; //Hex - binary 00000000 00000000 00000000 00001111
  EXPECT_EQ(pp->GetAllStateMask(), check);

  //Check our macro has worked.
  EXPECT_EQ(pp->GetClassId(), 'PLYR');
  EXPECT_NE(pp->GetClassId(), 'HELP');

  //Added some getters so I could check these - not an easy class to test.
  EXPECT_FLOAT_EQ(pp->GetMaxLinearSpeed(),  50.0f);
  EXPECT_FLOAT_EQ(pp->GetMaxRotationSpeed(), 5.0f);
  EXPECT_FLOAT_EQ(pp->GetWallRestitution(), 0.1f);
  EXPECT_FLOAT_EQ(pp->GetNPCRestitution(), 0.1f);
  EXPECT_FLOAT_EQ(pp->GetLastMoveTimestamp(), 0.0f);
  EXPECT_FLOAT_EQ(pp->GetThrustDir(), 0.0f);
  EXPECT_EQ(pp->GetHealth(), 10);
  EXPECT_FALSE(pp->IsShooting());
}


/* Tests Omitted
* There's a good chunk of this which cannot be tested in this limited example,
* however there should be enough to underake some testing of the serialisation code.
*/

TEST_F(PlayerTestHarness,EqualsOperator1)
{ /* Won't compile - why?
  Player a ();
  Player b ();

  a.SetPlayerId(10);
  b.SetPlayerId(10);

  EXPECT_TRUE(a == b);*/
}

TEST_F(PlayerTestHarness,EqualsOperator2)
{
  Player *a = static_cast<Player*>(Player::StaticCreate());
  Player *b = static_cast<Player*>(Player::StaticCreate());

  a->SetPlayerId(10);
  b->SetPlayerId(10);

  EXPECT_TRUE(*a == *b);
}

/* Need more tests here */

TEST_F(PlayerTestHarness,EqualsOperator3)
{
  Player *a = static_cast<Player*>(Player::StaticCreate());
  Player *b = static_cast<Player*>(Player::StaticCreate());

  a->SetPlayerId(10);
  b->SetPlayerId(30);

  EXPECT_FALSE(*a == *b);
}

TEST_F(PlayerTestHarness,EqualsOperator4)
{
  PlayerPtr b(static_cast<Player*>(Player::StaticCreate()));

  pp->SetPlayerId(10);
  b->SetPlayerId(10);

  EXPECT_TRUE(*pp == *b);
}
/* No longer works, as this has been moved to the PlayerClient class */
TEST_F(PlayerTestHarness,serialiseAndDeserialisePlayer)
{ /*
  std::shared_ptr<OutputMemoryBitStream> out;
  std::shared_ptr<InputMemoryBitStream> in;

  const int BUFF_MAX = 512;
  char* bigBuffer = new char[BUFF_MAX]; //testing only - gets replaced.
  in.reset(new InputMemoryBitStream(bigBuffer,BUFF_MAX));
  out.reset(new OutputMemoryBitStream());

  PlayerPtr readIn(static_cast<Player*>(Player::StaticCreate()));

   EXPECT_TRUE(*pp == *readIn); //expect constructed objs to be the same.

  // change this one a bit so I know the changes have copied over.
  pp->SetPlayerId(20);

  //OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState
  uint32_t state = 0x000F;

  EXPECT_FALSE(*pp == *readIn); //with one changed should be different.

  //write it into a buffer.
  pp->Write(*out,state);

  // ... imagine networking goes on and we get an
  // actually we're connecting the output buffer to the input.
  // copy the buffer first (or we get double de-allocation)
  int copyLen = out->GetByteLength();
  char* copyBuff = new char[copyLen];
  memcpy(copyBuff, out->GetBufferPtr(), copyLen);

  in.reset(new InputMemoryBitStream(copyBuff, copyLen));

  // update from our server.
  readIn->Read(*in);

  // expect these to now be the same.
  EXPECT_TRUE(*pp == *readIn); //expect constructed objs to be the same. 
  */
}
