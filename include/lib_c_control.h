#ifndef CONTROL_LIBRARY_H
#define CONTROL_LIBRARY_H

#include <stdint.h>

#if defined(_WIN32) || defined(__CYGWIN__)
#ifdef CONTROL_EXPORTS
#define LIB_API __declspec(dllexport)
#else
#define LIB_API __declspec(dllimport)
#endif
#else
#define LIB_API __attribute__((visibility("default")))
#endif

extern "C" {
LIB_API void init();
LIB_API void cleanup();
LIB_API int send_angle_control(int module_id, int angle);
LIB_API char *get_configuration(int *size_out);

LIB_API bool remote_call_c(uint8_t function_tag, uint8_t module, const uint8_t *params,
                           uint16_t params_len, uint8_t *out_buffer, uint16_t out_buffer_capacity,
                           uint16_t *out_bytes_written);

LIB_API bool control_sentry_init(const char *dsn, const char *environment, const char *release);
LIB_API void control_sentry_set_app_info(const char *app_name, const char *app_version,
                                         const char *build_number);
LIB_API void control_sentry_shutdown(void);
}

#endif // CONTROL_LIBRARY_H
