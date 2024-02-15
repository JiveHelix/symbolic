add_executable(rotations rotations.cpp)

target_link_libraries(
    rotations
    PUBLIC
    project_warnings
    project_options
    symbolic
    CONAN_PKG::tau)


add_executable(angle_sums angle_sums.cpp)

target_link_libraries(
    angle_sums
    PUBLIC
    project_warnings
    project_options
    symbolic)


add_executable(algebra algebra.cpp)

target_link_libraries(
    algebra
    PUBLIC
    project_warnings
    project_options
    symbolic)


add_executable(greek greek.cpp)

target_link_libraries(
    greek
    PUBLIC
    project_warnings
    project_options
    symbolic)

install(TARGETS rotations DESTINATION ${CMAKE_INSTALL_BINDIR})
install(TARGETS greek DESTINATION ${CMAKE_INSTALL_BINDIR})
