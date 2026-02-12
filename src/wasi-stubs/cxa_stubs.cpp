// Stub C++ exception ABI for WASI.
// wasi-sdk's libc++abi does not include exception support. If your C++
// dependencies contain throw statements (e.g. Crypto++ template
// instantiations), the linker needs these symbols. In WASI plugins all
// errors are handled via return codes, so if an exception is ever thrown
// it means an unrecoverable internal error — we abort and let the host
// runtime (Wazero) catch the WASM trap.
//
// Only needed for C++ plugins. Pure C plugins can omit this file.
#include <stdlib.h>

extern "C" {

void* __cxa_allocate_exception(unsigned long) {
    abort();
    return nullptr;
}

void __cxa_throw(void*, void*, void (*)(void*)) {
    abort();
}

void* __cxa_begin_catch(void*) {
    abort();
    return nullptr;
}

void __cxa_end_catch() {
    abort();
}

void __cxa_rethrow() {
    abort();
}

} // extern "C"
