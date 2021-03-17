# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)

set(PROJECT_NAME_STR NetworkGame)
project(${PROJECT_NAME_STR} C CXX)

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/bin)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/assets)

#from
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#from - https://stackoverflow.com/questions/44478492/cmake-failed-to-run-msbuild-command-msbuild-exe
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# Sadly these work with the very latest CMake only
# VS support is patchy too.
# set (CMAKE_CXX_STANDARD 14)


if(CMAKE_COMPILER_IS_GNUCXX)
  # New CMakeOnly
  # add_definitions(-Wall -ansi -Wno-deprecated -pthread)
  # Note: -ansi reinforces C90, will override c++11
  set(GNU_EXTRA_STUFF "-Wall -Wno-deprecated -Wno-multichar -pthread")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GNU_EXTRA_STUFF}")
endif()

if(MSVC)
    #vc 2012 fix for vararg templates
    set(MSVC_COMPILER_DEFS "-D_VARIADIC_MAX=10")
endif()

#-------------------
# set common include folder for module
#-------------------

# message(STATUS "Source dir:" ${PROJECT_SOURCE_DIR})
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/strings ${PROJECT_SOURCE_DIR}/maths ${PROJECT_SOURCE_DIR}/networking ${PROJECT_SOURCE_DIR}/serialisation ${PROJECT_SOURCE_DIR}/replication ${PROJECT_SOURCE_DIR}/client ${PROJECT_SOURCE_DIR}/server)
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)

#-------------------
# Configure Threads 
#-------------------
find_package(Threads REQUIRED)

if(Threads_FOUND)
	message(STATUS "Threads found is true ")
	if(NOT CMAKE_THREAD_LIBS_INIT)
		message(STATUS "No linking command defined, are we on Windows")
		if(WIN32)
			message(STATUS "Yuk, cache the value, the SDL2.cmake has a problem")
			set(CMAKE_THREAD_LIBS_INIT_STORED ${CMAKE_THREAD_LIBS_INIT})
			set(CMAKE_THREAD_LIBS_INIT 1)
		endif()
	endif()
endif()


#if(Threads_FOUND)
#	message(STATUS "Threads found is true?")
#endif()


#-------------------
# Configure SDL2
#-------------------
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/sdl2)
Message(STATUS ${CMAKE_MODULE_PATH})
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
#find_package(SDL2_gfx REQUIRED)

message(STATUS "SDL Libraries seem to be bugged?:" ${SDL2_LIBRARIES})
list(REMOVE_ITEM SDL2_LIBRARIES "1")
message(STATUS "SDL Libraries hacked?:" ${SDL2_LIBRARIES})

set(CMAKE_THREAD_LIBS_INIT ${CMAKE_THREAD_LIBS_INIT_STORED})
message(STATUS "Reset THREAD_LIBS_INIT")

# Recurse into the "maths" etc subdirectories. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.

add_subdirectory (maths)
add_subdirectory (strings)
add_subdirectory (networking)
add_subdirectory (serialisation)
add_subdirectory (replication)
add_subdirectory (engine)

add_subdirectory (client)
add_subdirectory (server)


#-------------------
# Test
#-------------------
#from snikulov - https://github.com/snikulov/google-test-examples/blob/master/CMakeLists.txt

# On Windows make sure you don't try and use pthreads 
if(WIN32)
	message("Windows build, compile gtest without pthreads ")
	set(gtest_disable_pthreads ON)
	set(CMAKE_USE_WIN32_THREADS_INIT 1)
endif()

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

# On Windows make sure you don't try and use pthreads 
if(WIN32)
	set(gtest_disable_pthreads ON)
	set(CMAKE_USE_WIN32_THREADS_INIT 1)
endif()

include_directories(${COMMON_INCLUDES})

add_definitions(${MSVC_COMPILER_DEFS})

enable_testing()
set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)
file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/gtest/*.cpp)

if(WIN32)
	# add_executable(${PROJECT_TEST_NAME} WIN32 ${TEST_SRC_FILES})
	add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
else()
	add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
endif()

target_link_libraries(${PROJECT_TEST_NAME} LINK_PUBLIC gtest_main)

target_link_libraries(${PROJECT_TEST_NAME} LINK_PUBLIC ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries (${PROJECT_TEST_NAME} LINK_PUBLIC strings)
target_link_libraries (${PROJECT_TEST_NAME} LINK_PUBLIC maths)
target_link_libraries (${PROJECT_TEST_NAME} LINK_PUBLIC networking)
target_link_libraries (${PROJECT_TEST_NAME} LINK_PUBLIC serialisation)
target_link_libraries(${PROJECT_TEST_NAME} LINK_PUBLIC replication)
target_link_libraries (${PROJECT_TEST_NAME} LINK_PUBLIC engine)

 target_link_libraries (${PROJECT_TEST_NAME} LINK_PUBLIC clientlib)
 target_link_libraries (${PROJECT_TEST_NAME} LINK_PUBLIC serverlib)

# Link SDL2

# target_include_directories(${PROJECT_TEST_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
# target_link_libraries(${PROJECT_TEST_NAME} LINK_PRIVATE ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(${PROJECT_TEST_NAME}  LINK_PRIVATE SDL2::Core)

# Link SDL2_image

# target_include_directories(${PROJECT_TEST_NAME} PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
# target_link_libraries(${PROJECT_TEST_NAME} LINK_PRIVATE ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(${PROJECT_TEST_NAME}  LINK_PRIVATE SDL2::Image)

add_test(test1 ${PROJECT_TEST_NAME})

#-------------------
# INSTALL TARGET
#-------------------

message(STATUS "Where is everything going %s" ${CMAKE_CURRENT_BINARY_DIR})
message(STATUS "Where is everything going %s" ${PROJECT_BINARY_DIR})

set(APP ${PROJECT_TEST_NAME})

# post-build copy for win32
if(WIN32 AND NOT MINGW)
	add_custom_command( TARGET ${APP} PRE_BUILD
		COMMAND if not exist .\\dist\\bin mkdir .\\dist\\bin )
	add_custom_command( TARGET ${APP} POST_BUILD
		COMMAND copy \"$(TargetPath)\" .\\dist\\bin )
endif(WIN32 AND NOT MINGW)

if(MINGW OR UNIX)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/dist/bin)
endif(MINGW OR UNIX)

message(STATUS "Value of APP " ${APP})

install(TARGETS ${APP}
	CONFIGURATIONS Release RelWithDebInfo Debug
	RUNTIME DESTINATION bin
	)

message(STATUS "Relative path" ${CMAKE_SOURCE_DIR})

install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets
	CONFIGURATIONS Release RelWithDebInfo Debug
	DESTINATION ./
	)

if(WIN32)
	# https://stackoverflow.com/questions/23950887/does-cmake-offer-a-method-to-set-the-working-directory-for-a-given-build-system
	# Ignored due to a VS issue https://developercommunity.visualstudio.com/content/problem/268817/debugger-no-longer-respects-localdebuggerworkingdi.html
	set_target_properties(${APP} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}/bin")
	message(STATUS "Binary dir!" ${CMAKE_CURRENT_BINARY_DIR})

	# Get all the SDL2 files! 
	file(GLOB_RECURSE SDL2_DLL_FILES  ${SDL2_PATH}/*.dll)

	install(FILES ${SDL2_DLL_FILES}
		CONFIGURATIONS Release RelWithDebInfo Debug
		DESTINATION bin
		)

	# Get all the SDL2_image files! 
	file(GLOB_RECURSE SDL2_IMAGE_DLL_FILES  ${SDL2_IMAGE_PATH}/*.dll)

	install(FILES ${SDL2_IMAGE_DLL_FILES}
		CONFIGURATIONS Release RelWithDebInfo Debug
		DESTINATION bin
		)

	# Get all the Gtest files! 
	# file(GLOB_RECURSE GTEST_DLL_FILES  ${GTEST_DIR}/*.dll)

	#install(FILES ${GTEST_DLL_FILES}
	#	DESTINATION bin
	#	CONFIGURATIONS Release RelWithDebInfo Debug)

endif()
	

message(STATUS "DONE!")
