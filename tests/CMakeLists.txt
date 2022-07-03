enable_testing()
find_package(GTest REQUIRED)

add_executable(
  vvtest
  test.cpp
)
target_link_libraries(
  vvtest
  libvvipers
  ${GTEST_MAIN_LIBRARIES}
)

gtest_discover_tests(vvtest)