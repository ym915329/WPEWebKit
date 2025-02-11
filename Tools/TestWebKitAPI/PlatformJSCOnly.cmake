set(TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestWebKitAPI")
set(TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY_WTF "${TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY}/WTF")

include_directories(
    ${DERIVED_SOURCES_DIR}/ForwardingHeaders
    "${WTF_DIR}/icu"
)

if (LOWERCASE_EVENT_LOOP_TYPE STREQUAL "glib")
    include_directories(SYSTEM
        ${GLIB_INCLUDE_DIRS}
    )
endif ()

set(test_main_SOURCES
    ${TESTWEBKITAPI_DIR}/jsconly/main.cpp
)

list(APPEND TestWTF_SOURCES
    ${TESTWEBKITAPI_DIR}/jsconly/PlatformUtilitiesJSCOnly.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/RunLoop.cpp
)
