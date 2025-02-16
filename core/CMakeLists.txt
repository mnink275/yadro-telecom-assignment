set(INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(HEADERS
    ${INCLUDE_PATH}/computer_club.hpp
    ${INCLUDE_PATH}/clock.hpp
    ${INCLUDE_PATH}/utility.hpp
    ${INCLUDE_PATH}/input.hpp
)

set(SOURCES
    ${SRC_PATH}/computer_club.cpp
    ${SRC_PATH}/clock.cpp
    ${SRC_PATH}/utility.cpp
    ${SRC_PATH}/input.cpp
)

add_library(
    ${PROJECT_NAME}_objs OBJECT ${HEADERS} ${SOURCES}
)
target_include_directories(
    ${PROJECT_NAME}_objs PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
)
