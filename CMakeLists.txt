cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(optoctreeparser
    VERSION 1.0.0
    DESCRIPTION "Library for parsing octotree landscape files in Subnautica"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

option(CMAKE_BUILD_TYPE "Build type" Release)
option(BUILD_TESTS "Need to build tests" OFF)
option(BUILD_SHARED_LIBS "Need to build as shared library?" OFF)


file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/include/*.hpp*
    ${CMAKE_CURRENT_LIST_DIR}/include/*.tpp*)


# Enable sanitizers if Debug
if((CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU") AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    message(STATUS "${PROJECT_NAME}: Using GCC/Clang sanitizer configuration")

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wpedantic -g")
    add_compile_definitions(_GLIBCXX_DEBUG)

    set(SANITIZER_FLAGS "-fsanitize=address,undefined,leak -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${SANITIZER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${SANITIZER_FLAGS}")
endif()


add_library(${PROJECT_NAME} ${SOURCES})

# Define the public header directories.
# The `PUBLIC` keyword ensures that any project linking to optoctreeparser
# will automatically get these include paths.
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# This block is essential for CMake's `find_package` and `FetchContent`.
# It exports the library target so other projects can find and use it easily.
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/optoctreeparserConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Export the library target for other projects.
export(TARGETS ${PROJECT_NAME} FILE optoctreeparserTargets.cmake)

# The install commands are not strictly required for FetchContent,
# but they are good practice for making your library more flexible.
# This makes your library installable.
install(TARGETS ${PROJECT_NAME} EXPORT optoctreeparserTargets)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT optoctreeparserTargets
        NAMESPACE optoctreeparser::
        DESTINATION lib/cmake/optoctreeparser)


if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE
    /W4
    /permissive-
    /sdl
    /w14254
    /w14265
    /w14287
    /w14296
  )
else()
  # G++ / CLANG
  target_compile_options(${PROJECT_NAME} PRIVATE
    -pedantic
    -Wall -Wextra
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wlogical-op
    -Wuseless-cast
  )
endif()


# Tests
if(${BUILD_TESTS})
  message(STATUS "${PROJECT_NAME}: Configuring tests...")

  include(CTest)
  enable_testing()
  add_subdirectory(tests)

  # Add custom target to run tests
  add_custom_target(run-tests
    COMMAND ${CMAKE_CTEST_COMMAND} "--output-on-failure"
    DEPENDS tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )

  message(STATUS "${PROJECT_NAME}: Tests configured")
endif()



