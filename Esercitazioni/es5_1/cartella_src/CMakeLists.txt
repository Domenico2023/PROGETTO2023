list(APPEND es5_1_headers ${CMAKE_CURRENT_SOURCE_DIR}/car.hpp)
list(APPEND es5_1_headers ${CMAKE_CURRENT_SOURCE_DIR}/test_car.hpp)

list(APPEND es5_1_sources ${CMAKE_CURRENT_SOURCE_DIR}/car.cpp)

list(APPEND es5_1_includes ${CMAKE_CURRENT_SOURCE_DIR})

set(es5_1_sources ${es5_1_sources} PARENT_SCOPE)
set(es5_1_headers ${es5_1_headers} PARENT_SCOPE)
set(es5_1_includes ${es5_1_includes} PARENT_SCOPE)
