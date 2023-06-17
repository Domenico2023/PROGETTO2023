list(APPEND es9_1_headers ${CMAKE_CURRENT_SOURCE_DIR}/quadrature.hpp)
list(APPEND es9_1_headers ${CMAKE_CURRENT_SOURCE_DIR}/mapTriangle.hpp)
list(APPEND es9_1_headers ${CMAKE_CURRENT_SOURCE_DIR}/gauss_quadrature_1D.hpp)
list(APPEND es9_1_headers ${CMAKE_CURRENT_SOURCE_DIR}/test_quadrature.hpp)

list(APPEND es9_1_sources ${CMAKE_CURRENT_SOURCE_DIR}/quadrature.cpp)
list(APPEND es9_1_headers ${CMAKE_CURRENT_SOURCE_DIR}/mapTriangle.cpp)

list(APPEND es9_1_includes ${CMAKE_CURRENT_SOURCE_DIR})

set(es9_1_sources ${es9_1_sources} PARENT_SCOPE)
set(es9_1_headers ${es9_1_headers} PARENT_SCOPE)
set(es9_1_includes ${es9_1_includes} PARENT_SCOPE)
