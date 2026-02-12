#ifndef SDN_HOST_IMPORTS_H
#define SDN_HOST_IMPORTS_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Host-provided functions (implemented by Go/Wazero runtime).
// These are WASI host imports in the "sdn" module namespace.
// Only declare the ones your plugin actually calls.

// Returns current wall-clock time in milliseconds since Unix epoch.
__attribute__((import_module("sdn"), import_name("clock_now_ms")))
int64_t sdn_clock_now_ms(void);

// Fills ptr with len cryptographically secure random bytes.
// Returns 0 on success, -1 on error.
__attribute__((import_module("sdn"), import_name("random_bytes")))
int32_t sdn_random_bytes(uint8_t* ptr, uint32_t len);

// Sends a log message to the host runtime.
// Levels: 0=debug, 1=info, 2=warn, 3+=error
__attribute__((import_module("sdn"), import_name("log")))
void sdn_log(int32_t level, const uint8_t* ptr, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif // SDN_HOST_IMPORTS_H
