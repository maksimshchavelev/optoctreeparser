# TESTS CMAKELISTS.TXT

include(FetchContent)

message(STATUS "Configuring googletest...")

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

message(STATUS "googletest configured")



# Finding test sources

message(STATUS "${PROJECT_NAME}: finding tests files...")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/unit/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/unit/*.hpp*)


list(LENGTH SOURCES SOURCES_SIZE)
message(STATUS "${PROJECT_NAME}: found ${SOURCES_SIZE} tests")



add_executable(tests ${SOURCES} main.cpp)
target_link_libraries(tests PRIVATE
    GTest::gtest_main
    optoctreeparser)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)


# Copy resources directory
add_custom_command(
    TARGET tests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/tests/unit/resources"
    "$<TARGET_FILE_DIR:tests>/resources"
)



gtest_discover_tests(tests)
