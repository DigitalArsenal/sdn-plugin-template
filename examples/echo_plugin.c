/**
 * Echo Plugin — minimal working SDN WASI plugin example.
 *
 * This plugin echoes back request data with a timestamp prefix,
 * demonstrating all required exports and host import usage.
 *
 * To build this example instead of the skeleton, edit CMakeLists.txt:
 *   set(PLUGIN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/examples/echo_plugin.c)
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "host_imports.h"

static int g_ready = 0;
static char g_name[64] = "echo";

/* ---- Helpers ---- */

static void write_u32_le(uint8_t* p, uint32_t v) {
    p[0] = (uint8_t)(v & 0xFF);
    p[1] = (uint8_t)((v >> 8) & 0xFF);
    p[2] = (uint8_t)((v >> 16) & 0xFF);
    p[3] = (uint8_t)((v >> 24) & 0xFF);
}

static void write_u64_le(uint8_t* p, uint64_t v) {
    for (int i = 0; i < 8; i++) {
        p[i] = (uint8_t)((v >> (i * 8)) & 0xFF);
    }
}

/* ---- Exports ---- */

__attribute__((export_name("plugin_init")))
int32_t plugin_init(const uint8_t* config_ptr, size_t config_len) {
    /* Config format: name as a NUL-terminated string */
    if (config_ptr && config_len > 0 && config_len < sizeof(g_name)) {
        memcpy(g_name, config_ptr, config_len);
        g_name[config_len] = '\0';
    }

    sdn_log(1, (const uint8_t*)"echo plugin ready", 17);
    g_ready = 1;
    return 0;
}

__attribute__((export_name("plugin_get_public_key")))
int32_t plugin_get_public_key(uint8_t* output, size_t output_size) {
    if (!g_ready || !output) return -1;

    /* Generate a random 32-byte "identity" */
    if (output_size < 32) return -1;
    if (sdn_random_bytes(output, 32) != 0) return -1;
    return 32;
}

__attribute__((export_name("plugin_get_metadata")))
int32_t plugin_get_metadata(uint8_t* output, size_t output_size) {
    if (!g_ready || !output || output_size < 4) return -1;

    /* Metadata: name length (4 LE) + name */
    size_t name_len = strlen(g_name);
    size_t total = 4 + name_len;
    if (output_size < total) return -1;

    write_u32_le(output, (uint32_t)name_len);
    memcpy(output + 4, g_name, name_len);
    return (int32_t)total;
}

__attribute__((export_name("plugin_handle_request")))
int32_t plugin_handle_request(
    const uint8_t* req, size_t req_len,
    const char* host_header,
    uint8_t* out, size_t out_size,
    size_t* out_len
) {
    if (!g_ready || !out || !out_len) {
        if (out_len) *out_len = 0;
        return 1;
    }

    /* Response: timestamp(8 LE) + echo of request */
    size_t resp_len = 8 + req_len;
    if (out_size < resp_len) {
        *out_len = 0;
        return 7;
    }

    int64_t now = sdn_clock_now_ms();
    write_u64_le(out, (uint64_t)now);

    if (req && req_len > 0) {
        memcpy(out + 8, req, req_len);
    }

    *out_len = resp_len;

    /* Log the echo */
    char log_buf[128];
    int n = snprintf(log_buf, sizeof(log_buf),
        "echo: %zu bytes from %s at %lld",
        req_len, host_header ? host_header : "(none)", (long long)now);
    if (n > 0) {
        sdn_log(0, (const uint8_t*)log_buf, (uint32_t)n);
    }

    return 0;
}
