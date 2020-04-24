cmake_minimum_required(VERSION 3.11)
set (CMAKE_CXX_STANDARD 17)

project(jitbuilder)

add_subdirectory(pybind11)

# set(COMPILER_FLAGS "-fPIC -Wall")
include_directories(/Users/daytonjallen/Tests/omr/omr/include_core ./include)

pybind11_add_module(jitbuilder /Users/daytonjallen/PyJitbuilder/py-jitbuilder/src/apigen/client/jitbuilder_bindings.cpp)
target_link_libraries(jitbuilder PRIVATE /Users/daytonjallen/omr/build/jitbuilder/libjitbuilder.a)
# set_target_properties(jitbuilder PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
set_target_properties(jitbuilder PROPERTIES CXX_VISIBILITY_PRESET default)
set_target_properties(jitbuilder PROPERTIES VISIBILITY_INLINES_HIDDEN default)


