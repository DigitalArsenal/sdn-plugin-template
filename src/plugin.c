/**
 * SDN Plugin Skeleton
 *
 * Replace this with your plugin implementation. This file demonstrates the
 * required exported functions and how to use host imports.
 *
 * Build: mkdir build-wasi && cd build-wasi
 *        cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/wasi-sdk.cmake -DCMAKE_BUILD_TYPE=Release
 *        make
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "host_imports.h"

/* ---- Plugin state (initialized by plugin_init) ---- */

static int g_initialized = 0;
static uint8_t g_config_hash[32]; /* placeholder — store whatever state you need */

/* ---- Helper: log a string via the host ---- */

static void log_info(const char* msg) {
    sdn_log(1, (const uint8_t*)msg, (uint32_t)strlen(msg));
}

/* ---- Required exports ---- */

/**
 * plugin_init — called once at startup with a binary config blob.
 *
 * The config format is plugin-specific. Parse whatever your Go wrapper packs.
 * Return 0 on success, non-zero on error.
 */
__attribute__((export_name("plugin_init")))
int32_t plugin_init(const uint8_t* config_ptr, size_t config_len) {
    if (!config_ptr || config_len == 0) {
        return 1; /* error: empty config */
    }

    /* TODO: Parse your config blob here */

    g_initialized = 1;
    log_info("plugin initialized");
    return 0;
}

/**
 * plugin_get_public_key — write the plugin's public identity into the output buffer.
 *
 * Return bytes written on success, -1 on error.
 */
__attribute__((export_name("plugin_get_public_key")))
int32_t plugin_get_public_key(uint8_t* output, size_t output_size) {
    if (!g_initialized || !output) {
        return -1;
    }

    /* TODO: Write your public key / identity bytes here */
    const char* placeholder = "placeholder-public-key";
    size_t len = strlen(placeholder);
    if (output_size < len) {
        return -1;
    }

    memcpy(output, placeholder, len);
    return (int32_t)len;
}

/**
 * plugin_get_metadata — write plugin metadata into the output buffer.
 *
 * Format is plugin-specific. The reference key broker uses:
 *   domainCount(4 LE) + [domainLen(2 LE) + domain(N)]...
 *
 * Return bytes written on success, -1 on error.
 */
__attribute__((export_name("plugin_get_metadata")))
int32_t plugin_get_metadata(uint8_t* output, size_t output_size) {
    if (!g_initialized || !output || output_size < 4) {
        return -1;
    }

    /* TODO: Write your metadata here */

    /* Example: zero domains */
    memset(output, 0, 4);
    return 4;
}

/**
 * plugin_handle_request — process a request packet.
 *
 * @param request_packet     Pointer to the incoming request bytes
 * @param request_packet_len Length of the request
 * @param host_header        NUL-terminated host/origin string
 * @param output_buffer      Pre-allocated buffer for the response
 * @param output_buffer_size Size of the output buffer
 * @param output_len         Pointer to write the actual response length (4 bytes LE)
 * @return Status code (0 = success)
 */
__attribute__((export_name("plugin_handle_request")))
int32_t plugin_handle_request(
    const uint8_t* request_packet, size_t request_packet_len,
    const char* host_header,
    uint8_t* output_buffer, size_t output_buffer_size,
    size_t* output_len
) {
    if (!g_initialized) {
        if (output_len) *output_len = 0;
        return 6; /* not initialized */
    }

    if (!request_packet || !output_buffer || !output_len) {
        return 1; /* malformed */
    }

    /* TODO: Process the request and build a response */

    /* Example: echo back a simple response */
    const char* resp = "OK";
    size_t resp_len = strlen(resp);

    if (output_buffer_size < resp_len) {
        *output_len = 0;
        return 7; /* internal error — buffer too small */
    }

    memcpy(output_buffer, resp, resp_len);
    *output_len = resp_len;

    return 0; /* success */
}
