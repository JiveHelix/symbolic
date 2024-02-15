add_library(symbolic)

if (${fPIC})
    set_property(TARGET symbolic PROPERTY POSITION_INDEPENDENT_CODE ON)
endif ()

# Projects that include this project must #include "symbolic/<header-name>"
target_include_directories(symbolic PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(
    symbolic
    PUBLIC
    project_warnings
    project_options
    CONAN_PKG::jive
    CONAN_PKG::fmt)

target_sources(
    symbolic
    PRIVATE
    angle_sums.cpp
    greek.cpp
    matrix.cpp
    named.cpp
    expression.cpp
    settings.cpp
    symbol.cpp
    value.cpp)

install(TARGETS symbolic DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/symbolic
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
