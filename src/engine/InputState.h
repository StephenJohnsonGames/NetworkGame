#ifndef INPUT_STATE_H
#define INPUT_STATE_H

class InputMemoryBitStream;
class OutputMemoryBitStream;

#include <memory>

class InputState
{
public:

	InputState() :
	mDesiredRightAmount( 0 ),
	mDesiredLeftAmount( 0 ),
	mDesiredForwardAmount( 0 ),
	mDesiredBackAmount( 0 ),
	mIsShooting( false )
	{}

	float GetDesiredHorizontalDelta()	const { return mDesiredRightAmount - mDesiredLeftAmount; }
	float GetDesiredVerticalDelta()		const { return mDesiredForwardAmount - mDesiredBackAmount; }
	bool  IsShooting()					const { return mIsShooting; }

	bool Write( OutputMemoryBitStream& inOutputStream ) const;
	bool Read( InputMemoryBitStream& inInputStream );

private:
	friend class InputManager;
	friend class InputStateTestHarness;

	float	mDesiredRightAmount, mDesiredLeftAmount;
	float	mDesiredForwardAmount, mDesiredBackAmount;
	bool	mIsShooting;
};

typedef std::shared_ptr< InputState >	InputStatePtr;

#endif
