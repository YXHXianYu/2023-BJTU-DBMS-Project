# This file is to test constraints

cmake_minimum_required(VERSION 3.10)

project(colasql_constraint_test)

# 将src文件夹下所有文件添加至SOURCE_src
aux_source_directory(./ SOURCE_src)

add_executable(
    colasql_constraint_test
    ${SOURCE_src}
)

target_include_directories(
    colasql_constraint_test
    PRIVATE
    ${PROJECT_SOURCE_DIR}/../../include/constraint
)

target_compile_features(
    colasql_constraint_test
    PRIVATE
    cxx_std_17
)