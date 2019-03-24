#include <wren.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include "wren_module_posix_int.h"


typedef struct 
{
    pid_t pid;
    int stdin_fd;
    int stdout_fd;
    int stderr_fd;
} wren_Shell_data_t;

void wren_Shell_allocate(WrenVM *vm)
{
    wren_Shell_data_t *ctx = wrenSetSlotNewForeign(vm, 0, 0, sizeof(wren_Shell_data_t));
    int stdin_pipe[2];
    int stdout_pipe[2];
    int stderr_pipe[2];
    pipe(stdin_pipe);
    pipe(stdout_pipe);
    pipe(stderr_pipe);
    pid_t pid = fork();
    if (pid < 0)
    {
        wrenSetSlotString(vm, 0, "Unable to fork");
        wrenAbortFiber(vm, 0);
        return;
    }
    if (pid == 0)
    {
        dup2(stdin_pipe[0], 0);
        dup2(stdout_pipe[1], 1);
        dup2(stderr_pipe[1],2);
        dup2(1,2);

        close(stdin_pipe[0]);
        close(stdin_pipe[1]);
        close(stdout_pipe[0]);
        close(stdout_pipe[1]);
        close(stderr_pipe[0]);
        close(stderr_pipe[1]);
        char *argv[] = {
            "/bin/bash",
            NULL
        };
        execve("/bin/bash", argv, __environ);
        printf("Unable to execute");
        _exit(-1);        
    }
    close(stdin_pipe[0]);
    close(stdout_pipe[1]);
    close(stderr_pipe[1]);
    ctx->pid = pid;
    ctx->stdin_fd = stdin_pipe[1];
    ctx->stdout_fd = stdout_pipe[0];
    ctx->stderr_fd = stderr_pipe[0];
}

void wren_Shell_finalize(void *data)
{
    wren_Shell_data_t *ctx = data;
    close(ctx->stdin_fd);
    close(ctx->stdout_fd);
    close(ctx->stderr_fd);
    kill(ctx->pid, SIGTERM);
    int status;
    waitpid(ctx->pid, &status, 0);
}



void wren_Shell_write_stdin(WrenVM *vm)
{
    const char *buffer;
    int buffer_size;
    wren_Shell_data_t *ctx = wrenGetSlotForeign(vm, 0);
    buffer = wrenGetSlotBytes(vm, 1, &buffer_size);
    write(ctx->stdin_fd, buffer, buffer_size);
}

void wren_Shell_read_stdout(WrenVM *vm)
{
    printf("Reading stdout\n");
    char buffer[4096];
    int buffer_size;
    wren_Shell_data_t *ctx = wrenGetSlotForeign(vm, 0);
    buffer_size = read(ctx->stdout_fd, buffer, sizeof(buffer));
    printf("Read complete\n");
    if (buffer_size < 0)
    {
        wrenSetSlotString(vm, 0, "Unable to read from shell");
        wrenAbortFiber(vm, 0);
    }
    wrenSetSlotBytes(vm, 0, buffer, buffer_size);
}

void wren_Shell_read_stderr(WrenVM *vm)
{
    char buffer[4096];
    int buffer_size;
    wren_Shell_data_t *ctx = wrenGetSlotForeign(vm, 0);
    buffer_size = read(ctx->stderr_fd, buffer, sizeof(buffer));
    if (buffer_size < 0)
    {
        wrenSetSlotString(vm, 0, "Unable to read from shell");
        wrenAbortFiber(vm, 0);
    }
    wrenSetSlotBytes(vm, 0, buffer, buffer_size);

}

void wren_Shell_is_readable(WrenVM *vm)
{
    wren_Shell_data_t *ctx = wrenGetSlotForeign(vm, 0);
    wrenSetSlotBool(vm, 0, is_readable(ctx->stdout_fd));
}