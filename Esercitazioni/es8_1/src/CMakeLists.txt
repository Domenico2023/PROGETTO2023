list(APPEND es8_1_headers ${CMAKE_CURRENT_SOURCE_DIR}/convexHull.hpp)
list(APPEND es8_1_headers ${CMAKE_CURRENT_SOURCE_DIR}/sorting.hpp)
list(APPEND es8_1_headers ${CMAKE_CURRENT_SOURCE_DIR}/test_convexHull.hpp)

list(APPEND es8_1_sources ${CMAKE_CURRENT_SOURCE_DIR}/convexHull.cpp)

list(APPEND es8_1_includes ${CMAKE_CURRENT_SOURCE_DIR})

set(es8_1_sources ${es8_1_sources} PARENT_SCOPE)
set(es8_1_headers ${es8_1_headers} PARENT_SCOPE)
set(es8_1_includes ${es8_1_includes} PARENT_SCOPE)
