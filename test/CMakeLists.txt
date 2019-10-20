add_custom_target(tests COMMENT "Build all unit tests.")

file(GLOB_RECURSE unit_test_files "*.cpp")

foreach(file IN LISTS unit_test_files)
  message("found unit test: " ${file})
  get_filename_component(test_name ${file} NAME_WE)
  set(test_name ${test_name}_test)
  add_executable(${test_name} EXCLUDE_FROM_ALL "${file}")
  add_dependencies(tests ${test_name})
  target_compile_definitions(${test_name} PUBLIC CATCH_CONFIG_MAIN)
  target_link_libraries(${test_name} c-lo catch2)
  add_test("test/${test_name}" ${test_name})
endforeach()