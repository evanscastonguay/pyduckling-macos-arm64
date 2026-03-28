# PyDuckling — macOS ARM64 (Apple Silicon) Build

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

**The first native macOS ARM64 build of [pyduckling-native-phihos](https://github.com/phihos/pyduckling)** — Python bindings for Facebook's [Duckling](https://github.com/facebook/duckling) natural language date/time parser.

## Why This Exists

`pyduckling-native-phihos` only ships Linux x86_64 wheels. It uses GHC 8.8.4 which has **zero ARM64 support**. This fork upgrades to GHC 9.2.8 (full ARM64 native code generator) and produces working macOS ARM64 wheels.

This enables running Duckling natively on Apple Silicon Macs — no Docker, no HTTP servers, no Rosetta emulation.

## What Changed from Upstream

| Change | Why |
|--------|-----|
| `stack.yaml`: LTS 16.31 → LTS 20.26 | GHC 8.8.4 → 9.2.8 (ARM64 support) |
| `duckling-ffi.cabal`: Remove `aeson < 2` | LTS 20.x ships aeson 2.x |
| `duckling-ffi.cabal`: Remove `-optl-static`, `-static`, `cc-options`, `ld-options` | Linux-only flags, macOS doesn't support fully-static linking |
| `duckling-ffi.cabal`: `HSrts` → `HSrts-1.0.2` | GHC 9.2 uses versioned RTS library names |
| `stack.yaml`: Add `extra-lib-dirs` for Homebrew | PCRE/GMP at `/opt/homebrew/lib` on ARM64 macOS |
| `stack.yaml`: Add `snap-core`, `snap-server`, `readable`, `some` to extra-deps | Required by Duckling, not in LTS 20.26 |
| `stack.yaml`: Update Duckling to latest upstream commit | Compatibility with GHC 9.2.x |
| `build.rs`: Add `/opt/homebrew/lib`, `libffi`, `libiconv` | macOS system libraries needed at link time |

## Quick Start

### Install Pre-built Wheel

```bash
pip install pyduckling_native_phihos-0.4.0.dev0-cp313-cp313-macosx_11_0_arm64.whl
```

### Usage

```python
from duckling import (
    load_time_zones, parse_ref_time, parse_lang,
    default_locale_lang, parse_locale, parse_dimensions,
    parse, Context
)
import time

# Load timezone database
tz = load_time_zones("/usr/share/zoneinfo")

# Set up parsing context
dims = parse_dimensions(["time", "duration", "time-grain"])
lang = parse_lang("EN")
locale = parse_locale("EN_US", default_locale_lang(lang))
ref_time = parse_ref_time(tz, "America/New_York", int(time.time()))
context = Context(ref_time, locale)

# Parse natural language dates
result = parse("tomorrow", context, dims, False)
# [{'body': 'tomorrow', 'dim': 'time', 'value': {'grain': 'day', 'type': 'value',
#   'value': '2026-03-29T00:00:00.000-04:00', ...}}]

result = parse("next week", context, dims, False)
# [{'body': 'next week', 'dim': 'time', 'value': {'grain': 'week', 'type': 'value',
#   'value': '2026-03-30T00:00:00.000-04:00', ...}}]
```

## Building from Source

### Prerequisites

```bash
# Haskell toolchain (GHC 9.2.8 + Stack)
curl --proto '=https' --tlsv1.2 -sSf https://get-ghcup.haskell.org | \
  BOOTSTRAP_HASKELL_NONINTERACTIVE=1 \
  BOOTSTRAP_HASKELL_GHC_VERSION=9.2.8 \
  BOOTSTRAP_HASKELL_INSTALL_STACK=1 \
  BOOTSTRAP_HASKELL_INSTALL_HLS=0 sh

# Rust
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
rustup update stable

# System libraries
brew install pcre gmp

# Python build tool
pip install maturin
```

### Build Steps

```bash
# 1. Clone this repo
git clone https://github.com/evanscastonguay/pyduckling-macos-arm64.git
cd pyduckling-macos-arm64

# 2. Build the Haskell FFI library (takes ~10-15 min first time)
cd duckling-ffi
export PATH="$HOME/.ghcup/bin:$PATH"
stack build
cd ..

# 3. Create the fat static library (bundles all Haskell deps + GHC RTS)
mkdir -p ext_lib
STACK_SNAP=$(find ~/.stack/snapshots -type d -name "aarch64-osx-ghc-9.2.8" | head -1)
stack exec -- ghc \
  -staticlib -no-hs-main -fPIC \
  -o ext_lib/libducklingffi.a \
  -package-db "$STACK_SNAP/pkgdb" \
  -package duckling -package aeson -package text -package bytestring \
  -package string-conversions -package extra -package unordered-containers \
  -package directory -package time -package timezone-olson \
  -package timezone-series -package filepath \
  duckling-ffi/DucklingFFI/FFI.hs

# 4. Build the Python wheel
maturin build --release --interpreter python3.13
# Wheel output: target/wheels/pyduckling_native_phihos-*.whl
```

## Supported Platforms

| Platform | Architecture | Status |
|----------|-------------|--------|
| macOS 11+ | ARM64 (Apple Silicon) | **NEW** |
| Linux | x86_64 (manylinux) | Upstream |
| Linux | x86_64 (musllinux) | Upstream |

## Credits

- [Facebook Duckling](https://github.com/facebook/duckling) — The Haskell NLP engine (BSD license)
- [phihos/pyduckling](https://github.com/phihos/pyduckling) — Upstream Python bindings fork (MIT license)
- [treble-ai/pyduckling](https://github.com/treble-ai/pyduckling) — Original Python bindings (archived, MIT license)

## License

MIT — same as upstream.
