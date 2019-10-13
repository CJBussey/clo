list(APPEND files_inc_clo clo/pattern.hpp)

add_library( clo INTERFACE )
target_include_directories( clo INTERFACE . )