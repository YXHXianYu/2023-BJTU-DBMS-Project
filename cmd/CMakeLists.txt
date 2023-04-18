cmake_minimum_required(VERSION 3.10)

project(colasql)

# 将src文件夹下所有文件添加至SOURCE_src
aux_source_directory(src SOURCE_src)

add_executable(
    colasql
    ${SOURCE_src}
)

target_include_directories(
    colasql
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(
    colasql
    PRIVATE
    cxx_std_17
)