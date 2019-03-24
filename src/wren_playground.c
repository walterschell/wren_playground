
#include <stdio.h>
#include <assert.h>
#include <wren.h>
#include <wauxlib.h>
#include <wren_module_readline.h>
#include <dlfcn.h>


void wren_print_error(
    WrenVM* vm, WrenErrorType type, const char* module, int line,
    const char* message)
{
    switch (type)
    {
        case WREN_ERROR_COMPILE:
            printf("Compile Error: (%s:%d) %s\n", module, line, message);
        break;

        case WREN_ERROR_RUNTIME:
            printf("Runtime Error: %s\n", message);
        break;

        case WREN_ERROR_STACK_TRACE:
            printf("=> (%s:%d) %s\n", module, line, message);
        break;

        default:
            assert(false);
        break;
    }
}
void wren_write(WrenVM* vm, const char* text)
{
    printf("%s", text);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <script>\n", argv[0]);
        return -1;
    }
    FILE *fh = fopen(argv[1], "rb");
    if (NULL == fh)
    {
        printf("Error opening %s\n", argv[1]);
        return -2;
    }
    //TODO: Fix this
    char program[4096];
    fread(program, 1, 4095, fh);
    fclose(fh);
    
    WrenConfiguration config;
    WauxlibBinderCtx *binderCtx = defaultBinderNew();
    wren_module_readline_register(binderCtx);

    wrenInitConfiguration(&config);
    config.bindForeignClassFn = defaultBinderBindForeignClassFn;
    config.bindForeignMethodFn = defaultBinderBindForeignMethodFn;
    config.loadModuleFn = defaultBinderLoadModuleFn;
    config.loaderCtx = binderCtx;
    config.binderCtx = binderCtx;
    config.errorFn = wren_print_error;
    config.writeFn = wren_write;
    WrenVM* vm = wrenNewVM(&config);
    
    if (WREN_RESULT_SUCCESS == wrenInterpret(vm, "main", program))
    {
        printf("Program ran successfully\n");
    }
    else
    {
        printf("Program failed to run\n");
    }
    defaultBinderDelete(binderCtx);
    wrenFreeVM(vm);  
}