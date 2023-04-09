cmake_minimum_required(VERSION 3.22)

project(MyProject)

find_package(SDL2 REQUIRED)

if( NOT DEFINED CMAKE_TOOLCHAIN_FILE )
  if( DEFINED ENV{VITASDK} )
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

set(VITA_APPNAME RedSquare)
set(VITA_TITLEID "REDSQUARE")

project(${VITA_APPNAME})
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")

include_directories(
  ${VITASDK}/arm-vita-eabi/include/SDL
)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(
    src,
    include,
    lib
    vita-data
)

add_executable(${VITA_APPNAME}
    src/main.cpp
)

target_link_libraries(${VITA_APPNAME} ${SDL2_LIBRARIES})

file(GLOB_RECURSE ASSETS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} data/*)

set(DATA "")

FOREACH(ITEM ${ASSETS})
  list(APPEND DATA "FILE")
  list(APPEND DATA ${ITEM})
  list(APPEND DATA ${ITEM})
ENDFOREACH()

vita_create_self(eboot.bin ${VITA_APPNAME} UNSAFE)
vita_create_vpk(${VITA_APPNAME}.vpk ${VITA_TITLEID} eboot.bin
  NAME ${VITA_APPNAME}
  FILE res/icon0.png sce_sys/icon0.png
  FILE res/bg.png sce_sys/livearea/contents/bg.png
  FILE res/startup.png sce_sys/livearea/contents/startup.png
  FILE res/template.xml sce_sys/livearea/contents/template.xml
  ${DATA}
)