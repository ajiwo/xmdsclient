cmake_minimum_required (VERSION 2.8.0)
project(xmdsclient)

set(xmdsclient_src src)
set(xmdsclient_example example)

set(xmdsclient_headers
    include/xmdsclient/xmds.h
    include/xmdsclient/xmds-node.h
    include/xmdsclient/xmds-util.h
    include/xmdsclient/xmds-params.h
    xml2-soap.h
    xmds-soap.h
    xml2-xmds.h
    soap-transport-curl.h
    soap-nsinfo.h
    xmds-params-func.h
    xml2-xmds-request.h
    xml2-xmds-response.h
    mbedtls-base64.h
    mbedtls-md5.h
)

set(xmdsclient_install_headers
    xmds.h
    xmds-node.h
    xmds-params.h
)

set(xmdsclient_srcs
    xmds.c
    xmds-util.c
    xml2-soap.c
    xml2-xmds.c
    soap-transport-curl.c
    xmds-soap.c
    soap-nsinfo.c
    xml2-xmds-request.c
    xml2-xmds-response.c
    xmds-params-func.c
    mbedtls-base64.c
    mbedtls-md5.c
    xmds-node.c
)

find_package(LibXml2 2.6.18 REQUIRED)
if(NOT LIBXML2_FOUND)
    message("++ libxml2 dev files are not found, exiting..")
endif()
find_package(CURL REQUIRED)
if(NOT CURL_FOUND)
    message("++ libcurl dev files are not found, exiting..")
endif()

set(xmdsclient_dep_incdir ${xmdsclient_dep_incdir} ${CURL_INCLUDE_DIRS})
set(xmdsclient_dep_incdir ${xmdsclient_dep_incdir} ${LIBXML2_INCLUDE_DIR})
set(xmdsclient_dep_incdir ${xmdsclient_dep_incdir} ${CMAKE_SOURCE_DIR}/include/xmdsclient)

add_subdirectory("${xmdsclient_src}")
add_subdirectory("${xmdsclient_example}")


