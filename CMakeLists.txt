cmake_minimum_required(VERSION 3.20)
project(3)

set(CMAKE_CXX_STANDARD 20)

set( Boost_USE_STATIC_LIBS ON )
set( Boost_USE_MULTITHREADED ON )
set( BOOST_LIBRARYDIR "$ENV{BOOST_ROOT}/stage_${CMAKE_ADDRESS_MODEL}/lib" )
set( SEARCH_PARAMETERS REQUIRED QUIET )

FIND_PACKAGE(Boost 1.71.0 COMPONENTS program_options )
MESSAGE(${Boost_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
add_executable (3 A.cpp)
add_executable (B B.cpp data.h)
add_executable (C C.cpp Calculate.h Calculate.cpp)
add_executable (D D.cpp)
TARGET_LINK_LIBRARIES(3 LINK_PUBLIC ${Boost_LIBRARIES} -static-libgcc -static-libstdc++ pthread  rt )
TARGET_LINK_LIBRARIES(B LINK_PUBLIC ${Boost_LIBRARIES} -static-libgcc -static-libstdc++ pthread  rt )
TARGET_LINK_LIBRARIES(C LINK_PUBLIC ${Boost_LIBRARIES} -static-libgcc -static-libstdc++ pthread  rt )
TARGET_LINK_LIBRARIES(D LINK_PUBLIC ${Boost_LIBRARIES} -static-libgcc -static-libstdc++ pthread  rt )


