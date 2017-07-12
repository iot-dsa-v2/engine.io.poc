#
cmake_minimum_required(VERSION 3.7)
project(engine_asio_poc)

set(CMAKE_CXX_STANDARD 11)

include_directories(../deps/libuv-1.x/include)
include_directories(inc)
include_directories(src)
include_directories(src/system.io.adapters)
include_directories(src/system.os)

link_directories(../deps/libuv.1.10.0-preview1-22040/runtimes/linux-x64/native)

set(SOURCE_FILES
        inc/platform-config.h
        inc/platform-linux.h
        inc/platform-universal.h
        src/system.io.adapters/operating-context.cpp
        src/system.io.adapters/operating-context.h
        src/system.io.adapters/stream.io-uv.cpp
        src/system.io.adapters/stream.io-uv.h
        src/system.io.adapters/systembus.cpp
        src/system.io.adapters/systembus.h
        src/system.os/platform-config.cpp
        src/system.os/platform-linux.cpp
        src/engine.io.cpp
        src/platform.cpp
        src/platform.h)

add_executable(engine_asio_poc ${SOURCE_FILES})
target_link_libraries(engine_asio_poc libuv.so)