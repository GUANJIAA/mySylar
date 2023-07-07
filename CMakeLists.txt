cmake_minimum_required(VERSION 2.8)
project(sylar)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include (cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined")


set(LIB_SRC 
    sylar/log.cc
    sylar/util.cc
    sylar/config.cc
    sylar/thread.cc
    sylar/fiber.cc
    sylar/scheduler.cc
    sylar/iomanager.cc
    sylar/timer.cc
    sylar/hook.cc
    sylar/fd_manager.cc
    sylar/address.cc
    sylar/socket.cc
    sylar/bytearray.cc
    sylar/http/http.cc
    sylar/http/http_parser.cc
    sylar/tcp_server.cc
    sylar/stream.cc
    sylar/socket_stream.cc
    sylar/http/http_session.cc
    sylar/http/http_server.cc
    sylar/http/servlet.cc
    sylar/http/http_connection.cc
    sylar/daemon.cc
    sylar/env.cc
    sylar/application.cc
)

ragelmaker(sylar/http/http11_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/sylar/http)
ragelmaker(sylar/http/httpclient_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/sylar/http)
ragelmaker(sylar/uri.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/sylar)


add_library(sylar SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(sylar)
#add_libray(sylar_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES(sylar_static PROPERTIES OUTPUT_NAME "sylar")

set(LIB_LIB sylar dl pthread yaml-cpp)

add_executable(test tests/test.cc)
add_dependencies(test sylar)
force_redefine_file_macro_for_sources(test)
target_link_libraries(test ${LIB_LIB})

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config sylar)
force_redefine_file_macro_for_sources(test_config)
target_link_libraries(test_config ${LIB_LIB})

add_executable(test_thread tests/test_thread.cc)
add_dependencies(test_thread sylar)
force_redefine_file_macro_for_sources(test_thread)
target_link_libraries(test_thread ${LIB_LIB})

add_executable(test_util tests/test_util.cc)
add_dependencies(test_util sylar)
force_redefine_file_macro_for_sources(test_util)
target_link_libraries(test_util ${LIB_LIB})

add_executable(test_fiber tests/test_fiber.cc)
add_dependencies(test_fiber sylar)
force_redefine_file_macro_for_sources(test_fiber)
target_link_libraries(test_fiber ${LIB_LIB})

add_executable(test_scheduler tests/test_scheduler.cc)
add_dependencies(test_scheduler sylar)
force_redefine_file_macro_for_sources(test_scheduler)
target_link_libraries(test_scheduler ${LIB_LIB})

add_executable(test_iomanager tests/test_iomanager.cc)
add_dependencies(test_iomanager sylar)
force_redefine_file_macro_for_sources(test_iomanager)
target_link_libraries(test_iomanager ${LIB_LIB})

add_executable(test_hook tests/test_hook.cc)
add_dependencies(test_hook sylar)
force_redefine_file_macro_for_sources(test_hook)
target_link_libraries(test_hook ${LIB_LIB})

add_executable(test_address tests/test_address.cc)
add_dependencies(test_address sylar)
force_redefine_file_macro_for_sources(test_address)
target_link_libraries(test_address ${LIB_LIB})

add_executable(test_socket tests/test_socket.cc)
add_dependencies(test_socket sylar)
force_redefine_file_macro_for_sources(test_socket)
target_link_libraries(test_socket ${LIB_LIB})

add_executable(test_bytearray tests/test_bytearray.cc)
add_dependencies(test_bytearray sylar)
force_redefine_file_macro_for_sources(test_bytearray)
target_link_libraries(test_bytearray ${LIB_LIB})

add_executable(test_http tests/test_http.cc)
add_dependencies(test_http sylar)
force_redefine_file_macro_for_sources(test_http)
target_link_libraries(test_http ${LIB_LIB})

add_executable(test_http_parser tests/test_http_parser.cc)
add_dependencies(test_http_parser sylar)
force_redefine_file_macro_for_sources(test_http_parser)
target_link_libraries(test_http_parser ${LIB_LIB})

add_executable(test_tcp_server tests/test_tcp_server.cc)
add_dependencies(test_tcp_server sylar)
force_redefine_file_macro_for_sources(test_tcp_server)
target_link_libraries(test_tcp_server ${LIB_LIB})

add_executable(echo_server examples/echo_server.cc)
add_dependencies(echo_server sylar)
force_redefine_file_macro_for_sources(echo_server)
target_link_libraries(echo_server ${LIB_LIB})

add_executable(test_http_server tests/test_http_server.cc)
add_dependencies(test_http_server sylar)
force_redefine_file_macro_for_sources(test_http_server)
target_link_libraries(test_http_server ${LIB_LIB})

add_executable(test_http_connection tests/test_http_connection.cc)
add_dependencies(test_http_connection sylar)
force_redefine_file_macro_for_sources(test_http_connection)
target_link_libraries(test_http_connection ${LIB_LIB})

add_executable(test_uri tests/test_uri.cc)
add_dependencies(test_uri sylar)
force_redefine_file_macro_for_sources(test_uri)
target_link_libraries(test_uri ${LIB_LIB})

add_executable(my_http_server samples/my_http_server.cc)
add_dependencies(my_http_server sylar)
force_redefine_file_macro_for_sources(my_http_server)
target_link_libraries(my_http_server ${LIB_LIB})

add_executable(test_daemon tests/test_daemon.cc)
add_dependencies(test_daemon sylar)
force_redefine_file_macro_for_sources(test_daemon)
target_link_libraries(test_daemon ${LIB_LIB})

add_executable(test_env tests/test_env.cc)
add_dependencies(test_env sylar)
force_redefine_file_macro_for_sources(test_env)
target_link_libraries(test_env ${LIB_LIB})

add_executable(test_application tests/test_application.cc)
add_dependencies(test_application sylar)
force_redefine_file_macro_for_sources(test_application)
target_link_libraries(test_application ${LIB_LIB})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
