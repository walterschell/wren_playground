cmake_minimum_required(VERSION 2.8.12)
project(WrenPlayground "C")
add_subdirectory(deps/wren)
add_subdirectory(modules)

set(wren_playground_SRCS 
src/wren_playground.c
)

add_executable(wren_playground ${wren_playground_SRCS})
target_link_libraries(wren_playground wren wauxlib)

target_link_libraries(wren_playground wren_module_readline)

function(add_wrenfile wrenfile)
    set(cpysrc ${CMAKE_CURRENT_SOURCE_DIR}/${wrenfile})
    get_filename_component(wrenfilename ${wrenfile} NAME)
        set(OUTPATH ${CMAKE_CURRENT_BINARY_DIR}/${wrenfilename})
        add_custom_command(OUTPUT ${OUTPATH}
        COMMAND cp ${cpysrc} ${OUTPATH}
        DEPENDS ${cpysrc}
    )
    add_custom_target(${wrenfilename}-install ALL
        DEPENDS ${OUTPATH}
    )
endfunction(add_wrenfile wrenfile)


add_wrenfile(wren_scripts/demo_readline.wren)