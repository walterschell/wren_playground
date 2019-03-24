#ifndef WREN_MODULE_POSIX_INT_H_
#define WREN_MODULE_POSIX_INT_H_
#include <stdbool.h>
#include <wren.h>
#define WREN_ERROR(VM, MSG) \
         wrenSetSlotString(VM, 0, MSG); \
         wrenAbortFiber(VM, 0); \
         return;
#define HIDDEN __attribute__((visibility ("hidden")))

HIDDEN bool is_readable(int fd);

HIDDEN void wren_Socket_allocate(WrenVM *vm);
HIDDEN void wren_Socket_finalize(void *data);
HIDDEN void wren_Socket_accept_to_fd(WrenVM *vm);
HIDDEN void wren_Socket_connect(WrenVM *vm);
HIDDEN void wren_Socket_listen(WrenVM *vm);
HIDDEN void wren_Socket_send(WrenVM *vm);
HIDDEN void wren_Socket_recvall(WrenVM *vm);
HIDDEN void wren_Socket_recv(WrenVM *vm);
HIDDEN void wren_Socket_is_readable(WrenVM *vm);

HIDDEN void wren_Shell_allocate(WrenVM *vm);
HIDDEN void wren_Shell_finalize(void *data);
HIDDEN void wren_Shell_write_stdin(WrenVM *vm);
HIDDEN void wren_Shell_read_stdout(WrenVM *vm);
HIDDEN void wren_Shell_read_stderr(WrenVM *vm);
HIDDEN void wren_Shell_is_readable(WrenVM *vm);

#endif
