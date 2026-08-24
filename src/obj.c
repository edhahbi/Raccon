#include "obj.h"

object* create_object(size_t size, const property* const properties){
    object* obj = malloc(sizeof(object));
    obj->size = size;
    obj->properties = properties;
    return obj;
}

