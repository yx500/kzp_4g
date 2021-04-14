project(kzp_4g VERSION 1.0)

include(my_boost)

file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    )


add_executable (${PROJECT_NAME} ${SOURCES} )
target_link_libraries (${PROJECT_NAME}
    gtcommon
    gtlan
    vx
    Boost::program_options
    )


install(TARGETS ${PROJECT_NAME} DESTINATION bin)


