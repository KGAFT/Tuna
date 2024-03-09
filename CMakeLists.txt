cmake_minimum_required(VERSION 3.16)


project(Tuna)

set(CMAKE_C_STANDARD 23)


file(GLOB_RECURSE DEF_FILES src/Tuna/*.h*)
file(GLOB_RECURSE SRC_FILES src/Tuna/*.c*)

if(WIN32)
    file(GLOB_RECURSE DEF_DRIVERS_FILES src/DriversImplementation/Windows/*.h*)
    file(GLOB_RECURSE SRC_DRIVERS_FILES src/DriversImplementation/Windows/*.c*)
endif()

if(LINUX)
    file(GLOB_RECURSE DEF_DRIVERS_FILES src/DriversImplementation/Linux/*.h*)
    file(GLOB_RECURSE SRC_DRIVERS_FILES src/DriversImplementation/Linux/*.c*)
endif()
if(APPLE)
    file(GLOB_RECURSE DEF_DRIVERS_FILES src/DriversImplementation/Mac/*.h*)
    file(GLOB_RECURSE SRC_DRIVERS_FILES src/DriversImplementation/Mac/*.c*)
endif()

add_executable(Tuna ${DEF_FILES} ${SRC_FILES} ${DEF_DRIVERS_FILES} ${SRC_DRIVERS_FILES} src/Executables/tun.c)
add_executable(ServTest ${DEF_FILES} ${SRC_FILES} ${DEF_DRIVERS_FILES} ${SRC_DRIVERS_FILES} src/Executables/serv.c)
add_executable(ClientTest ${DEF_FILES} ${SRC_FILES} ${DEF_DRIVERS_FILES} ${SRC_DRIVERS_FILES} src/Executables/client.c)

target_include_directories(Tuna PUBLIC src/Tuna )
target_include_directories(ServTest PUBLIC src/Tuna )
target_include_directories(ClientTest PUBLIC src/Tuna )

