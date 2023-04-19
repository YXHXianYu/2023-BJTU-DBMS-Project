cmake_minimum_required(VERSION 3.10)

project(colasql)

# 将src文件夹下所有文件添加至SOURCE_src
aux_source_directory(./src/ SOURCE_SRC)
aux_source_directory(./../core/src/ SOURCE_CORE)
aux_source_directory(./../core/src/constraint/ SOURCE_CONSTRAINT)


add_executable(
    colasql

    # cmd
    ${SOURCE_SRC}

    # core
    ${SOURCE_CORE}

    # constraints
    ${SOURCE_CONSTRAINT}
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