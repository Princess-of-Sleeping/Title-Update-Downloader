cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()


project(Title-Update-Downloader)
include("${VITASDK}/share/vita.cmake" REQUIRED)
set(VITA_APP_NAME "Title Update Downloader")
set(VITA_TITLEID  "PTUD00000")

set(VITA_VERSION  "01.20")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")



add_executable(${PROJECT_NAME}
  src/main.c
  src/ctrl.c
  src/gupd.c
  src/graphics.c
  src/font.c
  src/hmac-sha256.c
)


target_link_libraries(${PROJECT_NAME}

  k

  SceCtrl_stub
  SceDisplay_stub
  ScePower_stub
  SceShellSvc_stub
  SceSysmodule_stub

  SceHttp_stub
  SceNet_stub
  SceNetCtl_stub

)


vita_create_self(eboot.bin ${PROJECT_NAME} UNSAFE)



vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} eboot.bin
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
)
