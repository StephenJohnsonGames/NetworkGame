#ifndef _WIN32
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

#include "Engine.h"

class Client : public Engine
{
public:

	static bool StaticInit( );

protected:

	Client();

	virtual void	DoFrame() override;
	virtual void	HandleEvent( SDL_Event* inEvent ) override;

private:



};
