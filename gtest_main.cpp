
#include <cstdlib>


#include <limits.h>

#include "gtest/gtest.h"

#ifndef _WIN32
char** __argv;
int __argc;
#endif

#define SDL_MAIN_HANDLED


#if _WIN32
#include <Windows.h>
#endif

#if _WIN32
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
	::testing::InitGoogleTest(&__argc, __argv);
	return RUN_ALL_TESTS();

}
#else
int main(int argc, char** argv)
{
	__argc = argc;
	__argv = argv;
	::testing::InitGoogleTest(&__argc, __argv);
	return RUN_ALL_TESTS();
}
#endif