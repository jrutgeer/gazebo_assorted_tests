cmake_minimum_required(VERSION 3.8)
project(custom_protobuf_downstream)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# This example repository uses ament.
# If this is undesirable for your use case (e.g. using Gazebo Sim without ROS 2),
# then you will have to find another example repository.
find_package(ament_cmake REQUIRED)

# gz-sim is needed as the example is a gz::sim::system plugin
find_package(gz-sim7 REQUIRED)

# This system uses messages defined in the custom_protobuf_interface package
find_package(custom_protobuf_interface)


# This defines a target 'custom_msg_system' as a shared library, 
# to be compiled from custom_msg_system.cc
add_library(custom_msg_system
  SHARED
  src/custom_msg_system.cc
)


# This is probably an alternative to below 'target_include_directories()' and 
# 'target_link_libraries()' statements, but I could not get that to work...
#ament_target_dependencies(custom_msg_system PUBLIC gz-sim7 custom_protobuf_interface)


# This defines 'include' and ${custom_protobuf_interface_INCLUDE_DIRS} as include directories
# when compiling 'custom_msg_system'.
# See remark about 'ament_export_include_directories()' in the upstream CMakeLists.txt
target_include_directories(
  custom_msg_system PUBLIC
  include
  ${custom_protobuf_interface_INCLUDE_DIRS}
)

# This defines the libraries needed to link the custom_msg_system target.
# See remark about 'ament_export_targets()' in the upstream CMakeLists.txt
# This seems to also affect include directories, 
target_link_libraries(custom_msg_system PRIVATE
  gz-sim7::gz-sim7
  custom_protobuf_interface::custom_msg_lib)


# This ensures that the compiled library is installed into the
# 'install/custom_protobuf_downstream/lib/custom_protobuf_downstream/' folder.
# Not sure why I must append ${PROJECT_NAME} here, but must not in the upstream
# repository.
install(
  TARGETS custom_msg_system
  DESTINATION lib/${PROJECT_NAME}
)


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
