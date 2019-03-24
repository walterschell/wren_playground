#include <wren.h>
#include <wauxlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

static const char *readlineModuleSource = \
"class readline { \n"
"    foreign static prompt(message) \n"
"} \n"
;

 static void wren_readline_prompt(WrenVM *vm)
{
    const char *msg = wrenGetSlotString(vm, 1);
    char *response = readline(msg);

    wrenSetSlotString(vm, 0, response);
    free(response);
}

static ModuleRegistry module_readline[] = {
    MODULE(readline)
        CLASS(readline)
            STATIC_METHOD("prompt(_)", wren_readline_prompt)
        END_CLASS
    END_MODULE
    SENTINEL_MODULE
};

void wren_module_readline_register(WauxlibBinderCtx *binderCtx)
{
    defaultBinderAddModule(binderCtx, "readline", NULL, (void *) readlineModuleSource, NULL, NULL, module_readline);
}

