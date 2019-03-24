add_library(wren_module_readline STATIC readline/wren_module_readline.c)
target_include_directories(wren_module_readline PUBLIC "readline")
target_link_libraries(wren_module_readline PUBLIC wauxlib readline)

set(wren_module_posix_SRCS
    posix/wren_module_posix.c
    posix/wren_module_posix_shell.c
    posix/wren_module_posix_socket.c
    posix/util.c
)
add_library(wren_module_posix SHARED ${wren_module_posix_SRCS})
target_include_directories(wren_module_posix PUBLIC "posix")
target_link_libraries(wren_module_posix PUBLIC wauxlib_plugin_interface)

add_custom_target(uname ALL
    COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/uname/uname.wren" "${CMAKE_CURRENT_BINARY_DIR}/uname.wren"
)
