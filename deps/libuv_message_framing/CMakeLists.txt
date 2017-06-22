cmake_minimum_required(VERSION 3.7)
project(libuv_msgframing)

set(CMAKE_CXX_STANDARD 14)
set(TARGET libuv_msgframing)

include_directories(/home/ravi/projects/iot-dsa-v2/deps/libuv-1.x/include)

set(LIBRARY_SRC
        src/uv_msg_framing.c
        src/uv_msg_framing.h
        src/uv_send_message.c)

add_library(libuv_msgframing ${LIBRARY_SRC})

#add_executable(libuv_message_framing_master ${SOURCE_FILES})

#add_library(libuv_msgframing ${LIBRARY_SRC})
#add_executable(libuv_message_framing_master ${SOURCE_FILES})
#target_link_libraries(libuv_msgframing ${LIBRARY_SRC})