file(GLOB_RECURSE DATABASE_CORE_SOURCES CONFIGURE_DEPENDS "*.cpp")
list(FILTER DATABASE_CORE_SOURCES EXCLUDE REGEX "/main\\.cpp$")

add_library(database ${DATABASE_CORE_SOURCES})
target_include_directories(database PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(database main.cpp)
target_link_libraries(database PRIVATE database)