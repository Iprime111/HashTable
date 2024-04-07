cmake_minimum_required (VERSION 3.8 FATAL_ERROR)

project (HashTable)

set (DEBUG_FLAGS -DNDEBUG -ggdb3 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wno-format-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wno-switch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wno-unused-parameter -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fPIC -fno-omit-frame-pointer -pie -fPIE -Werror=vla -Wno-write-strings -fsanitize=address,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr)
set (RELEASE_FLAGS -O3 -std=c++17)

add_subdirectory (libs)

# -----------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------- Lib configuration -------------------------------------------------------
# -----------------------------------------------------------------------------------------------------------------------------
add_library (${PROJECT_NAME} INTERFACE)

target_include_directories (${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/HashTable ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries (${PROJECT_NAME} INTERFACE list-lib ColorConsole)

# -----------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------- Gtests configuration ----------------------------------------------------
# -----------------------------------------------------------------------------------------------------------------------------

enable_testing ()
add_executable (UnitTests ${CMAKE_CURRENT_SOURCE_DIR}/UnitTests.cpp)
target_include_directories (UnitTests PRIVATE ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options (UnitTests PRIVATE $<$<CONFIG:Debug>:${DEBUG_FLAGS}>)
target_compile_options (UnitTests PRIVATE $<$<CONFIG:Release>:${RELEASE_FLAGS}>)

target_link_options (UnitTests PRIVATE $<$<CONFIG:Debug>:${DEBUG_FLAGS}>)
target_link_options (UnitTests PRIVATE $<$<CONFIG:Release>:${RELEASE_FLAGS}>)

target_link_libraries (UnitTests PRIVATE ${PROJECT_NAME})
target_link_libraries (UnitTests PRIVATE gtest gtest_main)

include (GoogleTest)
gtest_discover_tests (UnitTests)

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set_target_properties (${PROJECT_NAME} PROPERTIES
                       ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                       LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                       RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")



