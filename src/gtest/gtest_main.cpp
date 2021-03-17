
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
<<<<<<< HEAD
int main(int argc, char** argv)
{
	__argc = argc;
	__argv = argv;
	::testing::InitGoogleTest(&__argc, __argv);
=======
const char** __argv;
int __argc;
int main(int argc, const char** argv)
{
	__argc = argc;
	::testing::InitGoogleTest(&argc, argv);
>>>>>>> e2b6bfd162f9224bed107a496da9773e7187fef6
	return RUN_ALL_TESTS();
}
#endif