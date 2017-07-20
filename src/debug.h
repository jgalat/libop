#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>

#ifdef VERBOSE
#define __DEBUG(m) (fprintf(stderr, "DEBUG [%s:%d]: %s\n", __FILE__, __LINE__, m))
#else
#define __DEBUG(m) ((void)(m))
#endif

#endif /* __DEBUG_H__ */
