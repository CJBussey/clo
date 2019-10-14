add_custom_target(examples COMMENT "Build all examples.")

file(GLOB_RECURSE example_files "*.cpp")

foreach(file IN LISTS example_files)
  message("found example: " ${file})
  get_filename_component(example_name ${file} NAME_WE)
  set(example_name ${example_name}_example)
  add_executable(${example_name} EXCLUDE_FROM_ALL "${file}")
  add_dependencies(examples ${example_name})
  target_link_libraries(${example_name} clo )
  add_test("test/${example_name}" ${example_name})
endforeach()