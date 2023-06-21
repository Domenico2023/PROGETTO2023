list(APPEND es5_2_headers ${CMAKE_CURRENT_SOURCE_DIR}/shape.hpp)
list(APPEND es5_2_headers ${CMAKE_CURRENT_SOURCE_DIR}/test_shape.hpp)

list(APPEND es5_2_sources ${CMAKE_CURRENT_SOURCE_DIR}/shape.cpp)

list(APPEND es5_2_includes ${CMAKE_CURRENT_SOURCE_DIR})

set(es5_2_sources ${es5_2_sources} PARENT_SCOPE)
set(es5_2_headers ${es5_2_headers} PARENT_SCOPE)
set(es5_2_includes ${es5_2_includes} PARENT_SCOPE)
