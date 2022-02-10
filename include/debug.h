#ifndef DEBUG_H_HDIFHD
#define DEBUG_H_HDIFHD

#ifdef debug_mode
    #define debug(x, format) printf("debug => "#x" = "#format"\n", (x))
#else
    #define debug(x, format)
#endif

#define llu_debug(x) debug(x, %llu)

#endif