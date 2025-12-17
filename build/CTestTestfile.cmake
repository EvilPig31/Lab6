# CMake generated Testfile for 
# Source directory: /workspaces/Lab6
# Build directory: /workspaces/Lab6/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(MyProjectTests "/workspaces/Lab6/build/tests")
set_tests_properties(MyProjectTests PROPERTIES  _BACKTRACE_TRIPLES "/workspaces/Lab6/CMakeLists.txt;42;add_test;/workspaces/Lab6/CMakeLists.txt;0;")
subdirs("_deps/googletest-build")
