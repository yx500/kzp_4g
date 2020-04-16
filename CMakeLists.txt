get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

include(my_boost REQUIRED)

include_directories( ../gtcommon )
include_directories( ../gtlan )
include_directories( ../vx )


file(GLOB ${TARGET_NAME}_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    )


add_executable (${TARGET_NAME} ${${TARGET_NAME}_SRC} )
target_link_libraries (${TARGET_NAME}
    gtcommon
    gtlan
    vx
    ${Boost_LIBRARIES}
    ${CMAKE_REQUIRED_LIBRARIES}
    )



