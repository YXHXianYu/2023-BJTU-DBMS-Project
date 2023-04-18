cmake_minimum_required(VERSION 3.10)

project(colasql)

# 将src文件夹下所有文件添加至SOURCE_src
aux_source_directory(./src SOURCE_SRC)
# aux_source_directory(./../core/src/. SOURCE_CORE)
# aux_source_directory(./src SOURCE_CONSTRAINT)


add_executable(
    colasql

    # cmd
    ${SOURCE_SRC}

    # core
    ./../core/src/colasqltool.cpp

    # constraints (因为constraint下有测试代码，会产生冲突)
    ./../core/src/constraint/constraint.cpp
    ./../core/src/constraint/default_constraint.cpp
    ./../core/src/constraint/foreign_key_constraint.cpp
    ./../core/src/constraint/not_null_constraint.cpp
    ./../core/src/constraint/primary_key_constraint.cpp
    ./../core/src/constraint/unique_constraint.cpp
)

target_include_directories(
    colasql
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/../core/include/constraint/.
    ${PROJECT_SOURCE_DIR}/../core/include/.
)

target_compile_features(
    colasql
    PRIVATE
    cxx_std_17
)