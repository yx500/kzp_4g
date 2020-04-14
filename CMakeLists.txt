get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

include(my_boost REQUIRED)

include_directories( ../gtcommon )
include_directories( ../gtlan )
include_directories( ../vx )


file(GLOB ${TARGET_NAME}_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB ${TARGET_NAME}_HDR "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
file(GLOB ${TARGET_NAME}_HDR "${CMAKE_CURRENT_SOURCE_DIR}/bdaq/*.h")


add_executable (${TARGET_NAME} ${${TARGET_NAME}_SRC} ${${TARGET_NAME}_HDR})
target_link_libraries (${TARGET_NAME}
    gtcommon
    gtlan
    vx
    ${Boost_LIBRARIES}
    ${CMAKE_REQUIRED_LIBRARIES}
    )



