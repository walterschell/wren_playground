#include <poll.h>
#include "wren_module_posix_int.h"

HIDDEN bool is_readable(int fd)
{
    struct pollfd poller;
    poller.fd = fd;
    poller.events = POLLIN;
    int retval = poll(&poller, 1, 0);
    if (1 == retval)
    {
        return true;
    }
    return false;
}


