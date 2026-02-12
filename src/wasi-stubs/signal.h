// Stub <signal.h> extensions for WASI.
// WASI's emulated signal.h provides signal()/raise() but gates sigset_t,
// sigprocmask, SIG_SETMASK etc. behind __wasilibc_unmodified_upstream.
// Some libraries use these for CPU feature detection which is irrelevant
// on WASM. Provide no-op stubs.
#pragma once

// Pull in the real WASI signal.h first (provides signal, raise, SIG_* constants)
#include_next <signal.h>

#ifndef _WASI_STUBS_SIGNAL_EXTRAS
#define _WASI_STUBS_SIGNAL_EXTRAS

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __DEFINED_sigset_t
typedef unsigned long sigset_t;
#define __DEFINED_sigset_t
#endif

#ifndef SIG_SETMASK
#define SIG_BLOCK   0
#define SIG_UNBLOCK 1
#define SIG_SETMASK 2
#endif

static inline int sigprocmask(int __attribute__((unused)) how,
                              const sigset_t* __attribute__((unused)) set,
                              sigset_t* __attribute__((unused)) oldset) {
    return 0;
}

static inline int sigemptyset(sigset_t* set) {
    if (set) *set = 0;
    return 0;
}

static inline int sigfillset(sigset_t* set) {
    if (set) *set = ~(sigset_t)0;
    return 0;
}

static inline int sigaddset(sigset_t* set, int signo) {
    if (set) *set |= (1UL << (unsigned)(signo - 1));
    return 0;
}

static inline int sigdelset(sigset_t* set, int signo) {
    if (set) *set &= ~(1UL << (unsigned)(signo - 1));
    return 0;
}

static inline int sigismember(const sigset_t* set, int signo) {
    if (!set) return 0;
    return (*set & (1UL << (unsigned)(signo - 1))) ? 1 : 0;
}

#ifdef __cplusplus
}
#endif

#endif // _WASI_STUBS_SIGNAL_EXTRAS
