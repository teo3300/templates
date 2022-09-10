#include "log.h"
#include <stdio.h>
#include <stdarg.h>

log_level_t treshold_log_level = LOG_LEVEL_DEBUG;
const char *log_level_flag[] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
};

const char *log_level_color[] = {
    SET_E(FG(BLUE_CODE)),
    SET_E(FG(GREEN_CODE)),
    SET2E(BLINKING_CODE, FG(YELLOW_CODE)),
    SET2E(BOLD_CODE, FG(RED_CODE))
};

void logSetLevel(log_level_t log_level) {
    treshold_log_level = log_level;
}

log_level_t logGetLevel() {
    return treshold_log_level;
}

void logPrint(log_level_t log_level, const char *file_name, int line, const char* format, ...) {

    if(log_level < treshold_log_level) return;
    // print header
    fprintf(stderr, "%s%-5s "RESET SET_E(LIGHT_CODE)"%s:%d "RESET, log_level_color[log_level], log_level_flag[log_level], file_name, line);
    va_list arg;
    va_start(arg, format);
    vfprintf(stderr, format, arg);
    va_end(arg);
    fprintf(stderr, "\n");
}