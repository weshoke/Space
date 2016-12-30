add_executable(unit-test
    ${CMAKE_CURRENT_SOURCE_DIR}/blade_predicates.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/blade_product_bit_product.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/blade_product_product.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/basis_xform_conformal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_compile_options(unit-test PUBLIC ${DEBUG_COMPILER_OPTIONS})
target_include_directories(unit-test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_include_directories(unit-test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/brigand)
