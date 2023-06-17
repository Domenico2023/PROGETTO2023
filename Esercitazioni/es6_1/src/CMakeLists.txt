list(APPEND es6_1_headers ${CMAKE_CURRENT_SOURCE_DIR}/web.hpp)
list(APPEND es6_1_headers ${CMAKE_CURRENT_SOURCE_DIR}/sorting.hpp)
list(APPEND es6_1_headers ${CMAKE_CURRENT_SOURCE_DIR}/test_pagerank.hpp)

list(APPEND es6_1_sources ${CMAKE_CURRENT_SOURCE_DIR}/web.cpp)

list(APPEND es6_1_includes ${CMAKE_CURRENT_SOURCE_DIR})

set(es6_1_sources ${es6_1_sources} PARENT_SCOPE)
set(es6_1_headers ${es6_1_headers} PARENT_SCOPE)
set(es6_1_includes ${es6_1_includes} PARENT_SCOPE)
