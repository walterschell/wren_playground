#include <wren.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "wren_module_posix_int.h"

void wren_Socket_allocate(WrenVM *vm)
{
    printf("In wren_Socket_allocate. Num Slots: %d\n", wrenGetSlotCount(vm));

    int *fd = wrenSetSlotNewForeign(vm, 0, 0, sizeof(int));
    if (wrenGetSlotCount(vm) == 2)
    {
        if (wrenGetSlotType(vm, 1) != WREN_TYPE_NUM)
        {
            WREN_ERROR(vm, "Socket.create_fd must be an integer");
        }
        *fd = wrenGetSlotDouble(vm, 1);
        printf("Initialized socket from pre-existing file descriptor %d\n", *fd);
    }
    else
    {
        *fd = socket(AF_INET, SOCK_STREAM, 0);
    }
    
}

void wren_Socket_finalize(void *data)
{
    int *fd = data;
    close(*fd);
}

void wren_Socket_accept_to_fd(WrenVM *vm)
{
    int *fd = wrenGetSlotForeign(vm, 0);
    int new_fd = accept(*fd, NULL, 0);
    if (fd < 0)
    {
        WREN_ERROR(vm, "Unable to accept");
    }
    wrenSetSlotDouble(vm, 0, new_fd);
}

void wren_Socket_connect(WrenVM *vm)
{
    int *fd = wrenGetSlotForeign(vm, 0);
    const char *ip_str = wrenGetSlotString(vm, 1);
    int port = wrenGetSlotDouble(vm, 2);
    printf("Connecting to %s:%d\n", ip_str, port);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip_str);
    addr.sin_port = htons(port);
    int retval = connect(*fd, (struct sockaddr *) &addr, sizeof(addr));
    if (retval < 0)
    {
        char errmsg[512];
        sprintf(errmsg, "Unable to connect to %s:%d", ip_str, port);
        wrenSetSlotString(vm, 0, errmsg);
        wrenAbortFiber(vm, 0);
        return;
    }
}

void wren_Socket_listen(WrenVM *vm)
{
    int *fd = wrenGetSlotForeign(vm, 0);
    int port = wrenGetSlotDouble(vm, 1);
    printf("Listening on port %d\n", port);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr =  INADDR_ANY;
    addr.sin_port = htons(port);
    int optval = 1;
    int optlen = sizeof(int);
    setsockopt(*fd, SOL_SOCKET, SO_REUSEADDR, &optval, optlen);
    if (0 != bind(*fd, (struct sockaddr *) &addr, sizeof(addr)))
    {
        WREN_ERROR(vm, "Error: unable to bind on socket");
    }

    if (0 != listen(*fd, 5))
    {
        WREN_ERROR(vm, "Error: failure on socket listen");
    }

}

void wren_Socket_send(WrenVM *vm)
{
    int *fd = wrenGetSlotForeign(vm, 0);
    const char *data;
    int data_size;
    data = wrenGetSlotBytes(vm, 1, &data_size);
    while (data_size > 0)
    {
        int retval = send(*fd, data, data_size, 0);
        if (retval < 0)
        {
            wrenSetSlotString(vm, 0, "Error Sending");
            wrenAbortFiber(vm, 0);
            return;
        }
        data += retval;
        data_size -= retval;
    }
}

void wren_Socket_recvall(WrenVM *vm)
{
    int *fd = wrenGetSlotForeign(vm, 0);
    int recv_size = wrenGetSlotDouble(vm, 1);
    char *data = malloc(recv_size);
    char *spot = data;
    int data_size = recv_size;
    while (data_size > 0)
    {
        int retval = recv(*fd, spot, data_size, 0);
        if (retval < 0)
        {
            wrenSetSlotString(vm, 0, "Error receiving");
            wrenAbortFiber(vm, 0);
            free(data);
            return;
        }
        if (retval == 0)
        {
            wrenSetSlotString(vm, 0, "Connection Closed");
            wrenAbortFiber(vm, 0);
            free(data);
            return;
        }
        spot += retval;
        data_size -= retval;
    }
    wrenSetSlotBytes(vm, 0, data, recv_size);
    free(data);
}

void wren_Socket_recv(WrenVM *vm)
{
    int *fd = wrenGetSlotForeign(vm, 0);
    int recv_size = wrenGetSlotDouble(vm, 1);
    char *data = malloc(recv_size);

    int retval = recv(*fd, data, recv_size, 0);
    if (retval < 0)
    {
        wrenSetSlotString(vm, 0, "Error receiving");
        wrenAbortFiber(vm, 0);
        free(data);
        return;
    }
    if (retval == 0)
    {
        wrenSetSlotString(vm, 0, "Connection Closed");
        wrenAbortFiber(vm, 0);
        free(data);
        return;
    }

    wrenSetSlotBytes(vm, 0, data, retval);
    free(data);
}

void wren_Socket_is_readable(WrenVM *vm)
{
    int *fd = wrenGetSlotForeign(vm, 0);
    wrenSetSlotBool(vm, 0, is_readable(*fd));
}