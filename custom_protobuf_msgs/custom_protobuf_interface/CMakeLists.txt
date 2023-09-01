cmake_minimum_required(VERSION 3.8)
project(custom_protobuf_interface)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# This example repository uses ament.
# If this is undesirable for your use case (e.g. using Gazebo Sim without ROS 2),
# then you will have to find another example repository.
find_package(ament_cmake REQUIRED)

# This example has a dependency on gz-msgs, since custom message proto uses
# message types from gz-msgs (gz.msgs.Header and gz.msgs.Pose).
# If your project depends on other custom message libraries, add find_package()
# statements accordingly, e.g.: find_package(your_custom_msg)
find_package(gz-msgs9 REQUIRED)


# Below statement is kept for documentation purposes:
# I think it is generally not good-practice to use the include_directories()
# function, and preferred to add the necessary include directories to the 
# target_include_directories() call instead:
#
#include_directories(
#  ${gz-msgs9_INCLUDE_DIRS}
#)


# Add the subdirectory
# Its CMakeLists.txt will be processed.
# The subdir's CMakeLists.txt holds 'set()' commands with 'PARENT_SCOPE' keyword.
# This causes the corresponding variables (e.g. ${PROTO_SOURCES}) to be defined
# also in the context of this CMakeLists.txt
add_subdirectory(proto)
set_source_files_properties(${PROTO_SOURCES} ${PROTO_HEADERS}
                            PROPERTIES GENERATED TRUE)

# This defines a target 'custom_msg_lib' as a shared library, 
# to be compiled from ${PROTO_SOURCES}
add_library(custom_msg_lib SHARED ${PROTO_SOURCES})

# This adds a dependency of the shared library to the protobuf generated files
# from the CMakeLists.txt in the proto subfolder.
add_dependencies(custom_msg_lib ${PROJECT_NAME}_protobuf_compilation)

# This defines the include directory for the shared library.
# If your project depends on other custom message libraries, add variables
# accordingly (e.g. ${your_custom_msg_INCLUDE_DIRS}).
target_include_directories(custom_msg_lib PRIVATE ${gz-msgs9_INCLUDE_DIRS})

# This ensures that the proto headers are installed into the
# 'install/custom_protobuf_interface/include/custom_protobuf_interface/msgs' folder
install(
  FILES
    ${PROTO_HEADERS}
  DESTINATION include/${PROJECT_NAME}/msgs
)

# This ensures that the compiled library is installed into the
# 'install/custom_protobuf_interface/lib' folder.
# The 'EXPORT' statement is needed to define a target name 
# for the ament_export_targets() call below. This name should not be
# identical to the library target (hence the suffix '_export').
# Not sure why I must append ${PROJECT_NAME} in the downstream repository
# (i.e. DESTINATION lib/${PROJECT_NAME}), but must **not** append this in
# this repository.
install(
  TARGETS custom_msg_lib
  EXPORT custom_msg_lib_export
  DESTINATION lib
)

# According to the ament documentastion, following statement should not be needed.
# https://docs.ros.org/en/iron/How-To-Guides/Ament-CMake-Documentation.html
# However, I did not succeed to omit this statement, as without it
# ${custom_protobuf_interface_INCLUDE_DIRS} is not defined in the downstream
# package on call of 'find_package(custom_protobuf_interface)'
ament_export_include_directories(include)

# Following statement is needed so 'custom_protobuf_interface::custom_msg_lib' is
# defined in the  downstream package, so it can be added to the
# 'target_link_libraries()' call in that CMakeLists.txt
ament_export_targets(custom_msg_lib_export HAS_LIBRARY_TARGET)


# I assumed this would be necessary so the downstream package does not
# need 'find_package(gz-msgs9)' but it seems it is not necessary anyway
#ament_export_dependencies(gz-msgs9)


# This is what everybody copy-pastes.
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

# These hooks ensure the environment variables are updated properly, upon
# issuing the 'source install/setup.bash' command
ament_environment_hooks("${CMAKE_CURRENT_SOURCE_DIR}/hooks/${PROJECT_NAME}.dsv.in")
ament_environment_hooks("${CMAKE_CURRENT_SOURCE_DIR}/hooks/${PROJECT_NAME}.sh.in")

# This is always necessary as final command for ament.
ament_package()
