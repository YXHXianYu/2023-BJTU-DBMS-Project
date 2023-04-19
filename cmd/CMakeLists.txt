cmake_minimum_required(VERSION 3.10)

project(colasql)

# 将src文件夹下所有文件添加至SOURCE_src
aux_source_directory(./src/ SOURCE_SRC)
aux_source_directory(./../core/src/ SOURCE_CORE)
aux_source_directory(./../core/src/constraint/ SOURCE_CONSTRAINT)
aux_source_directory(./../file/src/ SOURCE_FILE)


add_executable(
    colasql

    # cmd
    ${SOURCE_SRC}

    # core
    ${SOURCE_CORE}

    # core/constraints
    ${SOURCE_CONSTRAINT}

    # file
    ${SOURCE_FILE}
)

target_include_directories(
    colasql
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include/.
    ${PROJECT_SOURCE_DIR}/../core/include/.
    ${PROJECT_SOURCE_DIR}/../core/include/constraint/.
    ${PROJECT_SOURCE_DIR}/../file/include/.
)

target_compile_features(
    colasql
    PRIVATE
    cxx_std_17
)