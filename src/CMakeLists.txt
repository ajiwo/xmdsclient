add_library(xmdsclient SHARED ${xmdsclient_srcs})

target_link_libraries(xmdsclient ${CURL_LIBRARIES})
target_link_libraries(xmdsclient ${LIBXML2_LIBRARIES})

set_target_properties(xmdsclient PROPERTIES COMPILE_FLAGS "-Wall -ansi -pedantic -pedantic-errors")

foreach(dep_include_dir ${xmdsclient_dep_incdir})
    get_target_property(current_cflags xmdsclient COMPILE_FLAGS)
    set_target_properties(xmdsclient PROPERTIES COMPILE_FLAGS "${current_cflags} -I${dep_include_dir}")
endforeach()

CONFIGURE_FILE(
  "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.pc.in"
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc"
  @ONLY
)

install(
    TARGETS xmdsclient
    DESTINATION lib
    PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
)

install(
    FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc
    DESTINATION lib/pkgconfig
    PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
)

foreach(hfile ${xmdsclient_install_headers})
    set(hfiles ${hfiles} "${CMAKE_SOURCE_DIR}/include/xmdsclient/${hfile}")
endforeach()

install(
    FILES ${hfiles}
    DESTINATION include/xmdsclient
    PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
)

