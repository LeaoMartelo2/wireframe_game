/* Copyright 2024 LeaoMartelo2 (https://github.com/LeaoMartelo2)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#ifndef LOGNEST_H_
#define LOGNEST_H_

#define LOGNEST_VERSION "2.0.0"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/* CHANGE WHERE THE LOGS GETS SENT
 * DEFAULT VALUE: "latest.log"
 * (PATH RELATIVE TO THE EXECUTABLE)*/
#define LOGNEST_FILE "latest.log"

#define TIMESTAMP_BUFFER_MAX_SIZE 2048

void _lognest_trace_raw(const char *file, const char *format, ...);
void _lognest_warn_raw(const char *file, const char *format, ...);
void _lognest_error_raw(const char *file, const char *format, ...);
void _lognest_debug_raw(const char *file, const char *format, ...);

#define lognest_trace(...) _lognest_trace_raw(LOGNEST_FILE, __VA_ARGS__)
#define lognest_warn(...) _lognest_warn_raw(LOGNEST_FILE, __VA_ARGS__)
#define lognest_error(...) _lognest_error_raw(LOGNEST_FILE, __VA_ARGS__)
#define lognest_debug(...) _lognest_debug_raw(LOGNEST_FILE, __VA_ARGS__)

void lognest_to_file(const char *file, const char *level, const char *format, va_list args);

void get_timestamp(char *buffer, size_t len);

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef LOGNEST_IMPLEMENTATION

inline void get_timestamp(char *buffer, size_t len) {

    if (buffer == NULL || len > TIMESTAMP_BUFFER_MAX_SIZE) {
        fprintf(stderr, "LogNest: timestamp buffer, was too small\n");
        return;
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    if (t == NULL) {
        fprintf(stderr, "LogNest: failed to get localtime\n");
        return;
    }

    strftime(buffer, len, "[%y/%m/%d][%H:%M:%S]", t);
}

void lognest_to_file(const char *file, const char *level, const char *format, va_list args) {

    if (file == NULL) {
        fprintf(stderr, "LogNest: filename was recieved as NULL or invalid: value: %s\n", file);
        return;
    }

    FILE *log_file = fopen(file, "a");
    if (log_file == NULL) {
        fprintf(stderr, "LogNest: error trying to open file %s", file);
        return;
    }

    char timestamp[TIMESTAMP_BUFFER_MAX_SIZE] = {0};
    get_timestamp(timestamp, TIMESTAMP_BUFFER_MAX_SIZE);

    fprintf(log_file, "%s%s: ", timestamp, level);

    vfprintf(log_file, format, args);

    fprintf(log_file, "\n");

    fclose(log_file);
}

void _lognest_trace_raw(const char *file, const char *format, ...) {

#ifdef LOGNEST_DISABLE_TRACE
    return;
#endif // LOGNEST_DISABLE_TRACE

    va_list args;
    va_start(args, format);
    lognest_to_file(file, "[LOG]  ", format, args);

    va_end(args);
}

void _lognest_warn_raw(const char *file, const char *format, ...) {

#ifdef LOGNEST_DISABLE_WARN
    return;
#endif // LOGNEST_DISABLE_WARN

    va_list args;
    va_start(args, format);
    lognest_to_file(file, "[WARN] ", format, args);

    va_end(args);
}

void _lognest_error_raw(const char *file, const char *format, ...) {

#ifdef LOGNEST_DISABLE_ERROR
    return;
#endif // LOGNEST_DISABLE_ERROR

    va_list args;
    va_start(args, format);
    lognest_to_file(file, "[ERROR]", format, args);

    va_end(args);
}

void _lognest_debug_raw(const char *file, const char *format, ...) {

#ifdef LOGNEST_DISABLE_DEBUG
    return;
#endif // LOGNEST_DISABLE_DEBUG

    va_list args;
    va_start(args, format);
    lognest_to_file(file, "[DEBUG]", format, args);

    va_end(args);
}

#endif // LOGNEST_IMPLEMENTATION

#endif // !LOGNEST_H_
