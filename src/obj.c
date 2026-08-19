#include "obj.h"

object create_object(size_t size, property* const properties){
    object obj;
    obj.size = size;
    obj.properties = properties;
    return obj;
}
