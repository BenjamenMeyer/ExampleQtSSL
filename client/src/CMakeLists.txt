SET(CLIENT_PROGRAM_EXE exampleQtSslClient)
SET(THE_CLIENT_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/../include)

SET(THE_COMMON_LIB_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/common/include)
FILE(GLOB_RECURSE common_lib_include ${THE_COMMON_LIB_INCLUDE_DIR}/*.h)

FILE(GLOB_RECURSE client_header_files ${THE_CLIENT_INCLUDE_DIR}/*.h)
FILE(GLOB_RECURSE client_source_files *.cpp)

INCLUDE_DIRECTORIES(${THE_CLIENT_INCLUDE_DIR} ${CMAKE_HOME_DIRECTORY}/common/include)

SET(CLIENT_SOURCES ${client_source_files} ${client_header_files} ${common_lib_include})

ADD_EXECUTABLE(${CLIENT_PROGRAM_EXE} ${CLIENT_SOURCES})
ADD_DEPENDENCIES(${CLIENT_PROGRAM_EXE} exampleQtSslCommon)
TARGET_LINK_LIBRARIES(${CLIENT_PROGRAM_EXE} Qt5::Widgets Qt5Network Qt5::Gui exampleQtSslCommon)
