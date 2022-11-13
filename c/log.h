#ifndef C_LOG_H
#define C_LOG_H

#include "queue.h"

typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_NONE
} log_level_t;

extern void logSetLevel(log_level_t log_level);
extern log_level_t logGetLevel();
extern void logPrint(log_level_t log_level, const char *file_name, int line, const char* format, ...);

// apparently doing so is necessary to correctly connect strings
#define STR2( x) #x
#define STR(x) STR2(x)
#define CON2( a, b) (a##b)
#define CON( a, b) CON( a, b)

// modifier codes
#define DEFAULT_CODE    STR(0)
#define BOLD_CODE       STR(1)
#define LIGHT_CODE      STR(2)
#define ITALIC_CODE     STR(3)
#define UNDERLINED_CODE STR(4)
#define BLINKING_CODE   STR(5)
#define REVERSE_CODE    STR(7)
#define HIDDEN_CODE     STR(8)
#define STRIKED_CODE    STR(9)

// color codes
#define BLACK_CODE  0
#define RED_CODE    1
#define GREEN_CODE  2
#define YELLOW_CODE 3
#define BLUE_CODE   4
#define PURPLE_CODE 5
#define CYAN_CODE   6
#define GRAY_CODE   7

#define FG(code)    STR(3) STR(code)
#define BG(code)    STR(4) STR(code)

#ifndef LOG_DISABLE_COLOR

    #define SET_E(color)                    "\x1b[" color "m"
    #define SET2E(color1,color2)            "\x1b[" color1 ";" color2 "m"
    #define SET3E(color1,color2,color3)     "\x1b[" color1 ";" color2 ";" color3 "m"
    #define RESET                           SET_E(DEFAULT_CODE)

#else

    #define SET_E(c)            ""
    #define SET2E(c1, c2)       ""
    #define SET3E(c1, c2, c3)   ""
    #define RESET               ""

#endif//LOG_DISABLE_COLOR

#define logDebug(...)      logPrint(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define logInfo(...)       logPrint(LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define logWarning(...)    logPrint(LOG_LEVEL_WARNING, __FILE__, __LINE__, __VA_ARGS__)
#define logError(...)      logPrint(LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)

#endif//C_LOG_H