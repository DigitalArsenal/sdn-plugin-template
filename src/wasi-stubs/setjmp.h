// Stub <setjmp.h> for WASI.
// Some crypto libraries include this for SIMD feature detection
// which is irrelevant on WASM. The stubs allow compilation without
// actually supporting setjmp/longjmp.
#pragma once

typedef long jmp_buf[8];
typedef long sigjmp_buf[8];

static inline int setjmp(jmp_buf __attribute__((unused)) env) { return 0; }
static inline void longjmp(jmp_buf __attribute__((unused)) env, int __attribute__((unused)) val) {}
static inline int sigsetjmp(sigjmp_buf __attribute__((unused)) env, int __attribute__((unused)) savesigs) { return 0; }
static inline void siglongjmp(sigjmp_buf __attribute__((unused)) env, int __attribute__((unused)) val) {}
