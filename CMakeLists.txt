cmake_minimum_required(VERSION 2.8)
set(CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4
project(BuddyMemoryAllocator)
set(
SRC_LIST
BuddyMemoryAllocator.h
BuddyMemoryAllocator.cpp
MemoryAllocator.h
MemoryAllocator.cpp
test.cpp
)
add_executable(buddyTest ${SRC_LIST})
