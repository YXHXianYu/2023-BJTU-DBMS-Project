cmake_minimum_required(VERSION 3.10)

project(colasql)

# 将src文件夹下所有文件添加至SOURCE_src
aux_source_directory(./src SOURCE_SRC)
aux_source_directory(./src/constraint SOURCE_CONSTRANIT_SRC)

# cpp
add_executable(
    colasql

    # main
    main.cpp

    # cmd
    ${SOURCE_SRC}
    ${SOURCE_CONSTRANIT_SRC}
)

# h (directory)
target_include_directories(
    colasql
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/constraint
)

target_compile_features(
    colasql
    PRIVATE
    cxx_std_17
)