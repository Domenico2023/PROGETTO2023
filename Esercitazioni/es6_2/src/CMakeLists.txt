list(APPEND es6_2_headers ${CMAKE_CURRENT_SOURCE_DIR}/sorting.hpp)
list(APPEND es6_2_headers ${CMAKE_CURRENT_SOURCE_DIR}/test_sorting.hpp)

list(APPEND es6_2_includes ${CMAKE_CURRENT_SOURCE_DIR})


set(es6_2_sources ${es6_2_sources} PARENT_SCOPE)
set(es6_2_headers ${es6_2_headers} PARENT_SCOPE)
set(es6_2_includes ${es6_2_includes} PARENT_SCOPE)
