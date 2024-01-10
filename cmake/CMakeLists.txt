##########
# libplist
##########

#this is a cmake file for libplist
#this file is used to build libplist as a static library
#it returns a static library named libplist.a
#this file is used by HummingbirdCore

cmake_minimum_required(VERSION 3.24.4)
project(plistLib)

#set default build type
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif ()

#SET STDLIB
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -stdlib=libc++")

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=stdlibc++")


set(PLISTSRCFILES
        ../vendor/libplist/src/Array.cpp
        ../vendor/libplist/src/base64.c
        ../vendor/libplist/src/base64.h
        ../vendor/libplist/src/Boolean.cpp
        ../vendor/libplist/src/bplist.c
        ../vendor/libplist/src/bytearray.c
        ../vendor/libplist/src/bytearray.h
        ../vendor/libplist/src/Data.cpp
        ../vendor/libplist/src/Date.cpp
        ../vendor/libplist/src/Dictionary.cpp
        ../vendor/libplist/src/hashtable.c
        ../vendor/libplist/src/hashtable.h
        ../vendor/libplist/src/Integer.cpp
        ../vendor/libplist/src/jplist.c
        ../vendor/libplist/src/jsmn.c
        ../vendor/libplist/src/jsmn.h
        ../vendor/libplist/src/Key.cpp
        ../vendor/libplist/src/libplist++-2.0.pc.in
        ../vendor/libplist/src/libplist-2.0.pc.in
        ../vendor/libplist/src/Makefile.am
        ../vendor/libplist/src/Node.cpp
        ../vendor/libplist/src/oplist.c
        ../vendor/libplist/src/out-default.c
        ../vendor/libplist/src/out-limd.c
        ../vendor/libplist/src/out-plutil.c
        ../vendor/libplist/src/plist.c
        ../vendor/libplist/src/plist.h
        ../vendor/libplist/src/ptrarray.c
        ../vendor/libplist/src/ptrarray.h
        ../vendor/libplist/src/Real.cpp
        ../vendor/libplist/src/strbuf.h
        ../vendor/libplist/src/String.cpp
        ../vendor/libplist/src/Structure.cpp
        ../vendor/libplist/src/time64.c
        ../vendor/libplist/src/time64.h
        ../vendor/libplist/src/time64_limits.h
        ../vendor/libplist/src/Uid.cpp
        ../vendor/libplist/src/xplist.c
)
add_library(libplist STATIC ${PLISTSRCFILES})
target_include_directories(libplist PUBLIC ../vendor/libplist/include)

