cmake_minimum_required(VERSION 3.10)

add_library(
            csh_test-lib
            SHARED
            jni_main.cpp )

find_library(
            log-lib
            log )

target_link_libraries(csh_test-lib ${log-lib})