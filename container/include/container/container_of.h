// 2021/9/3
// zhangzhong
// container_of.h

#ifndef _CONTAINER_OF_H_
#define _CONTAINER_OF_H_

#include <stddef.h>

/**
 * @brief   get struct ptr from its member ptr
 * @ptr:    struct member's address
 * @type:   struct type
 * @member: struct member name
 */
#define container_of(ptr, type, member)                                        \
    (type*)((char*)ptr - offsetof(type, member))

#endif // _CONTAINER_OF_H_
