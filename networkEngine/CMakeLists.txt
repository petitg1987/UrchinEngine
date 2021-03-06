cmake_minimum_required(VERSION 3.16)
project(urchinNetworkEngine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    set(CURL_LIB curl.a ssl.a crypto.a brotlidec-static.a brotlicommon-static.a z.a nghttp2.a idn2.a gdi32.a zstd.a ws2_32.a iconv unistring psl)
else()
    set(LINK_OPT -rdynamic) #necessary for SignalHandler to work correctly
    set(CURL_LIB curl.a ssl.a crypto.a brotlidec.a brotlicommon.a z.a nghttp2.a)
endif()

add_definitions(-ffast-math -Wall -Wextra -Wpedantic -Wconversion -Werror -DCURL_STATICLIB)
include_directories(src ../common/src)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_library(urchinNetworkEngine STATIC ${SOURCE_FILES})
target_link_libraries(urchinNetworkEngine ${LINK_OPT} ${CURL_LIB} urchinCommon)
