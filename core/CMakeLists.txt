cmake_minimum_required(VERSION 3.10)

project(colasql)

# 将src文件夹下所有文件添加至SOURCE_src
aux_source_directory(./src SOURCE_SRC)
aux_source_directory(./src/constraint SOURCE_CONSTRANIT_SRC)
aux_source_directory(./../file/src/ SOURCE_FILE)

# cpp
add_executable(
    colasql

    # main
    main.cpp

    # core
    ${SOURCE_SRC}
    ${SOURCE_CONSTRANIT_SRC}

    # file
    ${SOURCE_FILE}
)

# h (directory)
target_include_directories(
    colasql
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include/.
    ${PROJECT_SOURCE_DIR}/include/constraint/.
    ${PROJECT_SOURCE_DIR}/../file/include/.
)

target_compile_features(
    colasql
    PRIVATE
    cxx_std_17
)