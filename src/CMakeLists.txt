#macro to add frameworks on MAC
macro(ADD_FRAMEWORK fwname appname)
    INCLUDE_DIRECTORIES( /Developer/Headers/FlatCarbon )
    FIND_LIBRARY(${fwname}_LIBRARY ${fwname})
    MARK_AS_ADVANCED (${fwname}_LIBRARY)
    target_link_libraries( ${appname} ${${fwname}_LIBRARY} )
endmacro(ADD_FRAMEWORK)

add_executable( ${PROJ_NAME} ${PROJ_SOURCES} ${PROJ_HEADERS})
target_link_libraries( ${PROJ_NAME} ${GLFW_LIB_PATH} ${GLEW_LIB_PATH} )

# Buildrule, copy the res map to build dir
# we don't want to copy if we're building in the source dir
if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    add_custom_command(
            OUTPUT "${PROJ_OUT_PATH}/res/"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJ_PATH}/res/" "${PROJ_OUT_PATH}/res/"
            DEPENDS "${PROJ_PATH}/res/"
    )
    # files are only copied if a target depends on them
    add_custom_target("res" DEPENDS "${PROJ_OUT_PATH}/res/")
    # ensure that the primary target depends on the resources
    add_dependencies( ${PROJ_NAME} "res")
endif()

if(APPLE)
	add_framework(Cocoa ${PROJ_NAME})
	add_framework(OpenGL ${PROJ_NAME})
	add_framework(IOKit ${PROJ_NAME})
	add_framework(CoreVideo ${PROJ_NAME})
	add_framework(Carbon ${PROJ_NAME})
endif(APPLE)

install(TARGETS ${PROJ_NAME} DESTINATION ${PROJ_OUT_PATH})