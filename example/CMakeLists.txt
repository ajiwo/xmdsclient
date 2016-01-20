add_executable(example main.c)
target_link_libraries(example LINK_PUBLIC xmdsclient ${CURL_LIBRARIES} ${LIBXML2_LIBRARIES})
set_target_properties(example PROPERTIES  COMPILE_FLAGS "-Wall -ansi -pedantic -pedantic-errors -I${CMAKE_SOURCE_DIR}/include")

foreach(dep_include_dir ${xmdsclient_dep_incdir})
    get_target_property(current_cflags example COMPILE_FLAGS)
    set_target_properties(example PROPERTIES COMPILE_FLAGS "${current_cflags} -I${dep_include_dir}")
endforeach()

