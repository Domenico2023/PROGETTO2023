list(APPEND raffinamento_headers ${CMAKE_CURRENT_SOURCE_DIR}/mesh_classes.hpp)
list(APPEND raffinamento_headers ${CMAKE_CURRENT_SOURCE_DIR}/sorting.hpp)

list(APPEND raffinamento_sources ${CMAKE_CURRENT_SOURCE_DIR}/mesh_classes.cpp)

list(APPEND raffinamento_includes ${CMAKE_CURRENT_SOURCE_DIR})

set(raffinamento_sources ${raffinamento_sources} PARENT_SCOPE)
set(raffinamento_headers ${raffinamento_headers} PARENT_SCOPE)
set(raffinamento_includes ${raffinamento_includes} PARENT_SCOPE)
