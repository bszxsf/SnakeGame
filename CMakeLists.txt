CMAKE_MINIMUM_REQUIRED(VERSION 3.14)

PROJECT(snake VERSION)

# forbid in-tree build
IF ("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
    MESSAGE(FATAL_ERROR "Do not build in-source")
ENDIF()

# global build config
if(MSVC)
    ADD_COMPILE_OPTIONS(/W4 /WX /source-charset:UTF-8)
else()
    ADD_COMPILE_OPTIONS(-Wall -Wextra -pedantic -Werror)
endif()

# source
FILE(GLOB_RECURSE SRCS "src/*.c")
# executable
ADD_EXECUTABLE("${PROJECT_NAME}" "${SRCS}")
# include
# TARGET_INCLUDE_DIRECTORIES("${PROJECT_NAME}" PUBLIC ".")
