cmake_minimum_required(VERSION 3.5)

project(unit_tests)

find_package(Qt5 COMPONENTS Core REQUIRED)

include_directories(${PROJECT_NAME})

add_subdirectory (googletest)
config_compiler_and_linker()

set(SOURCE_LIB mock_tcp_server.hpp
               unit_send_request_code.hpp
               unit_send_request_jwt.hpp
               unit_send_request_presence.hpp
               unit_send_request_contacts.hpp
               unit_send_request_dialog.hpp
               unit_send_request_message.hpp)

add_library(unit_tests STATIC ${SOURCE_LIB})

target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core gtest gmock)
