

# This was copied from the SubT Challenge repository:
# https://github.com/osrf/subt/blob/master/subt-communication/subt_communication_broker/src/protobuf/CMakeLists.txt
# Documentation was added, and slight changes were made to make it work for this standalone repository.

include(${PROJECT_SOURCE_DIR}/cmake/Protobuf.cmake)


# This defines which .proto files should be processed
set(PROTO_MESSAGES
  DetectedObject.proto
)

# See doc in above included Protobuf.cmake file:
# This generates the source, header and descriptor files for proto
# messages, and populates the ${PROTO_SOURCES}, ${PROTO_HEADERS} and
# ${PROTO_DESCRIPTORS} variables.
# If required for your project, add extra import directories to 
# IMPORT_DIRS to import other custom message definitions.
protobuf_generate_cpp_with_descriptor(
  PROTO_SOURCES
  PROTO_HEADERS
  PROTO_DESCRIPTORS
  IMPORT_DIRS
    ${gz-msgs9_INCLUDE_DIRS}
  ${PROTO_MESSAGES}
)


# I do not understand following comment from the original file:
#   # Variables needed to propagate through modules
#    # If more than one layer of cmake use CACHE INTERNAL ...
#
# But in any case: the 'PARENT_SCOPE' statement causes that these variables get
# defined in the context of the CMakeLists.txt in the parent folder
set(PROTOBUF_INCLUDE_DIRS ${PROTOBUF_INCLUDE_DIRS} PARENT_SCOPE)
set(PROTOBUF_LIBRARIES ${PROTOBUF_LIBRARIES} PARENT_SCOPE)
set(PROTO_SOURCES ${PROTO_SOURCES} PARENT_SCOPE)
set(PROTO_HEADERS ${PROTO_HEADERS} PARENT_SCOPE)


# According to the docs:
#    Adds a target with the given name that executes the given commands.
#    The target has no output and will always be built (even if the commands
#    try to create a file with the name of the target).
add_custom_target(${PROJECT_NAME}_protobuf_compilation ALL DEPENDS ${PROTO_SOURCES})
