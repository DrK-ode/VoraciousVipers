enable_testing()
find_package(GTest REQUIRED)

add_executable(
  vvtest
  testMath.cpp
)
target_link_libraries(
  vvtest
  libvvipers
  ${GTEST_MAIN_LIBRARIES}
)
target_include_directories(vvtest PRIVATE ${PROJECT_SOURCE_DIR})
gtest_discover_tests(vvtest)

add_custom_command(TARGET vvtest 
  POST_BUILD
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/vvtest
)