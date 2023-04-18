cmake_minimum_required(VERSION 3.10)

project(Co1aSQL_Command)

# 将src文件夹下所有文件添加至SOURCE_src
aux_source_directory(src SOURCE_src)

add_executable(
    Co1aSQL_Command
    ${SOURCE_src}
)

target_include_directories(
    Co1aSQL_Command
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)