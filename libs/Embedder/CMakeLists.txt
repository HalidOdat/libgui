cmake_minimum_required(VERSION 3.5)

add_subdirectory(bin)

set(This Embedder)
add_library(${This} STATIC
  include/Asset.hpp
  include/RootAsset.hpp

  src/Asset.cpp
  src/RootAsset.cpp
)
target_include_directories(${This} PUBLIC include)

function(EmbedAssets)
  # Parse function arguments
  cmake_parse_arguments(
    ASSET                 # prefix of output variables
    ""                         # Boolean arguments
    "NAME;DIRECTORY;NAMESPACE" # Single value arguments
    ""                         # Multi-value arguments
    "${ARGN}"
  )

  # Check if the required arguments are provided
  if(NOT ASSET_NAME)
    message(FATAL_ERROR "You must provide a name for the embedded assets")
  endif()
  if(NOT ASSET_DIRECTORY)
    message(FATAL_ERROR "You must provide the asset directory")
  endif()

  # Make sure there are no unpaired arguments, if that happens it's probably a bug.
  foreach(arg IN LISTS ASSET_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "${arg} upaired argument, did you forget to add NAME, DIRECTORY, or NAMESPACE prefix?")
  endforeach()

  # Define the output locations
  set(ASSET_OUTPUT_DIR ${CMAKE_BINARY_DIR}/EmbeddedAssets/${ASSET_NAME})
  set(ASSET_SOURCE ${ASSET_OUTPUT_DIR}/assets.cpp)
  set(ASSET_HEADER ${ASSET_OUTPUT_DIR}/${ASSET_NAME}/assets.hpp)

  # Make sure the required directories are created
  file(MAKE_DIRECTORY ${ASSET_OUTPUT_DIR}/${ASSET_NAME})

  add_custom_command(
    OUTPUT  ${ASSET_HEADER} ${ASSET_SOURCE}  
    COMMAND $<TARGET_FILE:EmbedderBin>  
    ARGS    ${ASSET_DIRECTORY} ${ASSET_SOURCE} ${ASSET_HEADER} ${ASSET_NAMESPACE}
    DEPENDS ${ASSET_DIRECTORY}
    COMMENT "Generating embedded assets of ${ASSET_DIRECTORY} directory as ${ASSET_NAME} library"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    VERBATIM
  )

  add_library(${ASSET_NAME} STATIC
    ${ASSET_HEADER}
    ${ASSET_SOURCE}
  )
  target_include_directories(${ASSET_NAME} PUBLIC ${ASSET_OUTPUT_DIR})
  target_link_libraries(${ASSET_NAME} PUBLIC Embedder)
endfunction()
