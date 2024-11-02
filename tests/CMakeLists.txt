enable_testing()
find_package(GTest REQUIRED)

add_executable(
  vvtest
  testCollision.cpp
  testColor.cpp
  testGameOptions.cpp
  testMath.cpp
  testViper.cpp
  testTime.cpp
  testTrack.cpp
)
target_link_libraries(
  vvtest
  libvvipers
  ${GTEST_MAIN_LIBRARIES}
)
target_include_directories(vvtest PRIVATE ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR}/include/)
gtest_discover_tests(vvtest)
configure_file(test.json ${CMAKE_CURRENT_BINARY_DIR}/test.json COPYONLY)
configure_file(../res/preferences.json ${CMAKE_CURRENT_BINARY_DIR}/preferences.json COPYONLY)

add_custom_command(TARGET vvtest 
  POST_BUILD
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/vvtest
)