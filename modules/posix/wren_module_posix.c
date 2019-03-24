#include "wren_module_posix_int.h"
#include <wauxlib.h>
static const char wren_module_posix_source[] = \
"foreign class Shell {\n"
"    construct create(){}\n"
"    foreign write_stdin(data)\n"
"    foreign read_stdout()\n"
"    foreign read_stderr()\n"
"    foreign is_readable()\n"
"}\n"
"\n"
"foreign class Socket {\n"
"    construct create() {}\n"
"    construct create_fd(fd) {}\n"
"    foreign connect(ip, port)\n"
"    foreign listen(port)\n"
"    foreign accept_to_fd()\n"
"    accept() {\n"
"        return Socket.create_fd(accept_to_fd())\n"
"    }\n"
"}\n"
;

static ModuleRegistry module_posix[] = {
    MODULE(posix)
        CLASS(Shell)
            STATIC_METHOD("<allocate>", wren_Shell_allocate)
            FINALIZER(wren_Shell_finalize)
            METHOD("write_stdin(_)", wren_Shell_write_stdin)
            METHOD("read_stdout()", wren_Shell_read_stdout)
            METHOD("read_stderr()", wren_Shell_read_stderr)
            METHOD("is_readable()", wren_Shell_is_readable)
        END_CLASS
        CLASS(Socket)
            STATIC_METHOD("<allocate>", wren_Socket_allocate)
            FINALIZER(wren_Socket_finalize)
            METHOD("connect(_,_)", wren_Socket_connect)
            METHOD("listen(_)", wren_Socket_listen)
            METHOD("accept_to_fd()", wren_Socket_accept_to_fd)
        END_CLASS
    END_MODULE
    SENTINEL_METHOD
};

WrenPluginInfo wren_mod_config = {
    .wren_version_number = WREN_VERSION_NUMBER,
    .loadModuleFnCtx = (void *) wren_module_posix_source,
    .bindForeignCtx = module_posix,
};