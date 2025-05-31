#ifndef STDARG_H
#define STDARG_H

typedef unsigned char* va_list;

#define va_start(ap, last) (ap = (va_list)&last + sizeof(last))
#define va_arg(ap, type) (*(type*)((ap += sizeof(type)) - sizeof(type)))
#define va_end(ap) (ap = 0)

#endif //STDARG_H