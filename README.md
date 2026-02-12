# SDN Plugin Template

A starter template for building **WASI plugins** for the [Space Data Network](https://github.com/DigitalArsenal/space-data-network) server. Plugins are compiled to WebAssembly (WASM/WASI) and loaded at runtime by the Go server via [Wazero](https://wazero.io/).

**[Read the full documentation &rarr;](https://digitalarsenal.github.io/sdn-plugin-template/)**

## Quick Start

```bash
# Clone this template
git clone https://github.com/DigitalArsenal/sdn-plugin-template.git my-plugin
cd my-plugin

# Install wasi-sdk (see docs for platform-specific links)
# Then build:
mkdir -p build-wasi && cd build-wasi
cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/wasi-sdk.cmake -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

Output: `my-sdn-plugin.wasm`

## What's Included

| Path | Description |
|---|---|
| `src/plugin.c` | Skeleton plugin with all required exports |
| `src/host_imports.h` | Host function declarations (`sdn.clock_now_ms`, `sdn.random_bytes`, `sdn.log`) |
| `src/wasi-stubs/` | Stub headers for POSIX features missing from WASI |
| `examples/echo_plugin.c` | Minimal working plugin that echoes requests with a timestamp |
| `cmake/wasi-sdk.cmake` | CMake toolchain file for wasi-sdk |
| `CMakeLists.txt` | Build configuration (edit this for your plugin) |
| `docs/` | Documentation site (deployed to GitHub Pages) |

## Plugin ABI

Every SDN plugin must export:

```c
int32_t plugin_init(const uint8_t* config, size_t len);
int32_t plugin_get_public_key(uint8_t* out, size_t out_size);
int32_t plugin_get_metadata(uint8_t* out, size_t out_size);
int32_t plugin_handle_request(const uint8_t* req, size_t req_len,
    const char* host, uint8_t* out, size_t out_size, size_t* out_len);
```

Plus standard `malloc` and `free` (provided by libc).

## License

MIT
