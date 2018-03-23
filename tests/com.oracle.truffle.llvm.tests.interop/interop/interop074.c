#include <polyglot.h>
#include <truffle.h>

typedef void *VALUE;

VALUE global;
VALUE** global_array;

int main() {
    global = polyglot_import("object");
    global_array = truffle_managed_malloc(sizeof(VALUE *) * 2);

    global_array[0] = &global;
    global_array[1] = NULL;

    void (*returnObject)(void *) = polyglot_import("returnObject");
    returnObject(global);

    int index = 0;

    //return truffle_is_truffle_object(global_array[index]);
    return global_array[index] == NULL;
}
