# vibuxOS

Created: 2026-09-25T05:42:08Z
Updated: 2026-09-25T06:08:30Z
URL: https://github.com/mahoraga94/vibuxOS


## No README


---

# xinux

Created: 2025-04-05T13:14:51Z
Updated: 2026-09-03T16:37:28Z
URL: https://github.com/mahoraga94/xinux


## README


# 🌟 Xinux Shell

Welcome to **Xinux Shell**, the command-line interface (CLI) that’s so good, it might just make you forget about GUIs altogether. Whether you're a developer, a power user, or someone who just likes to type random commands to feel productive, Xinux Shell is here to revolutionize your terminal experience. Seriously, it’s like upgrading from a tricycle to a rocket ship. 🚀

---

## 🚀 Key Features

### 1️⃣ **Customizable Prompt Styles**
- Why settle for boring prompts when you can have **15+ stunning styles**? Pick one that screams *you*—or at least whispers it politely.
    - **Examples:** 
        - Single-line (for minimalists)
        - Two-line (for overachievers)
        - Boxed (because why not?)
        - Minimal (less is more)
        - Classic (nostalgia, anyone?)
        - Bold Frame (for the dramatic)
        - Cyberpunk (neon dreams)
        - Retro (hello, 80s!)
- Oh, and did we mention they’re **colorized** and adapt to your current directory? Your terminal will look so good, you might start showing it off at parties.

---

### 2️⃣ **Intelligent Auto-Completion**
- Typing is hard. Let Xinux Shell do the heavy lifting with **context-aware auto-completion** for:
    - Shell history (because who remembers what they typed 5 minutes ago?)
    - Built-in commands (like `cd`, `ls`, and other classics)
    - Executables in your `$PATH` (no more guessing games)
    - User-defined aliases (your shortcuts, your rules)
- And if you mess up? Don’t worry—**hints for incomplete commands** will gently nudge you in the right direction. It’s like having a helpful friend who doesn’t judge.

---

### 3️⃣ **Advanced History Management**
- Forget the days of scrolling through endless, messy command history. Xinux Shell keeps things tidy:
    - **Duplicate-free history tracking** ensures no command gets repeated unnecessarily. (We’re looking at you, `ls`.)
    - Want a clean slate? Just type `clear history` and poof—gone.
- Your history is safely stored in:
  ```plaintext
  ~/.config/.Xinux/history.txt
  ```
- Never lose track of your most-used commands again. Unless you want to, of course.

---

### 4️⃣ **Powerful Aliases**
- Tired of typing the same long commands over and over? Create **custom shortcuts** with the `alias` command.
    - **Usage:**
      ```bash
      alias name=command
      ```
    - **Example:**
      ```bash
      alias ll="ls -la"
      ```
- Aliases are saved for eternity (or until you delete them) in the configuration file. Your fingers will thank you.

---
### 5️⃣ **Built-In Commands**

Xinux Shell comes preloaded with **essential built-in commands** that make your terminal experience magical:

```plaintext
Available commands:
├── cd: Teleport to another folder dimension!
├── ls: Summon a list of files and directories in your current realm.
├── clear: Wipe the terminal clean like a wizard's spell.
├── exit: Escape the Xinux universe and return to reality.
├── help: Summon this magical scroll of wisdom.
├── echo: Make the terminal repeat your words like a loyal parrot.
├── xinsay: Let Xin speak with the charm of cowsay!
├── alias: Forge shortcuts to commands like a true hacker-smith.
├── clear history: Erase your command history like a secret agent.
├── xinux config prompt: Redesign your prompt with style and flair.
├── xinux config where: Reveal the sacred location of the config file.
└── xinux reset: Reset Xinux to its pristine state, like a phoenix reborn.
```

---

### 6️⃣ **First-Time Setup**
- First impressions matter, and Xinux Shell knows it. On your **first launch**, you’ll get a guided setup to:
    - Choose your favorite **prompt style** (good luck picking just one).
    - Configure initial settings like a pro.
- All your preferences are saved to:
  ```plaintext
  ~/.config/.Xinux/config.toml
  ```
- It’s like a warm welcome, but for your terminal.

---

### 7️⃣ **Dynamic Configuration**
- Want to tweak your setup? No problem. Just edit the configuration file:
  ```plaintext
  ~/.config/.Xinux/config.toml
  ```
- Customize everything from **prompt styles** to **aliases**. And the best part? Changes are applied instantly—no need to restart. (Take that, other shells!)

---

---

So, what are you waiting for? Start your journey with **Xinux Shell** today and turn your terminal into a productivity powerhouse. Or at least make it look really cool. 🚀

> Pro tip: When in doubt, type `help`. It’s like having a cheat sheet, but without the guilt.

## Shhhh... Xinsay has something to tell you. Type `xinsay` to find out.
### Also, don’t forget to check out [CONFIG](CONFIG.md) for all the juicy customization options.



---

# blueraptor

Created: 2026-09-03T15:45:56Z
Updated: 2026-09-03T16:29:37Z
URL: https://github.com/mahoraga94/blueraptor


## README

# BlueRaptor

BlueRaptor is a small Linux userspace and boot environment built around a simple idea:

> Keep the system understandable, keep the runtime small, and let the pieces do one job well.

The project currently brings together a native BlueRaptor runtime, a native PID 1 path, a lightweight filesystem/runtime layer, binary discovery and execution support, system telemetry, networking and Wi-Fi support, Xinux, and Genome, the package manager.

It is designed around a Tiny Core-style base image and can turn that base into a customized BlueRaptor ISO with the included `patch.sh` builder.

The project is intentionally practical. It does not try to hide what is happening behind a giant framework. Most important operations are visible in the source, the build process is staged, and failures leave the build workspace behind so they can be inspected.

---

## What BlueRaptor is

BlueRaptor has a few major pieces.

### `br-init`

`br-init` is the native boot path.

The final image keeps the original Tiny Core init as `/init.tinycore`, installs `br-init` as `/usr/bin/br-init`, and places a small wrapper at `/init`. That wrapper displays the pre-rendered boot logo, starts the network manager, and then enters `br-init`.

`br-init` is the process that ultimately enters the BlueRaptor native PID 1 manager.

The normal userspace `blueraptor` binary deliberately refuses to run as PID 1. The source checks its process ID and tells you to use `/init`/`br-init` instead.

### BRFS

BRFS is the filesystem/runtime layer.

At boot, it prepares the runtime environment and the core Linux virtual filesystems. The boot report checks things such as:

- `/proc`
- `/sys`
- `/dev`
- the runtime directory
- BusyBox availability
- recovery state

BRFS also exposes a health check used by the regular `blueraptor status` command and the self-test command.

### LBIN

LBIN is the binary manager.

It discovers executable programs from the real `PATH` and from the project's standard fallback directories. It registers familiar BlueRaptor names first, then scans available executable files.

This means the runtime is not forced to assume that every command lives in one hard-coded location.

LBIN can:

- find a binary
- report its path
- verify that it is a real executable file
- list discovered binaries
- attach programs to the detected terminal
- run programs
- spawn programs detached from the current command

For child processes, LBIN sets the BlueRaptor runtime environment and attaches stdin/stdout/stderr to the detected terminal.

### Adaptive

Adaptive is the system telemetry layer.

It keeps a shared snapshot containing information such as:

- uptime
- CPU usage
- RAM usage
- load averages
- thermal readings
- network interfaces
- receive/transmit counters
- wireless signal information

The monitor takes an initial sample and then updates the shared snapshot in the background.

The regular runtime exposes this through:

```text
blueraptor adaptive status
```

### Networking

BlueRaptor contains a native network layer and Wi-Fi helpers.

The normal `blueraptor` command currently exposes:

```text
blueraptor wifi interfaces
blueraptor wifi scan [interface]
blueraptor wifi status [interface]
blueraptor wifi connect <interface> <ssid> <password>
blueraptor wifi disconnect [interface]
```

The ISO builder also packages a small automatic network manager and DHCP hook.

The builder detects host networking hardware, collects the corresponding kernel modules and dependencies, includes firmware where available, and writes an autoload list. `8139cp` is treated as mandatory because it is used for the project's QEMU/RTL8139 networking path.

### Xinux

Xinux is the shell/command environment expected by the BlueRaptor runtime.

The image places it at:

```text
/usr/bin/xinux
```

and also creates:

```text
/usr/bin/xinux-shell
```

as a link to Xinux.

### Genome

Genome is BlueRaptor's package manager.

It is designed around repository metadata, ranked mirrors, a local cache, package generations, and an isolated package store.

The current default state locations are:

```text
/var/lib/genome
/var/cache/genome
/etc/genome.conf
```

The package manager has repository operations, package search/info/install/remove/upgrade support, generations, rollback operations, cache management, mirror ranking, system status, and AUR support.

---

# Building BlueRaptor

The main build entry point is:

```bash
./patch.sh <input.iso> [options]
```

A normal build can be as simple as:

```bash
./patch.sh TinyCore.iso
```

The builder uses the supplied ISO as the base, extracts it, uses the host kernel and matching module tree by default, builds the Rust components, installs the runtime programs, adds hardware-specific modules, rebuilds `core.gz`, verifies the resulting filesystem, and creates the final ISO.

The build is deliberately staged. There are twenty named pipeline stages covering:

1. Rust build
2. base ISO extraction
3. base provider inspection
4. kernel installation
5. kernel module collection
6. BlueRaptor userspace installation
7. runtime utility packaging
8. network module list
9. DHCP hook
10. network manager
11. network hooks
12. network interface configuration
13. CA trust store
14. boot logo
15. PID 1
16. runtime identity
17. boot configuration
18. `core.gz`
19. final rootfs verification
20. final ISO creation

Build logs are kept under:

```text
.br-work/logs/
```

A failed build leaves `.br-work` in place so the logs and intermediate filesystem can be inspected.

---

# Builder customization

The builder accepts command-line configuration instead of requiring you to edit the script for every new ISO.

## OS identity

```bash
./patch.sh TinyCore.iso \
    --name "Divine Linux" \
    --id divine \
    --version "2026.09" \
    --pretty-name "Divine Linux 2026.09"
```

These values are used for the generated operating-system identity and build presentation.

The generated `/etc/os-release` contains the chosen name, ID, pretty name, version, architecture, and kernel.

The hostname, `/etc/issue`, and `/etc/motd` are also generated from the chosen identity.

## Output filename

```bash
./patch.sh TinyCore.iso \
    --id divine \
    --output divine.iso
```

Relative output paths are interpreted relative to the project directory.

## Parallel Rust build

```bash
./patch.sh TinyCore.iso --jobs 4
```

The builder passes this value to Cargo.

If `--jobs` is omitted, it uses `CARGO_BUILD_JOBS` when set, otherwise it falls back to the host CPU count reported by `nproc`.

## Compression

```bash
./patch.sh TinyCore.iso --gzip-level 1
```

Lower levels favor speed.

```bash
./patch.sh TinyCore.iso --gzip-level 9
```

Higher levels favor a smaller `core.gz`.

For an older machine, the default low compression setting is intentional: rebuilding the image should not spend unnecessary time compressing an already small filesystem.

## ELF handling

By default the builder strips unneeded symbols from ELF files in the packaged userspace tree.

Disable that with:

```bash
./patch.sh TinyCore.iso --no-strip
```

The builder also copies discovered ELF dependencies by default.

Disable dependency copying with:

```bash
./patch.sh TinyCore.iso --no-elf-deps
```

That option is mainly useful when you are deliberately constructing or debugging a different runtime layout.

## Kernel

By default the running host kernel is used.

A different kernel image can be supplied with:

```bash
./patch.sh TinyCore.iso --kernel /path/to/vmlinuz
```

The custom kernel still needs a matching module tree available to the builder. The builder expects the corresponding module directory under `/usr/lib/modules`.

---

# Networking during image creation

The builder detects host network hardware through `/sys/class/net` and resolves the kernel driver associated with detected interfaces.

It also checks for optional QEMU-friendly network modules such as:

```text
e1000
e1000e
virtio_net
pcnet32
ne2k_pci
```

The RTL8139 `8139cp` module is explicitly required.

Wi-Fi modules are added when a wireless interface is detected.

Sound modules are only collected when sound hardware is present.

Firmware referenced by the selected modules is copied into the image when the host has it available.

---

# Runtime networking

The image contains a generated network manager:

```text
/usr/local/sbin/blueraptor-net
```

and a DHCP hook:

```text
/etc/blueraptor/udhcpc.script
```

The network manager discovers interfaces at boot, brings them up, attempts DHCP when enabled, and can apply a QEMU fallback configuration when no default route exists.

The default fallback values are:

```text
IP:       10.0.2.15
Netmask:  24
Gateway:  10.0.2.2
DNS:      10.0.2.3
```

Disable DHCP:

```bash
./patch.sh TinyCore.iso --no-dhcp
```

Disable the QEMU fallback:

```bash
./patch.sh TinyCore.iso --no-qemu-fallback
```

Or customize it:

```bash
./patch.sh TinyCore.iso \
    --fallback-ip 10.0.2.50 \
    --fallback-netmask 24 \
    --fallback-gateway 10.0.2.2 \
    --fallback-dns 10.0.2.3
```

---

# Boot and branding

The builder can render a PNG logo into ANSI escape sequences during the build.

By default it looks for:

```text
brlogo.png
```

A different image can be supplied:

```bash
./patch.sh TinyCore.iso --logo ~/Pictures/my-logo.png
```

The resulting ANSI rendering is stored in:

```text
/etc/blueraptor/boot-logo.ansi
```

The original PNG is also installed under:

```text
/usr/share/blueraptor/brlogo.png
/usr/share/pixmaps/brlogo.png
```

The `/init` wrapper displays the pre-rendered logo before starting the network manager and `br-init`.

Because the logo is rendered during image creation, `chafa` itself does not need to be part of the final image just to display the boot logo.

---

# Boot entries

The generated ISOLINUX configuration contains:

```text
blueraptor
selftest
hold
legacy
```

The default boot entry is normally:

```text
blueraptor
```

You can change it:

```bash
./patch.sh TinyCore.iso --boot selftest
```

The timeout can be changed:

```bash
./patch.sh TinyCore.iso --timeout 5
```

A visible boot prompt can be enabled with:

```bash
./patch.sh TinyCore.iso --prompt
```

---

# Runtime commands

After booting the image, the normal userspace executable is:

```bash
blueraptor
```

## Help

```bash
blueraptor help
```

## Version

```bash
blueraptor version
```

## General runtime status

```bash
blueraptor status
```

The status command reports BRFS/runtime paths, terminal information, CPU and memory usage, load, filesystem health, interfaces, and registered binaries.

## Adaptive telemetry

```bash
blueraptor adaptive status
```

This shows uptime, CPU, RAM, load averages, interfaces, and available thermal readings.

## Binary discovery

```bash
blueraptor binaries
```

This lists discovered executable programs and verifies them as it goes.

## Self-test

```bash
blueraptor test
```

or:

```bash
blueraptor self-test
```

The current self-test checks BRFS health, binary availability, the runtime directory, and the adaptive monitor.

---

# Wi-Fi commands

List wireless interfaces:

```bash
blueraptor wifi interfaces
```

Scan:

```bash
blueraptor wifi scan
```

Or scan through a particular interface:

```bash
blueraptor wifi scan wlan0
```

Check status:

```bash
blueraptor wifi status
```

Or:

```bash
blueraptor wifi status wlan0
```

Connect:

```bash
blueraptor wifi connect wlan0 "My WiFi" "password"
```

Disconnect:

```bash
blueraptor wifi disconnect wlan0
```

The Wi-Fi command layer reports interface state, MAC address, SSID scan results, signal strength, channel, security state, carrier state, IPv4, and IPv6 where available.

---

# Genome

Genome is the package-management part of the project.

The current command set includes:

```text
sync
rank
search
info
install
remove
reinstall
upgrade
update
list
generations
rollback
back
final
files
mirrors
cache
clean
status
aur
```

Examples:

```bash
genome sync
genome search curl
genome info curl
genome install curl
genome remove curl
genome reinstall curl
genome upgrade
genome list
genome status
genome cache
genome mirrors
```

Genome also supports generation-oriented package management:

```bash
genome generations <package>
genome rollback <package> <generation>
genome back <package>
genome final <package>
```

Package-owned files can be inspected with:

```bash
genome files <package>
```

Cache cleanup supports keeping a chosen number of generations:

```bash
genome clean
genome clean --keep 5
genome clean <package>
```

AUR operations are grouped below:

```bash
genome aur search <query>
genome aur info <package>
genome aur install <package>
```

The installed package database is kept under:

```text
/var/lib/genome
```

The package cache is kept under:

```text
/var/cache/genome
```

---

# Genome command-line flags

The current CLI also supports global output controls.

The `--quiet` option is intended for error-only output.

The normal Genome command handlers use a common visual language with headers, success/failure markers, progress steps, package names, versions, and actionable failure context.

When a failure is returned with no more specific hint, the CLI points toward `RUST_BACKTRACE=1` for additional Rust-side detail.

---

# Repository and mirrors

Genome maintains repository metadata and a local cache.

Mirrors are ranked, with the configuration allowing regional preference. The current configuration and implementation are designed to prefer the configured region; the CLI describes the built-in ranking command as "India first".

Synchronizing repositories looks like:

```bash
genome sync
```

Viewing ranked mirrors:

```bash
genome mirrors
```

or:

```bash
genome rank
```

The mirror subsystem keeps HTTP clients alive for repeated operations and uses cached mirror data when it is still fresh.

---

# Why the project is structured this way

There are a few design choices worth understanding before changing the project.

### PID 1 has one owner

`/init` and `br-init` own the real boot/PID 1 path.

The ordinary `blueraptor` command is userspace only. It intentionally refuses to become PID 1.

That separation makes normal CLI usage much less dangerous.

### Hardware discovery happens at build time

The ISO builder is meant to create an image suitable for the machine it is being built on.

It detects network, Wi-Fi, sound, driver dependencies, and firmware on the host, then packages what it finds.

This avoids blindly including every possible kernel module.

### Runtime commands are lightweight

Adaptive telemetry is shared between commands instead of being rebuilt independently for every status call.

LBIN discovers executables once per manager instance and reuses the resulting map.

### The ISO keeps a small base

The builder starts from the supplied base ISO, modifies the extracted root filesystem, and rebuilds `core.gz`.

The runtime gets the project's native components plus the utilities required for networking, diagnosis, module management, and normal operation.

---

# Troubleshooting

## The builder says a host command is missing

The builder checks a fixed set of host tools before doing the real build.

If something is absent and a package mapping exists, it can install the missing host package automatically using pacman.

The missing command is reported before the build continues.

## A build fails

Do not immediately delete `.br-work`.

The failed workspace contains:

```text
.br-work/
├── logs/
├── iso/
├── rootfs/
└── verify/
```

The individual stage commands redirect their longer output into logs, while the terminal keeps the useful summary.

Start with:

```bash
ls -lah .br-work/logs
```

Then inspect the relevant log.

## The build was interrupted

The workspace is intentionally preserved when the build does not finish successfully.

This is useful for debugging because the extracted ISO and partially assembled rootfs are still available.

## The terminal UI looks broken after Ctrl+C

The builder traps SIGINT/SIGTERM and restores the cursor during cleanup.

If a terminal is ever left in a bad state anyway, the usual shell reset command is:

```bash
reset
```

---

# Project layout

The important top-level pieces are:

```text
src/
├── adaptive.rs
├── brfs/
├── genome/
├── init/
├── lbin.rs
├── net/
├── shell.rs
├── lib.rs
├── main.rs
└── bin/
    └── br-init.rs

srcbins/
    xinux/

patch.sh
brlogo.png
Cargo.toml
Cargo.lock
```

The source tree separates runtime responsibilities instead of placing everything in one binary.

---

# Development

Build the main Rust components:

```bash
cargo build --release --locked \
    --target x86_64-unknown-linux-gnu \
    --bin blueraptor \
    --bin br-init \
    --bin genome
```

The ISO builder performs this build for you and also builds Xinux when it is not already available.

For a faster development cycle, retain the Cargo target directory between builds.

---

# A practical build

A comfortable customized build command looks like this:

```bash
./patch.sh TinyCore.iso \
    --name "Divine Linux" \
    --id divine \
    --version "2026.09" \
    --pretty-name "Divine Linux 2026.09" \
    --output divine.iso \
    --jobs 4 \
    --gzip-level 1 \
    --timeout 2 \
    --prompt
```

For a fast, minimal build on an older machine:

```bash
./patch.sh TinyCore.iso \
    --name "Divine Linux" \
    --id divine \
    --version "2026.09" \
    --output divine.iso \
    --jobs 4 \
    --gzip-level 1
```

---

# Current scope

This README describes the functionality present in the source snapshot used to write it.

It deliberately does not claim features that are not exposed by the current command layer. In particular, the documentation follows the current source's command names and builder options rather than assuming future functionality.

---

# License

No license information is defined in the provided source snapshot.

Add a license file and update this section when the project settles on one.


---

# tungstun

Created: 2026-08-06T09:07:03Z
Updated: 2026-08-24T16:15:54Z
URL: https://github.com/mahoraga94/tungstun


## README

<p align="center">
  <img src="wlogo.png" alt="Tungstun Logo" width="120">
</p>

<h1 align="center">Tungstun</h1>

<p align="center">
  <em>A batteries-included graphics framework written in Rust.</em>
</p>

<p align="center">
  <a href="LICENSE-MIT"><img src="https://img.shields.io/badge/License-MIT-blue.svg" alt="MIT License"></a>
  <a href="LICENSE-APACHE"><img src="https://img.shields.io/badge/License-Apache--2.0-blue.svg" alt="Apache 2.0 License"></a>
</p>

---

Tungstun is a batteries-included graphics framework written in Rust. It combines rendering, physics, audio, UI, and input into a cohesive API while remaining lightweight, transparent, and easy to understand. It is built to help developers learn how rendering works under the hood while providing a reliable toolset for building graphical applications.

The engine features two distinct rendering methods. It includes a software rasteriser built from scratch to run entirely on the CPU, alongside a hardware-accelerated OpenGL pipeline. The system can transition between these two approaches based on hardware capabilities, allowing applications to run smoothly on both older systems and modern desktops.

Tungstun minimizes complex abstractions to keep the architecture straightforward. Instead of deeply nested object hierarchies, it uses flat data structures and direct memory management. This approach is especially helpful for developers who are just getting into graphics programming, as it clearly illustrates how data moves from the computer's memory to the screen.

## Features

- **Dual Rendering Backends:** A built-from-scratch CPU software rasteriser alongside a modern, hardware-accelerated OpenGL pipeline.
- **Physics & Collision:** Continuous Collision Detection (CCD) system supporting spheres, bounding boxes, and complex triangle meshes.
- **Spatial Audio:** 3D positional audio tracking with support for standard audio files and procedural sound generation.
- **UI & Text Rendering:** Highly batched user interface rendering with a procedural font generated entirely in memory at startup.
- **Dual API Integration:** Native Rust state machine integration and a complete, thread-safe C Application Binary Interface (ABI).
- **Cross-Platform:** Smooth execution on both legacy hardware and modern desktops.

## Architectural Overview

The architecture prioritizes efficient execution and memory usage. The codebase utilizes specific mathematical techniques to process multiple colour channels simultaneously during texture filtering. It also carefully manages how often the operating system is queried to keep frame rates stable, and it uses multiple memory buffers to upload animation data smoothly without causing the application to pause.

### Core Subsystems

**1. Rendering Pipeline**
The engine supports a traditional, immediate-mode drawing style for rapid prototyping, as well as a modern batching pipeline. When running on the graphics card, Tungstun compiles OpenGL shaders to handle the visuals. When running on the processor, the software rasteriser takes over, utilizing highly tuned mathematics to draw textured, lighted shapes directly into a memory buffer.

**2. Physics and Collision**
Physics interactions are handled via a Continuous Collision Detection system. Objects are sorted to quickly rule out items that are too far apart to collide, which reduces the computational workload. The system supports spheres, bounding boxes, and complex triangle meshes. To prevent fast-moving objects from passing through walls, the engine calculates predictive paths before updating their positions.

**3. Spatial Audio**
The audio system tracks virtual listeners in 3D space, calculating volume and panning based on distance and direction. It supports loading standard audio files into memory and features a procedural sound generator for immediate auditory feedback during development.

**4. Interface and Text**
Rendering user interfaces can often slow down an application due to how many individual elements need to be drawn. Tungstun resolves this by grouping text and interface elements together. The engine also includes a procedural font generated entirely in memory at startup, meaning debug text can be displayed without needing to load external font files.

## API Reference and Integration

To keep the architectural vision clear, code samples are omitted from this overview. For practical implementation, please refer to the dedicated reference files.

**Native Rust Integration**
For applications built in Rust, developers will interface directly with the TungstunContext state machine. This native API allows for maximum performance and direct access to the engine's features. Please review the [RS_API.md](RS_API.md) file for the complete Rust structural layouts, module imports, and usage guidelines.

**C and C++ Integration**
Tungstun exports a complete C Application Binary Interface. This layer is designed to be thread-safe through localized context pointers, making it possible to link the compiled library to a C or C++ codebase. Please review the [c_abi.md](c_abi.md) and the [tungstun.h](tungstun.h) file for exact function signatures, memory rules, and initialization steps.

## Future Development

The engine is stable for current use cases, with ongoing work focused on further refining the software rasteriser and minimizing the computational footprint of the physics system. Contributions that maintain the existing memory layout and prioritize clear, educational, and measurable improvements are always welcome.

---
<p align="center">
  <img src="logo.png" alt="Tungstun Logo" width="500">
</p>

---

# allsec

Created: 2026-08-01T08:14:17Z
Updated: 2026-08-01T14:41:02Z
URL: https://github.com/mahoraga94/allsec


## README

# `allsec`

> **Uncompromising Performance. Idiomatic Safety. Elegant Generation.**

`allsec` is a next-generation, zero-overhead lexical and syntactic analysis toolchain. Written in highly optimized C, it compiles domain-specific grammar definitions into production-ready, borrow-checker-compliant Rust code. 

Designed for engineers who refuse to choose between generation speed, runtime performance, and type safety, `allsec` bridges the gap between formal language theory and modern systems programming.

---

## 🎯 Core Philosophy

1. **Zero-Cost Abstractions**: The generated Rust code contains no hidden allocations, dynamic dispatch, or runtime overhead. It is as fast as hand-written parser combinators, but fractionally the effort.
2. **Deterministic Performance**: Lexical analysis is powered by a parallelized, minimised DFA builder. Syntactic analysis uses Predictive PEG (Parsing Expression Grammar) semantics with automatic, scoped backtracking.
3. **Ergonomic by Design**: The toolchain understands Rust's borrow checker. It automatically infers lifetimes, scopes mutable references tightly, and generates clean, `clippy`-approved code.

---

## 🚀 Primary Use Cases

`allsec` is engineered for environments where correctness, maintainability, and raw throughput are non-negotiable.

### 1. Compiler & Interpreter Toolchains
Bootstrap language frontends with predictable, linear-time lexical analysis and robust, backtracking-capable recursive descent parsers. Ideal for building custom languages, transpilers, or static analysis tools where error recovery and precise source mapping are critical.

### 2. Domain-Specific Languages (DSLs)
Rapidly prototype and deploy custom query languages, configuration formats, or embedded scripting engines. Define your AST and grammar in a single, readable file, and instantly generate the strongly-typed Rust scaffolding required to integrate it into your host application.

### 3. High-Throughput Data Ingestion
Parse structured logs, custom network protocols, or high-frequency telemetry data. The generated DFA lexer operates with branch-prediction-friendly, array-based state transitions, ensuring nanosecond-level latency per token even under massive load.

### 4. Systems & Embedded Programming
The generated code is explicitly designed to be `no_std` compatible (with minor adjustments). By avoiding heap allocations during the parsing phase and relying on explicit error types, `allsec` is perfectly suited for resource-constrained environments, kernel modules, or WebAssembly targets.

---

## ⚡ Key Technical Features

- **Parallelized DFA Compilation**: Leverages OpenMP to construct and minimize NFA-to-DFA state machines concurrently, drastically reducing generator startup time for complex lexers.
- **Arena-Allocated Meta-Parsing**: The generator itself uses a custom, cache-friendly arena allocator, ensuring the grammar compilation phase is virtually instantaneous and immune to memory fragmentation.
- **Smart Lifetime Inference**: The generator intelligently scopes `save_pos()` and `restore_pos()` operations, automatically satisfying the Rust borrow checker without requiring manual lifetime annotations in the grammar.
- **Rich Error Diagnostics**: Generates detailed, source-aware `ParseError` enums that track exact line and column numbers, enabling IDE-quality error reporting out of the box.

---

## 🛠️ Quick Start

Defining a language with `allsec` is a declarative, two-step process.

### 1. Define the Lexicon (`lexer.lex`)
Specify tokens, skip rules, and special lexical constructs using standard regular expressions.

```lex
%skip WHITESPACE "[ \t\n\r]+"
%skip LINE_COMMENT "//[^\n]*"
%skip BLOCK_COMMENT "/\*([^*]|\*[^/])*\*/"

%token BRING "bring"
%token IDENT "[a-zA-Z_][a-zA-Z0-9_]*"
%token NUMBER "[0-9]+(_[0-9]+)*"
%token PLUS "\+"
%token DOT "\."
%token SEMICOLON ";"
%token LBRACE "\{"
%token RBRACE "\}"
%token COMMA ","
```

read  [lexer.md](lexer.md)

### 2. Define the Grammar (`grammar.peg`)
Declare your Abstract Syntax Tree (AST) and Parsing Expression Grammar rules, complete with inline Rust semantic actions.

```text
%ast Program {
    Program { statements: Vec<Statement> }
}

%ast Statement {
    Import { path: String, children: Vec<String> },
    Expr { expr: Expression }
}

Program -> Program ::= stmts:Statement* {
    Program::Program { statements: stmts }
} ;

Statement -> Statement ::= ImportStmt | ExprStmt ;

ImportStmt -> Statement ::= 
    BRING path:IDENT DOT LBRACE first:IDENT rest:(COMMA IDENT)* RBRACE SEMICOLON {
        let mut v: Vec<String> = vec![first.lexeme.to_string()];
        for (_, id) in rest { v.push(id.lexeme.to_string()); }
        Statement::Import { path: path.lexeme.to_string(), children: v }
    } ;
```
read  [parser.md](parser.md)


### 3. Generate
Invoke the toolchain to produce your optimized Rust source files.

```bash
# Generate the lexer
allsec -o src/lexer.rs < lexer.lex

# Generate the parser
allsec -o src/parser.rs --parser < grammar.peg
```

---

## 📈 Performance Profile

`allsec` is built to be compiled with aggressive, production-grade optimization flags. When building the generator itself, we recommend:

```bash
clang -O3 -ffast-math -march=native -mtune=native -flto=full \
      -mllvm -inline-threshold=15000 -mllvm -unroll-threshold=15000 \
      -funroll-loops -fno-stack-protector -fno-common -fno-plt \
      -fno-semantic-interposition -fomit-frame-pointer \
      -pipe -fuse-ld=mold -static -o allsec *.c
```
*Note: The generated Rust code inherently benefits from standard `cargo build --release` optimizations, requiring no special flags to achieve peak throughput.*

---

## 📜 License & Contribution

under MIT license

`allsec` is provided for engineers who demand excellence from their tooling. 

For bug reports, feature requests, or contributions regarding the core C generator, please refer to the project repository guidelines. We welcome rigorous, well-reasoned discourse on compiler design and optimization.

---
*Built for those who parse the world, one token at a time.*

---

# mahoraga94

Created: 2024-04-24T08:59:28Z
Updated: 2026-07-17T06:50:41Z
URL: https://github.com/mahoraga94/mahoraga94


## README

<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="https://capsule-render.vercel.app/api?type=waving&color=gradient&height=220&text=Mahoraga94&fontSize=60&fontAlignY=35&animation=twinkling&desc=Systems%20Artisan&descAlignY=55&descSize=22&fontColor=ffffff&bgColor=0d1117">
    <img src="https://capsule-render.vercel.app/api?type=waving&color=gradient&height=220&text=Mahoraga94&fontSize=60&fontAlignY=35&animation=twinkling&desc=Systems%20Artisan&descAlignY=55&descSize=22&fontColor=ffffff&bgColor=0d1117" alt="Mahoraga94 header">
  </picture>
</p>

<p align="center">
  <a href="https://git.io/typing-svg">
    <img src="https://readme-typing-svg.herokuapp.com?font=Fira+Code&size=24&duration=1800&pause=800&color=FF6A00&center=true&vCenter=true&width=850&lines=Adapting+to+any+abstraction%2C+one+segfault+at+a+time;Rust+%7C+C+%7C+OCaml+%7C+Haskell+%7C+NixOS;Building+compilers%2C+shells%2C+and+other+dark+magic;%22Software+should+explain+itself%22" alt="Typing SVG" />
  </a>
</p>

<br>

<!-- Animated line divider -->
<img src="https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/colored.png" width="100%">

<br>

### 👾 I write code that talks to the hardware so you don’t have to
…actually I just enjoy the pain. I craft **compilers**, **custom shells**, **binary analyzers**, and **systems languages** – the kind of tools that run fast, break less, and make other developers wonder if I’m okay.

<br>

<!-- ====== FEATURED PROJECTS ====== -->
<h2 align="center">
  <img src="https://media2.giphy.com/media/QssGEmpkyEOhBCb7e1/giphy.gif?cid=ecf05e47a0n3gi1bfqntqmob8g9aid1oyj2wr3ds3mg700bl&rid=giphy.gif" width="28px">
  &nbsp; Featured Creations &nbsp;
  <img src="https://media.giphy.com/media/uhQuegHFWkVZ4iGzjk/giphy.gif?cid=ecf05e47g3yh3r3b2m67e4m8s8cj5b2n9h1p0z4g3g7i8w1t&rid=giphy.gif" width="28px">
</h2>

<p align="center">
  <a href="https://github.com/mahoraga94/bure">
    <img height="160em" src="https://github-stats-extended.vercel.app/api/pin/?username=mahoraga94&repo=bure&theme=gruvbox&hide_border=true&show_owner=true" />
  </a>
  <a href="https://github.com/mahoraga94/sip">
    <img height="160em" src="https://github-stats-extended.vercel.app/api/pin/?username=mahoraga94&repo=sip&theme=gruvbox&hide_border=true&show_owner=true" />
  </a>
</p>

<p align="center">
  <a href="https://github.com/mahoraga94/tral">
    <img height="160em" src="https://github-stats-extended.vercel.app/api/pin/?username=mahoraga94&repo=tral&theme=gruvbox&hide_border=true&show_owner=true" />
  </a>
  <a href="https://github.com/mahoraga94/xinux">
    <img height="160em" src="https://github-stats-extended.vercel.app/api/pin/?username=mahoraga94&repo=xinux&theme=gruvbox&hide_border=true&show_owner=true" />
  </a>
</p>

<details>
<summary align="center"><b>📂 a few more projects…</b></summary>
<br>
<table align="center">
  <tr>
    <td align="center"><b>xasm-rs</b><br>Assembly generator<br><sub>Rust</sub></td>
    <td align="center"><b>negma</b><br>NixOS manager<br><sub>Rust</sub></td>
    <td align="center"><b>ruty</b><br>2D game engine<br><sub>Rust</sub></td>
    <td align="center"><b>ryx</b><br>Low‑level language<br><sub>Rust</sub></td>
  </tr>
  <tr>
    <td align="center"><b>nasembler</b><br>x86‑64 assembler<br><sub>Rust</sub></td>
    <td align="center"><b>gob</b><br>Universal package manager<br><sub>Rust</sub></td>
    <td align="center"><b>popoenos</b><br>Toy OS<br><sub>Rust</sub></td>
    <td align="center"><b>jimmer</b><br>Timer with music<br><sub>Rust</sub></td>
  </tr>
</table>
</details>

<br>

<img src="https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/colored.png" width="100%">

<br>

<!-- ====== TECH STACK ====== -->
<h2 align="center">
  <img src="https://media.giphy.com/media/j2pOGeGYKe2xCCKwfi/giphy.gif" width="28px">
  &nbsp; Tech I live in &nbsp;
  <img src="https://media.giphy.com/media/j2pOGeGYKe2xCCKwfi/giphy.gif" width="28px">
</h2>

<p align="center">
  <img src="https://img.shields.io/badge/Rust-000000?style=for-the-badge&logo=rust&logoColor=white" />
  <img src="https://img.shields.io/badge/C-A8B9CC?style=for-the-badge&logo=c&logoColor=white" />
  <img src="https://img.shields.io/badge/OCaml-EC6813?style=for-the-badge&logo=ocaml&logoColor=white" />
  <img src="https://img.shields.io/badge/Haskell-5D4F85?style=for-the-badge&logo=haskell&logoColor=white" />
  <img src="https://img.shields.io/badge/Nix-5277C3?style=for-the-badge&logo=nixos&logoColor=white" />
  <img src="https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black" />
  <img src="https://img.shields.io/badge/Neovim-57A143?style=for-the-badge&logo=neovim&logoColor=white" />
</p>

<br>

<img src="https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/colored.png" width="100%">

<br>

<!-- ====== GITHUB STATISTICS ====== -->
<h2 align="center">
  <img src="https://media.giphy.com/media/cj87CxfRtrUifF3Ryk/giphy.gif" width="28px">
  &nbsp; Stats that actually update &nbsp;
  <img src="https://media.giphy.com/media/cj87CxfRtrUifF3Ryk/giphy.gif" width="28px">
</h2>

<p align="center">
  <a href="https://github.com/stats-organization/github-stats-extended">
    <img height="170em" src="https://github-stats-extended.vercel.app/api?username=mahoraga94&show_icons=true&theme=gruvbox&hide_border=true&count_private=true&include_all_commits=true" />
  </a>
  <a href="https://github.com/stats-organization/github-stats-extended">
    <img height="170em" src="https://github-stats-extended.vercel.app/api/top-langs/?username=mahoraga94&layout=compact&theme=gruvbox&hide_border=true&langs_count=8&card_width=330" />
  </a>
</p>

<!-- Recent Activity Graph -->
<p align="center">
  <a href="https://github.com/ashutosh00710/github-readme-activity-graph">
    <img alt="Mahoraga94's Activity Graph" src="https://github-readme-activity-graph.vercel.app/graph?username=mahoraga94&theme=gruvbox&hide_border=true&area=true" />
  </a>
</p>

<!-- Trophy Case -->
<p align="center">
  <a href="https://github.com/ryo-ma/github-profile-trophy">
    <img src="https://github-profile-trophy.vercel.app/?username=mahoraga94&theme=gruvbox&no-frame=true&row=1&column=7" alt="trophy" />
  </a>
</p>

<!-- Contribution Snake -->
<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/platane/snk/output/github-contribution-grid-snake-dark.svg">
    <source media="(prefers-color-scheme: light)" srcset="https://raw.githubusercontent.com/platane/snk/output/github-contribution-grid-snake.svg">
    <img alt="github contribution grid snake animation" src="https://raw.githubusercontent.com/platane/snk/output/github-contribution-grid-snake.svg">
  </picture>
</p>

<br>

<img src="https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/colored.png" width="100%">

<br>

<!-- ====== CURRENT STATUS ====== -->
<h2 align="center">⚡ What I’m up to</h2>

<p align="center">
  🔭 <b>Building</b>: a compiler backend that doesn’t make me cry (still a work in progress)<br>
  🌱 <b>Learning</b>: formal verification & type theory – mostly for the aesthetic<br>
  💬 <b>Ask me about</b>: why NixOS is the one true OS, or how to write a shell in a weekend<br>
  🎮 <b>Fun fact</b>: my code runs faster than my social life
</p>

<br>

> *“Waste no more time arguing what a good person should be. Be one.”* – Marcus Aurelius  
> *“When you're hanging around trash long enough, you start to smell like it.”* – Bennett Foddy

<p align="center">
  <img src="https://img.shields.io/badge/Made%20with-%E2%9D%A4%EF%B8%8F%20and%20Rust-orange?style=flat-square" />
  <img src="https://img.shields.io/badge/Powered%20by-NixOS-blueviolet?style=flat-square&logo=nixos" />
</p>

<!-- Footer Wave -->
<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="https://capsule-render.vercel.app/api?type=waving&color=gradient&height=120&section=footer&bgColor=0d1117&fontColor=ffffff">
    <img src="https://capsule-render.vercel.app/api?type=waving&color=gradient&height=120&section=footer&bgColor=0d1117&fontColor=ffffff">
  </picture>
</p>


---

# tral

Created: 2026-07-17T04:25:19Z
Updated: 2026-07-17T04:56:42Z
URL: https://github.com/mahoraga94/tral


## README

# TRAL  
### *Software should explain itself (or at least try)*

[![Rust](https://img.shields.io/badge/Rust-1.70+-orange.svg)](https://www.rust-lang.org/)
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey.svg)]()

TRAL is a binary analysis thing I wrote because I kept forgetting what all those weird functions do. It parses ELF, PE, Mach‑O, even some EFI stuff, and then tells you stuff about the binary in plain English (or as close as I could get). No cloud, no telemetry – it all runs on your own machine. Good for reverse engineering, malware analysis, CTFs, or just poking around.

---

## Why tho?

Look, I got tired of running `strings` and then manually googling what each symbol means. So I dumped a bunch of descriptions into `knowntable.rs` (over 500 now?) and added some regexes for common patterns. It's not perfect, but it saves me hours. Maybe it'll help you too.

---

## What it can do (the good bits)

- **Multi‑format binary parsing** – ELF, PE, Mach‑O, TE (UEFI), COFF objects. The `goblin` crate does the heavy lifting.
- **Symbol resolution** – maps function names to what they *actually* do. Covers libc, POSIX, Windows API, crypto libraries, Rust runtime, Go, Zig, .NET… and a few malware‑related APIs because, well, you know.
- **String extraction & classification** – extracts ASCII strings (min length 4) and uses a bunch of heuristics to flag passwords, tokens, crypto headers, network indicators, red‑team one‑liners, PII like credit cards & emails, and even embedded file magic bytes.
- **Security hardening audit** – checks if PIE/ASLR, RELRO, NX/DEP, CFG, SEH, Authenticode signatures, etc. are enabled. Tells you if your binary is a soft target.
- **Three interfaces** because why not:
  - **CLI** – quick color‑coded terminal output.
  - **TUI** – a full‑screen Markdown viewer (`ratatui`) so you can scroll through reports without leaving the terminal.
  - **GUI** – dark‑mode hacker‑looking interface with interactive hex dump, sortable symbol/string tables, and even a Markdown renderer with math support.
- **Full Markdown export** – untruncated, all strings, all symbols, good for documentation or sharing with the team.

---

## Installation

1. Install Rust (https://rustup.rs) – I used 1.70, but anything recent should work.
2. Clone this repo:
   ```bash
   git clone https://github.com/yourusername/tral.git
   cd tral
   ```
3. Build it:
   ```bash
   cargo build --release
   ```
   It'll take a bit. There’s a lot of dependencies (clap, egui, ratatui, goblin…). If you get some font‑related warnings in the GUI part, ignore them unless you're picky.
4. Optionally install to your path:
   ```bash
   cargo install --path .
   ```
   Then you can just type `tral`.

**Note for Windows**: I haven't tested the TUI extensively on Powershell/cmd, but it should work in Windows Terminal. The GUI (eframe/egui) works fine on all major OSes.

---

## Usage (quickstart)

If you run `tral` without any arguments, it launches the GUI. For CLI stuff:

```bash
# Basic header + security analysis
tral /bin/ls

# Show known (notable) symbols
tral -s /bin/ls

# Show every single symbol, even the boring ones
tral --symbols-all /bin/ls

# Extract and show classified strings
tral --strings /bin/ls

# All strings, no length truncation
tral --strings-all /bin/ls

# Export a full Markdown report to a file
tral -e my_analysis.md /bin/ls

# Preview a saved report in the terminal (TUI)
tral -p my_analysis.md
```

The TUI uses vim‑like keys: `j/k` to scroll, `PgUp/PgDn`, `Home/End`, `q` to quit.

---

## Deep dive into the analysis

### Security stuff

TRAL doesn't just print a "yes/no"; it explains what each mitigation does in the context of that format. For ELF:
- PIE (Position Independent Executable) → ASLR friendliness.
- RELRO → Read‑only GOT, prevents overwrite attacks.
- NX → No‑execute stack.
- CET (Intel IBT/SHSTK) – if the note is present.

For PE:
- ASLR (Dynamic Base), High Entropy VA.
- DEP (NX compatible).
- Control Flow Guard, SEH disabled, AppContainer, Authenticode.

Mach‑O checks if the symbol table is stripped, if DWARF debug info exists, and whether the binary is code‑signed.

### Symbol resolution (the big table)

`knowntable.rs` is a giant `match` statement (seriously, it’s hundreds of lines) that maps symbol names to descriptions. Some examples:
- `malloc` → "Allocates a block of memory from the heap. If it fails, it returns NULL."
- `VirtualAllocEx` → "Allocates memory in a remote process. Commonly used by malware for injection."
- `ptrace` → "Used by debuggers to trace another process. Often checked by malware to detect debugging."
- `rust_begin_unwind` → "Rust's panic handler entry point. Not a sign of trouble by itself, but if you see it in a stripped binary, it’s probably a Rust program."
- `SHA256_Init` → "Initializes a SHA‑256 hashing context (OpenSSL)." – and so on.

Unresolved symbols (ones I haven't added yet) still get listed with their address, size, type, and binding. Over time I’ll expand the database, but PRs are welcome!

### String analysis

The `knownstrings.rs` module uses regex and some simple entropy checks to figure out what a string might be. It looks for:
- **Credentials**: AWS keys, GitHub tokens, Slack webhooks, Stripe API keys, JWTs, generic high‑entropy base64.
- **Crypto**: PEM headers, algorithms (AES, ChaCha20, SHA‑256, Argon2), hash strings of exact lengths (32, 40, 64 chars…).
- **Network**: URLs, WebSocket URIs, IRC, LDAP, SMB paths, User‑Agents.
- **Malware indicators**: PowerShell download cradles, reverse shell commands, registry persistence paths, DGA‑looking domain names.
- **PII**: Email addresses, credit card numbers, US SSNs, phone numbers, UUIDs.
- **Embedded files**: Magic bytes for ZIP, PDF, GZIP, PNG, JPEG, even nested executable headers.

The GUI will even flag those in **red** so you don't miss them.

---

## The GUI

Launch it with `tral` (or `tral --gui`). It has tabs:

- **Info**: collapsible sections with binary metadata and security evaluation.
- **Symbols**: a filterable table (by address, type, binding, name). Known symbols get a ✓ and a tooltip with the description.
- **Strings**: you can filter by min length, search substring, and choose between "All", "Known" (classified), or "Unknown" (just raw). Suspicious items are highlighted.
- **Hex Dump**: continuous scroll, click a byte to see offset, hex, decimal, binary. I spent too long on this one.
- **Markdown**: if you open a `.md` file, it renders it with code blocks and tables, and even inline math (`$...$`) using KaTeX.

It’s built with `egui`/`eframe`, so it’s pretty snappy even with large binaries. Background threads parse the file without locking the UI.

---

## Project layout (so you know what’s where)

- `main.rs` – CLI args (clap) and routing.
- `gui.rs` – all the egui code (the biggest file, probably messy).
- `preview.rs` – the ratatui Markdown viewer.
- `printbin.rs` – CLI formatting for headers and security.
- `report.rs` – generates the Markdown report.
- `stmap.rs` – extracts and normalizes symbols across formats.
- `strings.rs` – string extraction and CLI table printing.
- `knownstrings.rs` – the regex/heuristic engine for classifying strings.
- `knowntable.rs` – the huge map of symbol descriptions.

Everything is synchronous except the GUI worker threads. No async runtime because I didn't need it.

---

## Contributing

If you want to add more symbols or string patterns, just edit `knowntable.rs` or `knownstrings.rs`. I tried to make the format easy to copy‑paste. If you break something, `cargo clippy` will complain. Also, the GUI code is a bit of a mess, so if you refactor it I'll be grateful.

---

## License

MIT. Use it, modify it, steal it for your own tools – I don't care.

---

> *"Software should explain itself."* – Me, after staring at Ghidra for too long.

---

# ringest

Created: 2026-07-16T16:12:06Z
Updated: 2026-07-16T16:14:48Z
URL: https://github.com/mahoraga94/ringest


## README

# ringest

_– because `gitingest` asked for your API key and you just wanted a local damn text dump._

---

## yo, what is this thing?

ringest walks into your project folder, hoovers up all the files you care about (or all of them, if you're not picky), slaps a nice header on each one, and dumps everything into a **single clean `.txt`** or straight to your terminal.

it doesn't talk to the internet. it doesn't read git. it doesn't even ask your name. it just reads bytes and writes bytes. good bytes.

---

## why ringest over the other guy?

 - zero network calls, zero logins, zero telemetry
 - runs cold, like a fridge, offline
 - dirt simple: point → filter → output
 - so fast your SSD barely wakes up
 - written in Rust, so you know it's not lying about the "fast" part

---

## ok, how do i shove it into my machine?

```bash
cargo install ringest
```

(or clone the repo and `cargo build --release` if you're into that manual feel)

---

## the sacred usage line

```
ringest [OPTIONS] [paths...]
```

if you run it naked (no paths), it prints help and exits. no guessing, no weird defaults.

---

## the funny flags

| flag | what it does |
|------|--------------|
| `--ext rs,py,js` | only take files that end with these. case doesn't matter. |
| `--min-size 1K` | skip anything smaller (suffixes: K, M, G, or raw bytes) |
| `--max-size 5M` | skip anything bigger (yes, you can combine both) |
| `--exclude "target/**"` | chuck out paths that match this glob. use many times. |
| `-a` / `--include-hidden` | grab those sneaky dotfiles too |
| `-o FILE` | put everything in one file instead of auto-naming |
| `--stdout` | spray the output to the terminal, no file at all |
| `-s` / `--summary` | just count and measure, don't write anything |
| `-q` / `--quiet` | shut up the progress bar and warnings |
| `--no-parallel` | read files one by one, like the old days |

---

## examples that actually work

```bash
# whole current folder -> creates "current.txt"
ringest .

# two specific files -> "main.rs.txt" and "lib.rs.txt"
ringest src/main.rs src/lib.rs

# only .rs and .toml from src, no dotfiles
ringest --ext rs,toml src/

# one big bundle, ignore lock files
ringest -o massive_dump.txt --exclude "*.lock" .

# print to terminal, show what's happening
ringest --stdout --include-hidden my_project

# quick check: how much code do i even have
ringest -s --ext rs,ts,py .
```

---

## size filter tricks

- `--min-size 100` → "at least 100 bytes, please"
- `--max-size 2M` → "no larger than 2 megs, my terminal can't handle it"
- together they make a "goldilocks zone" for files. `ringest --min-size 50 --max-size 500K` means no tiny empty files, no big honking blobs.

---

## what the output looks like

each file gets a header that screams:

```
──────────────────────────────────────────────────────────────
src/weird_utils.rs
┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄ :— 
pub fn do_weird() { ... }
```

the LLM sees this and knows exactly where things start. the human sees it and thinks "hm, clean."

---

## hidden file logic (a.k.a. dotfile drama)

by default, anything starting with a dot (except `.` and `..`) gets the boot. if you're one of those folks with a `.env` that actually matters, use `-a` to bring it in from the cold.

---

## glob patterns

they work on the path relative to where you run the command. `--exclude "**/*.log"` nukes all log files. `--exclude "target/"` nukes the build folder. it's basic glob, same as `.gitignore`, no regex acrobatics needed.

---

## when nothing matches

ringest doesn't explode. it just prints a polite warning (unless you're in quiet mode) and exits clean. no error, no red text screaming.

---

## speed vibes

- parallel reading by default (all your cores)
- single-threaded if you add `--no-parallel` (maybe you're on a potato)
- binary files get ignored silently (we assume UTF‑8 or bust)

if you're processing a million tiny files, go make a coffee — but ringest won't be the slow part.

---

## common mishaps

- "I forgot `-a` and my `.env` didn't show up" — yeah, pass `-a`.
- "my huge `node_modules` got included" — add `--exclude "node_modules/**"` next time.
- "it says no files matched" — check your `--ext` or size filters, maybe you filtered too hard.

---

## go, do the thing

ringest is small, opinionated, and does exactly one job. no config files, no weird yaml, no dashboards. just the files you have, turned into a single text block. run it, pipe it, save it, whatever. now go ingest some files.

---

# bure

Created: 2026-06-11T08:48:18Z
Updated: 2026-06-11T10:05:26Z
URL: https://github.com/mahoraga94/bure


## No README


---

# negma

Created: 2025-06-29T12:39:52Z
Updated: 2025-06-29T14:41:47Z
URL: https://github.com/mahoraga94/negma


## README

# ✨ `negma` – Clean NixOS & Home Manager CLI ✨

> **A fast, minimal, colored CLI to manage NixOS + Home Manager confidently and cleanly in your daily workflow.**

---

## 🌱 What is `negma`?

`negma` is a **lightweight Rust CLI** that helps you:

✅ Rebuild and garbage collect your NixOS system effortlessly.  
✅ Manage Home Manager generations and rollbacks simply.  
✅ Auto-run garbage collection after a configurable number of days.  
✅ Edit and auto-format your `negma` configuration easily.  
✅ All with **clean, colored, minimal CLI output** designed for clarity.

---

## ✨ Features

### 🖥️ NixOS System Management
- `nix make` – rebuild and switch to the new system configuration
- `nix gc` – garbage collect old generations
- `nix list-generations` – list system generations with clarity
- `nix rollback [gen]` – rollback to a specific system generation

### 🏡 Home Manager Management
- `home make` – apply Home Manager configuration
- `home edit` – edit your `home.nix` easily
- `home gc` – garbage collect Home Manager generations
- `home backup` – backup your `home.nix` safely
- `home list-generations` – list Home Manager generations
- `home rollback [gen]` – rollback to a specific Home Manager generation

### ⚙️ Configuration Management
- `edit-cfg` – edit your `negma` configuration with auto-formatting if enabled

### ♻️ Auto GC
- Automatically runs `nix-collect-garbage` after N days.
- Uses a marker file in `~/.config/negma/` to track last run cleanly.

---

## 🚀 Installation

### 🔧 Build from source (recommended)

```bash
git clone https://github.com/skubed0007/negma
cd negma
cargo build --release
# Optionally move to your PATH:
sudo cp target/release/negma /usr/local/bin
```

### 📦 From Release

You may find prebuilt binaries for NixOS in the [Releases](https://github.com/skubed0007/negma/releases) section.

---

## ⚙️ Configuration

Your config file is located at:

```
~/.config/negma/config.cfg
```

Edit it easily with:

```bash
negma edit-cfg
```

> # Important: please read default config created by negma to get to know all the options

---

## 🛠 Usage

Run without arguments to see available commands:

```bash
negma
```

Example usage:

```bash
negma home make
negma home edit
sudo negma nix gc
negma edit-cfg
```

---

## 🖤 Clean, Colored, Readable Output

- Uses `colored` for clear status outputs.
- Aligned and minimal, no overwhelming logs.
- Shows **clear success/error** with hints.

---

## 📄 License

Apache 2.0 License

---

## 🤝 Contributing

Contributions are welcome:
- CLI UX improvements
- Flake integration
- Additional workflow hooks
- Bug fixes

---

> **Happy hacking & keep your NixOS clean! 🩶✨**

---

## 📫 Contact

- **Author:** [skubed0007](https://github.com/skubed0007)
- Feel free to open issues for feature requests, questions, or enhancements!

---

# lovestory

Created: 2025-06-21T11:38:26Z
Updated: 2025-06-21T11:38:53Z
URL: https://github.com/mahoraga94/lovestory


## No README


---

# futils

Created: 2025-06-16T14:49:34Z
Updated: 2025-06-16T14:49:55Z
URL: https://github.com/mahoraga94/futils


## README



---

# loveletter

Created: 2025-06-15T03:48:09Z
Updated: 2025-06-18T04:40:23Z
URL: https://github.com/mahoraga94/loveletter


## No README


---

# eagledb

Created: 2025-06-11T15:13:19Z
Updated: 2025-06-11T15:19:22Z
URL: https://github.com/mahoraga94/eagledb


## README

# EagleDB 🦅

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](LICENSE)
[![Rust Version](https://img.shields.io/badge/rust-1.70+-blue.svg)](https://www.rust-lang.org)
[![Build Status](https://github.com/yourusername/eagledb/actions/workflows/rust.yml/badge.svg)](https://github.com/yourusername/eagledb/actions)

EagleDB is a high-performance, memory-mapped key-value database written in Rust. It's designed for extreme performance and reliability, making it perfect for applications that require fast data access and persistence.

## 🌟 Features

- **Memory-Mapped Storage**: Utilizes memory mapping for efficient data access
- **Concurrent Operations**: Thread-safe operations with efficient locking mechanisms
- **SIMD Optimizations**: Leverages CPU SIMD instructions for faster data processing
- **Configurable Memory**: Dynamic memory allocation based on system resources
- **Built-in Compression**: Optional data compression for storage efficiency
- **Progress Tracking**: Real-time progress indicators for long-running operations
- **Error Handling**: Comprehensive error handling with detailed error messages
- **CLI Interface**: User-friendly command-line interface for database operations

## 📋 Requirements

- Rust 1.70 or higher
- Linux/Unix-like operating system (for optimal performance)
- Sufficient system memory (recommended: 4GB+)

## 🚀 Installation

1. Clone the repository:
```bash
git clone https://github.com/yourusername/eagledb.git
cd eagledb
```

2. Build the project:
```bash
cargo build --release
```

3. Install globally (optional):
```bash
cargo install --path .
```

## 💻 Usage

### Basic Commands

```bash
# Initialize configuration
eagledb init

# Create a new database
eagledb create [config.json]

# Query the database
eagledb query <query_string> [database.db]

# Show database statistics
eagledb stats [database.db]

# Display current configuration
eagledb config

# Create a test database
eagledb testdb
```

### Query Examples

```bash
# Query by hex key
eagledb query "key: 0x1234567890abcdef"

# Query by base64 key
eagledb query "key: EjRWeJCrze8="

# List all records
eagledb query "list"

# Find maximum value
eagledb query "max"

# Find minimum value
eagledb query "min"
```

## 🔧 Configuration

The default configuration is automatically generated based on your system's resources. You can customize it by editing `config.json`:

```json
{
    "database_path": "MAIN.db",
    "memory_mapped_size": 67108864,
    "value_size": 1048576,
    "max_records": 1000000,
    "compression_enabled": false,
    "cache_size": 33554432
}
```

## 🛠️ Development

### Building from Source

```bash
# Clone the repository
git clone https://github.com/yourusername/eagledb.git
cd eagledb

# Build in debug mode
cargo build

# Build in release mode
cargo build --release

# Run tests
cargo test

# Run benchmarks
cargo bench
```

### Project Structure

```
eagledb/
├── src/
│   ├── main.rs      # CLI interface and main program logic
│   ├── engine.rs    # Core database engine
│   ├── storage.rs   # Memory-mapped storage implementation
│   ├── error.rs     # Error handling
│   ├── config.rs    # Configuration management
│   ├── parser.rs    # Query parsing
│   └── generator.rs # Test data generation
├── tests/           # Test suite
├── benches/         # Benchmarks
├── Cargo.toml       # Project dependencies
└── README.md        # This file
```

## 🤝 Contributing

We welcome contributions! Please see our [Contributing Guidelines](CONTRIBUTING.md) for details.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📝 License

This project is licensed under the Apache License 2.0 - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- [Rust](https://www.rust-lang.org/) - The programming language
- [memmap2](https://crates.io/crates/memmap2) - Memory mapping implementation
- [crossbeam](https://crates.io/crates/crossbeam) - Concurrent programming utilities
- [parking_lot](https://crates.io/crates/parking_lot) - Efficient synchronization primitives
- [rayon](https://crates.io/crates/rayon) - Parallel iterators
- [dashmap](https://crates.io/crates/dashmap) - Concurrent hashmap
- [tokio](https://crates.io/crates/tokio) - Async runtime
- [serde](https://crates.io/crates/serde) - Serialization framework

## ⭐ Show your support

Give a ⭐️ if this project helped you! 

---

# penp1

Created: 2025-06-10T10:42:07Z
Updated: 2025-06-10T12:04:05Z
URL: https://github.com/mahoraga94/penp1


## README

# Penp: Geometric Path Compression & TSP Analysis

This project analyzes the efficiency of geometric path compression algorithms compared to the optimal Traveling Salesman Problem (TSP) solution across different city arrangements (linear, grid, circle). It generates summary statistics and visualizations for step/cost reduction as the number of cities increases.

## Features
- Generates cities in linear, grid, and circular arrangements
- Computes optimal TSP path using brute-force (for small N)
- Computes a geometric path compression (greedy by x-coordinate)
- Compares path length and cost between methods
- Outputs summary statistics to the console
- Generates graphs for step/cost reduction and efficiency by geometry

## Usage
1. **Build the project:**
   ```sh
   cargo build --release
   ```
2. **Run the analysis:**
   ```sh
   cargo run --release
   ```
   Optionally, repeat each test multiple times:
   ```sh
   cargo run --release -- --repeat 10
   ```
3. **View results:**
   - Summary statistics are printed to the console.
   - Graphs are saved in the `tsp_analysis/` directory:
     - `step_reduction.png`
     - `cost_analysis.png`
     - `efficiency_by_geometry.png`

## Requirements
- Rust (edition 2021 or later)
- [plotters](https://crates.io/crates/plotters) crate for graphing
- [itertools](https://crates.io/crates/itertools) for permutations

## License
This project is licensed under the [Apache License 2.0](LICENSE).

## Acknowledgments
- Brute-force TSP is only feasible for small city counts (default: up to 30).
- Geometric path compression is a simple greedy baseline for comparison.

---

*Feel free to modify or extend for your own research or visualization needs!*


---

# ruty

Created: 2025-06-07T16:12:03Z
Updated: 2025-06-09T14:20:06Z
URL: https://github.com/mahoraga94/ruty


## README

# Ruty Game Engine
<img width=200em height=200em src="rsrcs/icon.png"><br>
Ruty is a modern, 2D game engine written in Rust, built on top of [macroquad](https://github.com/not-fl3/macroquad). It demonstrates a component-based architecture inspired by engines like Unity, making it easy to add, remove, and combine behaviors for game objects.

## 🌟 Features

### Core Engine
- **Component System**: Attach modular behaviors (gravity, friction, collision, force, etc.) to game objects
- **Physics Simulation**: Advanced physics with gravity, friction, and force application
- **Collision Handling**: Sophisticated collision detection and resolution with slope support
- **Input Handling**: Robust keyboard and mouse input management
- **Rendering**: High-performance 2D rendering with support for shapes and text

### UI System
- **Modern UI Components**: Buttons, panels, inputs, sliders, checkboxes, and more
- **Theme System**: Consistent styling with dark/light mode support
- **Animation System**: Smooth transitions and effects for all UI elements
- **Event System**: Robust event handling with callbacks
- **Z-Ordering**: Proper layering of UI elements

### Physics Features
- **Gravity**: Configurable gravity strength and direction
- **Friction**: Surface and air friction with customizable coefficients
- **Collision**: AABB collision detection with bounce and slope support
- **Force System**: Support for both temporary and permanent forces
- **Constraint System**: Create complex physics-based shapes and joints

## 📦 Project Structure

```
src/
├── basics/           # Core game mechanics
│   ├── collision.rs  # Collision detection and response
│   ├── force.rs      # Force application system
│   ├── friction.rs   # Friction simulation
│   └── gravity.rs    # Gravity implementation
├── objects/          # Game objects
│   ├── quad.rs       # Basic rectangular object
│   ├── point.rs      # Physics point for constraints
│   ├── shapes.rs     # Pre-built shape generators
│   └── ui.rs         # UI component system
├── utils/            # Utility functions
│   ├── font_text.rs  # Font loading and text rendering
│   └── screen.rs     # Screen management
├── test/             # Test framework
│   ├── mod.rs        # Test module definitions
│   └── ui_test.rs    # UI component tests
└── main.rs           # Main application entry
```

## 🚀 Quick Start

```rust
use ruty::objects::ui::{Theme, UiButton, UiPanel};
use ruty::basics::{Gravity, Collision, Friction};

// Create a game object with physics
let mut cube = Quad::new(200.0, 0.0, 50.0, 50.0, WHITE);
cube.add_component(Box::new(Gravity::new(0.5)));
cube.add_component(Box::new(Collision::new(0.8, 0.9)));
cube.add_component(Box::new(Friction::new(0.85)));

// Create a UI panel
let theme = Theme::default();
let mut panel = UiPanel::new(
    20.0, 20.0, 300.0, 500.0,
    theme.clone(),
    Some("Game UI".to_string())
);

// Add a button
let button = UiButton::new(
    "Click Me",
    40.0, 60.0, 220.0, 40.0,
    18, font.clone(), theme.clone(),
    Some(Box::new(|| println!("Button clicked!")))
);
panel.add_element(Box::new(button));
```

## 🎮 Controls

- **A/D**: Move left/right
- **Space**: Jump (when on ground)
- **Shift**: Sprint
- **Mouse**: Interact with UI elements

## 🛠️ Building & Running

Make sure you have Rust and Cargo installed. Then run:

```bash
cargo run
```

## 📚 Documentation

For detailed documentation, see the [docs.md](docs.md) file.

## 🔧 Dependencies
- [macroquad](https://crates.io/crates/macroquad) - Graphics, input, and windowing
- [image](https://crates.io/crates/image) - Image handling (optional)

## 📝 License

MIT License. See [LICENSE](LICENSE) for details.

---

*Ruty Game Engine – Building games with Rust, one component at a time!*


---

# sip

Created: 2025-04-09T15:17:26Z
Updated: 2026-04-24T03:41:34Z
URL: https://github.com/mahoraga94/sip


## README

<img src="logo.png" alt="Sip" width="200">

# Sip Programming Language
**Sip** is a lightweight, blazingly fast systems programming language.  
Its unique syntax, external/C interop, rich error reporting, and a tiny AST make it perfect for crafting high performance, low level code.

---

## 🚀 Features

- **C‑style syntax**  
- **`extern` / `link` ** for seamless C interoperability  
- **Powerful error reporting** with `ErrT` kinds (`ExpectName`, `MissingSemicolon`, etc.)  
- **Flexible AST** (`NT::Extern`, `NT::Link`, `NT::fncall`)  
- **Zero‑allocation identifiers** via `smallstr` inline buffers  
- **Simple lexer** supporting comments, numbers, strings, operators, and more  
- **Parser modules** for `extern`, `link`, function calls, etc.  
- **Memory‑mapped input** for blazing‑fast file access  
- **Configurable build** with optional features  
- **Colorful diagnostics** that guide you to fix issues quickly  
- **Extensible design**—add new AST nodes, error kinds, or parse rules easily

---

## 📦 Installation

Follow these steps to get **Sip** up and running on your system:

1. **Prerequisites**  
   - Rust toolchain (Rust 1.60+).  
     Install Rust with [rustup](https://rustup.rs/):  
     ```bash
     curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
     ```  
   - Ensure you have `git`, `cargo`, and a C compiler if you plan to link against native libraries.

2. **Clone the Repository**  
   ```bash
   git clone https://github.com/your‑username/sip.git
   cd sip
   ```

3. **Build in Debug Mode**  
   ```bash
   cargo build
   ```

4. **Build in Release Mode**  
   ```bash
   cargo build --release
   ```

5. **(Optional) Install to PATH**  
   ```bash
   sudo cp target/release/sip /usr/local/bin/
   ```

6. **Verify Installation**  
   ```bash
   sip --help
   ```

OR

just download the binary file from [here](bin) and place it on your path
---

## ⚙️ Usage

After building, you can run **Sip** directly:

```bash
sip <SOURCE_FILE>
```

### Example

Given a source file `example.sip`:

```c
//link unistd.h
link "unistd.h";
extern write fd(i32) /*give optional types*/ text len(i32) = "write" fd text len;
//call the function now
write(1,"hello world",12);
```

---

## 📂 Project Structure

```
sip/
├── Cargo.toml               # Project manifest
├── LICENSE                  # MIT License
├── README.md                # ← this file
└── src/
    ├── main.rs              # CLI entrypoint & ANSI setup
    ├── lexer.rs             # Byte‐level tokenizer
    ├── parsers/
    │   ├── mod.rs           # Parser entrypoint
    │   ├── externp.rs       # `extern` declarations
    │   ├── linkp.rs         # `link` / `clink` parsing
    │   ├── fncall.rs        # Function‐call parsing
    │   └── other.rs         # Other parse modules
    ├── ast.rs               # AST node & `Var` enums
    ├── tokdefs.rs           # Token‐type definitions
    ├── errt/
    │   ├── t.rs             # `ErrT` & `SErr`
    │   └── d1.rs            # `DErr` diagnostic renderer
    └── codegen/             # code generation 
```

---

## 📐 Language Specification

### Lexical Elements

1. **Identifiers**  
   - `[a-zA-Z_][a-zA-Z0-9_]*`  
   - Quoted identifiers: `"..."` (double-quoted)  
2. **Literals**  
   - Integer: `0-9+` (decimal)  
   - Floating point: digits with `.`  
3. **Operators**  
   - Arithmetic: `+`, `-`, `*`, `/`  
   - Assignment: `=`  
   - Variadic: `...`  
4. **Delimiters**  
   - Parentheses: `(`, `)`  
   - Braces: `{`, `}`  
   - Brackets: `[`, `]`  
   - Semicolon: `;`  
   - Comma: `,`  

### Grammar (EBNF)

```ebnf
<program>      ::= { <declaration> | <link> | <statement> }
<declaration>  ::= "extern" <sip-ident> "(" [<param-list>] ")" "=" <c-ident> "(" [<ctype-list>] ")" ";"
<link>         ::= "link" <string-literal> { <string-literal> } ";"
<statement>    ::= <function-call> ";"

<param-list>   ::= <typed-var> { "," <typed-var> } 
<ctype-list>   ::= <c-type> { "," <c-type> }
<typed-var>    ::= <sip-ident> "(" <c-type> ")" | "..."
<c-type>       ::= "i32" | "f32" | custom ident

<function-call>::= "call" <sip-ident> "(" [<arg-list>] ")"
<arg-list>     ::= <literal> | <identifier> | <function-call> | <list> { "," ... }
```

---

## 📜 Detailed Components

### Lexer (`lexer.rs`)

- **Whitespace & Comments**: Skips spaces, tabs, newlines, `//`, `/* ... */`  
- **Token Types**:  
  - `TT::IDENT`, `TT::NUM`, `TT::OP`, etc.  
  - Quoted strings: `TT::IDENT(IDT::DQ, ...)`  
- **Error Tokens**: `TT::ErrT` for unexpected characters  

### Parser (`parsers/`) Modules

- **`externp.rs`**  
  - Parses `extern` declarations  
  - States: `ExpectSipName`, `ExpectSipArgOrEq`, `ExpectEq`, `ExpectCName`, `ExpectCArgOrSemicolon`, `Done`  
  - Builds `NT::Extern(sipname, sipfnargs, cname, cfnargs)`  
  - Error cases: missing identifier, missing `=`, unexpected token in arg lists, missing semicolon  
  - Uses `parse_typed_var` to handle typed parameters and variadic `...`

- **`linkp.rs`**  
  - Parses `link` / `clink` directives  
  - Consumes tokens until `;`, collecting double‑quoted identifiers  
  - Builds `NT::Link(Vec<SmallString>)`  
  - Error cases: no libraries before `;`, unexpected tokens, EOF without `;`

- **`fncall.rs`**  
  - Parses function calls via `call <name>(...) ;`  
  - Looks up `extern` declarations in AST to validate signatures  
  - Iterates over expected parameters, consuming commas and arguments  
  - Supports nested calls, numeric literals, lists, variadic arguments  
  - Builds `NT::fncall(SmallString::from(fnname), args)`  
  - Error cases: unexpected token, mismatched types, missing `)`, missing `;`

- **Error Handling**  
  - All parsers accumulate `Vec<SErr>` on failure  
  - Return `Err(errs)` if any parse errors occurred  
  - Top‑level `parse()` returns either `Ok(Vec<NT>)` AST or `Err(Vec<SErr>)`

### Error Types (`errt/t.rs`)

- **`ErrT`** enum  
  - Core variants:  
    - `ExpectName`  
    - `UnexpectTok`  
    - `MissingSemicolon`  
    - `UnterminatedLiteral`  
    - `InvalidNumber`  
    - `UnknownKeyword`  
    - `UnmatchedParen` / `UnmatchedBrace` / `UnmatchedBracket`  
    - `ExpectOperator` / `ExpectExpression`  
    - `Generic(String)`  
  - Each paired with human‑readable message in `d1.rs`

- **`SErr`** struct  
  - Fields:  
    - `t: ErrT` – error kind  
    - `line: i32` – 1‑based line number (or `-1` for missing location)  
    - `start: i32`, `end: i32` – character indices in source line  
    - `file: String` – file path for context  

- **`DErr`** (`errt/d1.rs`)  
  - Renders colorful diagnostics:  
    1. `error[E{line:04}]: {kind}`  
    2. `  --> file:line:col` location bar  
    3. Source frame with `╭─[source]`, code line, caret(s) under the error span  
    4. Detail message under caret(s)  
    5. Optional `[help]` section with suggestions  
    6. Optional `note:` with extra context  

---

***The Sip Programming language and all its source files , source codes and tools present in the repo found here: https://github.com/skubed0007/sip is licensed under MIT License found here: [LICENSE](LICENSE)***


---

# owormr

Created: 2025-05-25T06:02:46Z
Updated: 2025-05-25T06:09:25Z
URL: https://github.com/mahoraga94/owormr


## No README


---

# sipsite

Created: 2025-04-10T15:28:48Z
Updated: 2025-05-12T11:38:22Z
URL: https://github.com/mahoraga94/sipsite


## No README


---

# popoenos

Created: 2025-05-08T12:54:10Z
Updated: 2025-05-08T15:59:20Z
URL: https://github.com/mahoraga94/popoenos


## README

# Popoen

![alt text](image.png)

Popoen is a minimal operating system written in Rust that implements a basic but functional kernel with essential features. It provides a foundation for learning OS development concepts through practical implementation.

## Features

- **Custom Shell**: A simple shell with support for:
  - File operations (`ls`, `cat`, `touch`)
  - File content manipulation (`>>` for appending text)
  - System commands (`clear`, `exit`)
  - System information (`pofetch`)
- **Keyboard Driver**: Advanced PS/2 keyboard support with:
  - Full scancode set 2 implementation
  - Modifier key support (Shift, Caps Lock, Ctrl, Alt)
  - Special key handling (Function keys, Arrow keys, etc.)
  - Extended scancode support
- **VGA Text Mode Interface**:
  - 80x25 text mode display
  - Color support with ANSI-style formatting
  - Cursor management
  - Scrolling support
- **Basic File System**:
  - Inode-based structure
  - Support for basic file operations
  - Directory entry management
  - Block device abstraction

## Getting Started

### Prerequisites

- Rust nightly toolchain
- `bootimage` installed (`cargo install bootimage`)
- QEMU or similar emulator for testing
- `rust-src` component (`rustup component add rust-src`)

### Building the Project

1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd popoen
   ```

2. Build the project:
   ```bash
   cargo build
   ```

3. Create a bootable image:
   ```bash
   cargo bootimage
   ```

### Running the OS

Run the OS in QEMU:
```bash
qemu-system-x86_64 -drive format=raw,file=target/x86_64-unknown-none/debug/bootimage-popoen.bin
```

## Usage

### Shell Commands

- `ls`: List files in the current directory
- `cat <filename>`: Display contents of a file
- `touch <filename>`: Create a new empty file
- `>> <filename> <text>`: Append text to a file
- `clear`: Clear the screen
- `pofetch`: Display system information with ASCII art
- `exit`: Exit the shell

### File System Operations

The file system supports basic operations through an inode-based structure with:
- Direct block pointers
- File modes and permissions
- Directory entries with name-to-inode mapping

### Project Structure

- `src/`
  - `main.rs`: Kernel entry point and initialization
  - `driver/`
    - `keyboard.rs`: PS/2 keyboard driver implementation
    - `shell.rs`: Interactive shell implementation
  - `fs/`
    - `structure.rs`: File system data structures
    - `utils.rs`: File system utility functions
  - `apps/`
    - `pofetch.rs`: System information display
  - `polib/`
    - `print.rs`: VGA text mode interface
    - `macros.rs`: Utility macros for printing

## License

This project is licensed under the GNU General Public License v3.0. See the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Inspired by the [Writing an OS in Rust](https://os.phil-opp.com/) series
- Uses the `bootloader` crate for bootstrapping
- Built with Rust's no-std ecosystem for bare metal development

---

Feel free to contribute or report issues to improve this project!

---

# tetesay

Created: 2025-04-30T12:31:13Z
Updated: 2025-04-30T12:34:14Z
URL: https://github.com/mahoraga94/tetesay


## README

# Tetesay
![alt text](image.png)

---

# joy-nixos-configs

Created: 2025-03-20T17:03:44Z
Updated: 2025-04-14T07:03:04Z
URL: https://github.com/mahoraga94/joy-nixos-configs


## No README


---

# sipls-vsc

Created: 2025-04-12T11:24:51Z
Updated: 2025-04-12T11:39:42Z
URL: https://github.com/mahoraga94/sipls-vsc


## README

# SIP Language Extension for VS Code

A powerful and modern extension for the SIP programming language, providing syntax highlighting, snippets, and language support in VS Code.

## About SIP

Visit [siplang.vercel.app](https://siplang.vercel.app) to learn more about the language and its features.

## Features

- Syntax highlighting for `.sip` files
- Code snippets for common patterns
- Markdown code block support
- Theme support
- Smart completions
- Language configuration

## Credits

This extension is inspired by and built upon the excellent work of the Gleam language and its VS Code extension. We extend our gratitude to:

- [Gleam Language](https://gleam.run) - For inspiration and language design patterns
- [Gleam VS Code Extension](https://github.com/gleam-lang/vscode-gleam) - For the foundation of our extension

## Installation

1. Open VS Code
2. Go to the Extensions view (Ctrl+Shift+X)
3. Search for "SIP Language"
4. Click Install


---

# aui

Created: 2025-04-04T08:28:31Z
Updated: 2025-04-04T08:37:40Z
URL: https://github.com/mahoraga94/aui


## README

# AUI — Artificial User Interface

**AUI** is a sleek, modern, AI-powered chat client built with **PyQt5** and themed using **Gruvbox** for a polished user experience. Designed for local usage with support for the **Gemini API**, it offers a customizable, fluid, and beautiful desktop AI chat environment — right on your system.

![aui-preview](preview.png)

---

## Features

- Beautiful, themed chat UI (Gruvbox, Monokai, Catppuccin, etc.)
- Chat bubbles aligned (left for AI, right for user)
- Chat history with session management
- Settings panel with:
  - Font selection and scaling
  - Theme switching
  - API Key storage
- Keyboard Shortcuts:
  - `Ctrl+Enter` to send message
- Message input area always docked to bottom
- Auto-saving chat sessions
- Info tab for usage guidance
- Built-in motivational and taunting quotes from **Bennett Foddy**
- Smooth animations, UI feedback, and stylized interactions

## Installation

### Requirements

- Python 3.8+
- `PyQt5`
- Gemini API Key (optional for offline use)

Install dependencies:


---

# nasembler

Created: 2025-04-03T12:55:42Z
Updated: 2025-04-04T03:15:47Z
URL: https://github.com/mahoraga94/nasembler


## README

# nasembler - Modern x86-64 Assembler

nasembler is a modern assembler for x86-64 assembly language, designed with a focus on clear error messages, extensive documentation, and developer-friendly features.

## Features

- **Clean and Modern CLI**: Intuitive command-line interface with sensible defaults
- **Enhanced Error Messages**: Detailed and color-coded error messages with clear pointers to the exact issue
- **ELF Output**: Generates executable Linux ELF binaries
- **Debugging Tools**: Integrated tools for inspecting the assembly process
- **Support for x86-64 Instructions**: Comprehensive support for the x86-64 instruction set

## Installation

```bash
# Clone the repository
git clone https://github.com/skubed0007/nasembler.git
cd nasembler

# Build with Cargo
cargo build --release

# Optionally, install it system-wide
cargo install --path .
```

## Quick Start

```bash
# Assemble a file
nasembler hello.asm

# Assemble and run immediately
nasembler hello.asm -x

# Show verbose output
nasembler hello.asm -v
```

## Cross-Platform Building

nasembler is designed to be built for multiple platforms. The repository includes a build script that can generate binaries for various architectures and operating systems.

### Optimized Release Builds

The `Cargo.toml` file includes optimized release profile settings:

```toml
[profile.release]
opt-level = 3                # Maximum optimization
lto = "fat"                  # Link-time optimization
codegen-units = 1            # Better optimization
panic = "abort"              # Removes panic unwinding
strip = true                 # Strip symbols
```

These settings produce smaller, faster binaries at the cost of longer build times.

### Building for Multiple Platforms

The included `build.sh` script allows you to build nasembler for multiple target platforms:

```bash
# Make the script executable
chmod +x build.sh

# Run the build script
./build.sh
```

The script will:

1. Display a list of supported target platforms
2. Ask you to select platforms by number (space-separated)
3. Install any necessary Rust target toolchains
4. Build optimized binaries for each selected platform
5. Place the compiled binaries in the `cross-platform-builds` directory

Supported platforms include:
- Linux (x86_64, ARM, etc.)
- Windows (MSVC and MinGW)
- macOS (Intel and Apple Silicon)
- FreeBSD, NetBSD, OpenBSD
- And many more specialized targets

### Requirements for Cross-Compilation

Some platforms may require additional tools:
- Windows targets require either the MSVC toolchain or MinGW
- macOS targets require an OSX SDK if building from Linux
- Some targets may require specific linkers or system libraries

The script will attempt to install required Rust targets automatically.

## Documentation

Comprehensive documentation is available in the `Infos_MD` directory:

- [Syntax Reference](Infos_MD/SYNTAX_REFERENCE.md) - Detailed guide to nasembler syntax
- [Command-Line Reference](Infos_MD/CLI_REFERENCE.md) - All available command-line options
- [Example Programs](Infos_MD/EXAMPLES.md) - Example assembly programs and patterns
- [Architecture Overview](Infos_MD/ARCHITECTURE.md) - How nasembler works internally
- [Assembly Pipeline](Infos_MD/ASSEMBLER_PIPELINE.md) - Details of the assembly process
- [Debugging Guide](Infos_MD/DEBUGGING_GUIDE.md) - How to debug assembly programs
- [Instruction Reference](Infos_MD/INSTRUCTION_REFERENCE.md) - Guide to supported instructions
- [x86-64 Assembly Guide](Infos_MD/X86_64_ASSEMBLY_GUIDE.md) - General guide to x86-64 assembly

## Example

Here's a simple "Hello, World!" example:

```asm
section .data
    msg db "Hello, World!", 10, 0  ; String with newline and null terminator

section .text
    global _start

_start:
    ; Write the message to stdout
    mov rax, 1          ; syscall number for sys_write
    mov rdi, 1          ; file descriptor 1 (stdout)
    mov rsi, msg        ; pointer to message
    mov rdx, 14         ; message length (including newline)
    syscall

    ; Exit the program
    mov rax, 60         ; syscall number for sys_exit
    xor rdi, rdi        ; exit code 0
    syscall
```

Save this as `hello.asm` and assemble it with:

```bash
nasembler hello.asm -ex
```

## Error Messages

nasembler provides clear and helpful error messages:

```
■ FILE:test_errors.asm

×01 8:9 Bad Operand Invalid first operand for 'jmp' instruction
  │ jmp missing_label
  └→ ^^^~~~~~~~~~~~~

×02 19:0 Dup Label Duplicate label 'duplicate' found
  │ duplicate:
  └→ ^~~~~~~~~

×03 25:4 Unknown Instr Unknown x86-64 instruction 'invalidinstr'
  │ invalidinstr rax, rbx
  └→ ~~~~~~~~~~~~

×04 28:8 Bad Operand Instruction 'mov' requires 2 operands, but found 1
  │ mov rax
  └→ ~~~~~~~

×05 31:8 Bad Operand Invalid first operand for 'mov' instruction
  │ mov xyz, 42
  └→ ~~~~^^^

×06 34:8 Sect Err Invalid section name
  │ section 123invalid
  └→ ~~~~~~~^^^^^^^^^

×07 37:13 Bad Operand Invalid second operand for 'rbx' instruction
  │ mov rax, [rbx+*4]
  └→ ~~~~~~~~~~~~~^~

×08 37:14 Syntax Err Unexpected token '4'
  │ mov rax, [rbx+*4]
  └→ ~~~~~~~~~~~~~~^

×09 41:18 String Error Unclosed string literal
  │ message db "Hello, World 
  └→ ~~~~~~~~~~~~~~~~^~~~

═══════════════════════════════
×9 errs
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the LICENSE file for details.


---

# nixpkgs

Created: 2025-03-20T08:30:04Z
Updated: 2025-03-20T08:34:56Z
URL: https://github.com/mahoraga94/nixpkgs


## README

<p align="center">
  <a href="https://nixos.org">
    <picture>
      <source media="(prefers-color-scheme: light)" srcset="https://raw.githubusercontent.com/NixOS/nixos-homepage/main/public/logo/nixos-hires.png">
      <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/NixOS/nixos-artwork/master/logo/nixos-white.png">
      <img src="https://raw.githubusercontent.com/NixOS/nixos-homepage/main/public/logo/nixos-hires.png" width="500px" alt="NixOS logo">
    </picture>
  </a>
</p>

<p align="center">
  <a href="CONTRIBUTING.md"><img src="https://img.shields.io/github/contributors-anon/NixOS/nixpkgs" alt="Contributors badge" /></a>
  <a href="https://opencollective.com/nixos"><img src="https://opencollective.com/nixos/tiers/supporter/badge.svg?label=supporters&color=brightgreen" alt="Open Collective supporters" /></a>
</p>

[Nixpkgs](https://github.com/nixos/nixpkgs) is a collection of over
100,000 software packages that can be installed with the
[Nix](https://nixos.org/nix/) package manager. It also implements
[NixOS](https://nixos.org/nixos/), a purely-functional Linux distribution.

# Manuals

* [NixOS Manual](https://nixos.org/nixos/manual) - how to install, configure, and maintain a purely-functional Linux distribution
* [Nixpkgs Manual](https://nixos.org/nixpkgs/manual/) - contributing to Nixpkgs and using programming-language-specific Nix expressions
* [Nix Package Manager Manual](https://nixos.org/nix/manual) - how to write Nix expressions (programs), and how to use Nix command line tools

# Community

* [Discourse Forum](https://discourse.nixos.org/)
* [Matrix Chat](https://matrix.to/#/#community:nixos.org)
* [NixOS Weekly](https://weekly.nixos.org/)
* [Official wiki](https://wiki.nixos.org/)
* [Community-maintained list of ways to get in touch](https://wiki.nixos.org/wiki/Get_In_Touch#Chat) (Discord, Telegram, IRC, etc.)

# Other Project Repositories

The sources of all official Nix-related projects are in the [NixOS
organization on GitHub](https://github.com/NixOS/). Here are some of
the main ones:

* [Nix](https://github.com/NixOS/nix) - the purely functional package manager
* [NixOps](https://github.com/NixOS/nixops) - the tool to remotely deploy NixOS machines
* [nixos-hardware](https://github.com/NixOS/nixos-hardware) - NixOS profiles to optimize settings for different hardware
* [Nix RFCs](https://github.com/NixOS/rfcs) - the formal process for making substantial changes to the community
* [NixOS homepage](https://github.com/NixOS/nixos-homepage) - the [NixOS.org](https://nixos.org) website
* [hydra](https://github.com/NixOS/hydra) - our continuous integration system
* [NixOS Artwork](https://github.com/NixOS/nixos-artwork) - NixOS artwork

# Continuous Integration and Distribution

Nixpkgs and NixOS are built and tested by our continuous integration
system, [Hydra](https://hydra.nixos.org/).

* [Continuous package builds for unstable/master](https://hydra.nixos.org/jobset/nixos/trunk-combined)
* [Continuous package builds for the NixOS 24.11 release](https://hydra.nixos.org/jobset/nixos/release-24.11)
* [Tests for unstable/master](https://hydra.nixos.org/job/nixos/trunk-combined/tested#tabs-constituents)
* [Tests for the NixOS 24.11 release](https://hydra.nixos.org/job/nixos/release-24.11/tested#tabs-constituents)

Artifacts successfully built with Hydra are published to cache at
https://cache.nixos.org/. When successful build and test criteria are
met, the Nixpkgs expressions are distributed via [Nix
channels](https://nix.dev/manual/nix/stable/command-ref/nix-channel.html).

# Contributing

Nixpkgs is among the most active projects on GitHub. While thousands
of open issues and pull requests might seem a lot at first, it helps
consider it in the context of the scope of the project. Nixpkgs
describes how to build tens of thousands of pieces of software and implements a
Linux distribution. The [GitHub Insights](https://github.com/NixOS/nixpkgs/pulse)
page gives a sense of the project activity.

Community contributions are always welcome through GitHub Issues and
Pull Requests.

For more information about contributing to the project, please visit
the [contributing page](CONTRIBUTING.md).

# Donations

The infrastructure for NixOS and related projects is maintained by a
nonprofit organization, the [NixOS
Foundation](https://nixos.org/nixos/foundation.html). To ensure the
continuity and expansion of the NixOS infrastructure, we are looking
for donations to our organization.

You can donate to the NixOS foundation through [SEPA bank
transfers](https://nixos.org/donate.html) or by using Open Collective:

<a href="https://opencollective.com/nixos#support"><img src="https://opencollective.com/nixos/tiers/supporter.svg?width=890" /></a>

# License

Nixpkgs is licensed under the [MIT License](COPYING).

Note: MIT license does not apply to the packages built by Nixpkgs,
merely to the files in this repository (the Nix expressions, build
scripts, NixOS modules, etc.). It also might not apply to patches
included in Nixpkgs, which may be derivative works of the packages to
which they apply. The aforementioned artifacts are all covered by the
licenses of the respective packages.


---

# halsh

Created: 2024-12-28T12:01:57Z
Updated: 2025-03-20T08:15:46Z
URL: https://github.com/mahoraga94/halsh


## README

# Halsh

Halsh is a **custom shell** written in C. Its code base is extremely **simple** and **lightweight**. 

Halsh isn't a fancy shell—it's just... a **shell**. There's no "AI" nonsense, no unnecessary complexity—just a **plain minimal shell** with a few essential features:

- **Aliases**  
- **Command history**  
- **Job control**  
- **Piping and output redirection**

Halsh is designed for users who appreciate a simple, no-frills environment. Whether you need to run basic commands, manage processes, or customize your shell prompt, Halsh gets the job done without any unnecessary distractions. 

It's **efficient**, **functional**, and **minimalistic**—exactly what you need if you want a simple shell experience without the bloat.

# Requirements
You need ncurses and readline libraries installed on your system

# License
this project is licensed under the [Joay License](LICENSE.md)

---

# world_simulator

Created: 2025-03-17T15:14:17Z
Updated: 2025-03-17T15:26:16Z
URL: https://github.com/mahoraga94/world_simulator


## README

# world_simulator
a world building tool designed to be fairly generic using a dedicated DSL

this repo is still in the very early stages of devlopment

# syntax

classes and inhertance

```
class location;
class economic_entity {
	float wealth;
	float food;
}
class city extends location,economic_entity {
	string name;
}

class merchant extends economic_entity{
	string name;
}

class road{
	source city;
	target city;
}
```

rendering (expiremental)
```

render {
	graph_render(nodes=city,edge_start = road.target,edge_end = road.end)
}

```

# technical decsions
it makes a lot of sense to use some sort of a qurying engine for runing the underlying representation.
since that gives us parllalisem for free and all sorts of other niceties.

unfortunatly they all take way too long to compile... so what we probably want with this is a setup where there is a subcrate that compiles a base data structure for us to use then the rest of the code just refrences it...

---

# surrealdb

Created: 2025-03-05T13:34:53Z
Updated: 2025-03-07T07:57:31Z
URL: https://github.com/mahoraga94/surrealdb


## README

<a href="https://surrealdb.com#gh-dark-mode-only" target="_blank">
    <img width="100%" src="/img/white/hero.png" alt="SurrealDB Hero">
</a>
<a href="https://surrealdb.com#gh-light-mode-only" target="_blank">
    <img width="100%" src="/img/black/hero.png" alt="SurrealDB Hero">
</a>

<p align="center">
    <a href="https://github.com/surrealdb/surrealdb"><img src="https://img.shields.io/github/v/release/surrealdb/surrealdb?color=ff00a0&include_prereleases&label=version&sort=semver&style=flat-square"></a>
    &nbsp;
    <a href="https://github.com/surrealdb/surrealdb"><img src="https://img.shields.io/badge/built_with-Rust-dca282.svg?style=flat-square"></a>
    &nbsp;
	<a href="https://github.com/surrealdb/surrealdb/actions"><img src="https://img.shields.io/github/actions/workflow/status/surrealdb/surrealdb/nightly.yml?style=flat-square&branch=main"></a>
    &nbsp;
    <a href="https://github.com/surrealdb/license"><img src="https://img.shields.io/badge/license-BSL_1.1-00bfff.svg?style=flat-square"></a>
</p>

<p align="center">
    <a href="https://hub.docker.com/repository/docker/surrealdb/surrealdb"><img src="https://img.shields.io/docker/pulls/surrealdb/surrealdb?label=docker%20pulls&style=flat-square"></a>
    &nbsp;
    <a href="https://crates.io/crates/surrealdb"><img src="https://img.shields.io/crates/d/surrealdb?color=dca282&label=rust&style=flat-square"></a>
	&nbsp;
    <a href="https://www.npmjs.com/package/surrealdb.js"><img src="https://img.shields.io/npm/dt/surrealdb.js?color=f7df1e&label=javascript&style=flat-square"></a>
    &nbsp;
	<a href="https://pypi.org/project/surrealdb/"><img src="https://img.shields.io/pepy/dt/surrealdb?color=426c99&label=python&style=flat-square"></a>
	&nbsp;
	<a href="https://www.nuget.org/packages/SurrealDb.Net"><img src="https://img.shields.io/nuget/dt/surrealdb.net?color=4c2dcc&label=.NET&style=flat-square"></a>
	&nbsp;
	<a href="https://packagist.org/packages/surrealdb/surrealdb.php"><img src="https://img.shields.io/packagist/dt/surrealdb/surrealdb.php?color=4d588b&label=php&style=flat-square"></a>
    &nbsp;
	<a href="https://hub.docker.com/repository/docker/surrealdb/surrealdb"><img src="https://img.shields.io/github/downloads/surrealdb/surrealdb/total?color=8259dd&label=github%20downloads&style=flat-square"></a>
</p>

<p align="center">
	<a href="https://surrealdb.com/discord"><img src="https://img.shields.io/discord/902568124350599239?label=discord&style=flat-square&color=5a66f6" alt="Discord"></a>
	&nbsp;
    <a href="https://x.com/surrealdb"><img src="https://img.shields.io/badge/x-follow_us-222222.svg?style=flat-square" alt="X"></a>
    &nbsp;
    <a href="https://dev.to/surrealdb"><img src="https://img.shields.io/badge/dev-join_us-86f7b7.svg?style=flat-square" alt="Dev"></a>
    &nbsp;
    <a href="https://www.linkedin.com/company/surrealdb/"><img src="https://img.shields.io/badge/linkedin-connect_with_us-0a66c2.svg?style=flat-square" alt="LinkedIn"></a>
	&nbsp;
    <a href="https://www.youtube.com/@surrealdb"><img src="https://img.shields.io/badge/youtube-subscribe-fc1c1c.svg?style=flat-square" alt="YouTube"></a>
</p>

<p align="center">
	<a href="https://surrealdb.com/blog"><img height="25" src="./img/social/blog.svg" alt="Blog"></a>
	&nbsp;
	<a href="https://github.com/surrealdb/surrealdb"><img height="25" src="./img/social/github.svg" alt="Github"></a>
	&nbsp;
    <a href="https://www.linkedin.com/company/surrealdb/"><img height="25" src="./img/social/linkedin.svg" alt="LinkedIn"></a>
    &nbsp;
    <a href="https://x.com/surrealdb"><img height="25" src="./img/social/x.svg" alt="X"></a>
    &nbsp;
    <a href="https://www.youtube.com/@surrealdb"><img height="25" src="./img/social/youtube.svg" alt="YouTube"></a>
    &nbsp;
    <a href="https://dev.to/surrealdb"><img height="25" src="./img/social/dev.svg" alt="Dev"></a>
    &nbsp;
    <a href="https://surrealdb.com/discord"><img height="25" src="./img/social/discord.svg" alt="Discord"></a>
    &nbsp;
    <a href="https://stackoverflow.com/questions/tagged/surrealdb"><img height="25" src="./img/social/stack-overflow.svg" alt="Stack Overflow"></a>
</p>

<br>

<h2><img height="20" src="./img/whatissurreal.svg">&nbsp;&nbsp;What is SurrealDB?</h2>

SurrealDB is an end-to-end cloud-native database designed for modern applications, including web, mobile, serverless, Jamstack, backend, and traditional applications. With SurrealDB, you can simplify your database and API infrastructure, reduce development time, and build secure, performant apps quickly and cost-effectively.

**Key features of SurrealDB include:**

- **Reduces development time**: SurrealDB simplifies your database and API stack by removing the need for most server-side components, allowing you to build secure, performant apps faster and cheaper.
- **Real-time collaborative API backend service:** SurrealDB functions as both a database and an API backend service, enabling real-time collaboration.
- **Support for multiple querying languages:** SurrealDB supports SQL querying from client devices, GraphQL, ACID transactions, WebSocket connections, structured and unstructured data, graph querying, full-text indexing, and geospatial querying.
- **Granular access control**: SurrealDB provides row-level permissions-based access control, giving you the ability to manage data access with precision.

View the [features](https://surrealdb.com/features), the latest [releases](https://surrealdb.com/releases), and [documentation](https://surrealdb.com/docs).

<img width="100%" src="./img/interface.png" alt="Surrealist">

<h2><img height="20" src="./img/contents.svg">&nbsp;&nbsp;Contents</h2>

- [Features](#features)
- [Documentation](#documentation)
- [Getting started](#getting-started)
	- [Server side code](#server-side-code)
	- [Client side apps](#client-side-apps)
- [Surreal Cloud](#surreal-cloud)
- [Installation](#installation)
	- [Install on macOS](#install-on-macos)
	- [Install on Linux](#install-on-linux)
	- [Install on Windows](#install-on-windows)
	- [Run using Docker](#run-using-docker)
- [Quick look](#quick-look)
- [Why SurrealDB](#why-surrealdb)
	- [Database, API, and permissions](#database-api-and-permissions)
	- [Tables, documents, and graph](#tables-documents-and-graph)
	- [Advanced inter-document relations](#advanced-inter-document-relations-and-analysis-no-joins-no-pain)
	- [Simple schema definition](#simple-schema-definition-for-frontend-and-backend-development)
	- [Connect directly from web-browsers](#connect-and-query-directly-from-web-browsers-and-client-devices)
	- [Multiple different query methods](#query-the-database-with-the-tools-you-want)
	- [Realtime live queries and data changes](#realtime-live-queries-and-data-changes-direct-to-application)
	- [Scale effortlessly for high-availability](#scale-effortlessly-to-hundreds-of-nodes-for-high-availability-and-scalability)
	- [Extend your database with JavaScript](#extend-your-database-with-javascript-functions)
	- [Designed to be embedded or in the cloud](#designed-to-be-embedded-or-to-run-distributed-in-the-cloud)
- [Community](#community)
- [Contributing](#contributing)
- [Security](#security)
- [License](#license)

<h2><img height="20" src="./img/features.svg">&nbsp;&nbsp;Features</h2>

- [x] Database server, or embedded library
- [x] Multi-row, multi-table ACID transactions
- [x] Single-node, or highly-scalable distributed mode
- [x] Record links and directed typed graph connections
- [x] Store structured and unstructured data
- [x] Incrementally computed views for pre-computed advanced analytics
- [x] Realtime-api layer, and security permissions built in
- [x] Store and model data in any way with tables, documents, and graph
- [x] Simple schema definition for frontend and backend development
- [x] Connect and query directly from web-browsers and client devices
- [x] Use embedded JavaScript functions for custom advanced functionality

<h2><img height="20" src="./img/documentation.svg">&nbsp;&nbsp;Documentation</h2>

For guidance on installation, development, deployment, and administration, take a look at the following resources:

- Documentation: https://surrealdb.com/docs
- SurrealDB University: https://surrealdb.com/learn/fundamentals
- Aeon's Surreal Renaissance (interative book): https://surrealdb.com/learn/book

<h2><img height="20" src="./img/gettingstarted.svg">&nbsp;&nbsp;Getting started</h2>

Getting started with SurrealDB is as easy as starting up the SurrealDB database server, choosing your platform, and integrating its SDK into your code. You can easily get started with your platform of choice by reading one of our tutorials.

**Server side code**

<p>
    <a href="https://surrealdb.com/docs/integration/sdks/rust"><img width=60 title="Rust" src="https://raw.githubusercontent.com/surrealdb/icons/main/rust.svg" /></a>
    &nbsp;
    <a href="https://surrealdb.com/docs/integration/sdks/javascript"><img width=60 title="JavaScript" src="https://raw.githubusercontent.com/surrealdb/icons/main/javascript.svg" /></a>
	&nbsp;
    <a href="https://surrealdb.com/docs/integration/sdks/javascript"><img width=60 title="WebAssembly" src="https://raw.githubusercontent.com/surrealdb/icons/main/webassembly.svg" /></a>
	&nbsp;
    <a href="https://surrealdb.com/docs/integration/sdks/javascript"><img width=60 title="Node.js" src="https://raw.githubusercontent.com/surrealdb/icons/main/nodejs.svg" /></a>
	&nbsp;
    <a href="https://surrealdb.com/docs/integration/sdks/javascript"><img width=60 title="Deno" src="https://raw.githubusercontent.com/surrealdb/icons/main/deno.svg" /></a>
	&nbsp;
	<a href="https://surrealdb.com/docs/integration/sdks/python"><img width=60 title="Python" src="https://raw.githubusercontent.com/surrealdb/icons/main/python.svg" /></a>
	&nbsp;
    <a href="https://surrealdb.com/docs/integration/sdks/golang"><img width=60 title="Golang" src="https://raw.githubusercontent.com/surrealdb/icons/main/golang.svg" /></a>
	&nbsp;
    <a href="https://surrealdb.com/docs/integration/sdks/dotnet"><img width=60 title=".NET" src="https://raw.githubusercontent.com/surrealdb/icons/main/dotnet.svg" /></a>
	&nbsp;
    <a href="https://surrealdb.com/docs/integration/sdks/php"><img width=60 title="PHP" src="https://raw.githubusercontent.com/surrealdb/icons/main/php.svg" /></a>
	&nbsp;
    <a href="https://surrealdb.com/docs/integration/sdks/java"><img width=60 title="Java" src="https://raw.githubusercontent.com/surrealdb/icons/main/java.svg" /></a>
</p>

**Client side apps**

<p>
	<a href="https://surrealdb.com/docs/integration/sdks/javascript"><img width=60 title="JavaScript" src="https://raw.githubusercontent.com/surrealdb/icons/main/javascript.svg" /></a>
    &nbsp;
    <a href="https://surrealdb.com/docs/integration/sdks/javascript"><img width=60 title="WebAssembly" src="https://raw.githubusercontent.com/surrealdb/icons/main/webassembly.svg" /></a>
	&nbsp;
    <a href="https://surrealdb.com/docs/integration/sdks/javascript"><img width=60 title="React" src="https://raw.githubusercontent.com/surrealdb/icons/main/reactjs.svg" /></a>
	&nbsp;
    <a href="https://surrealdb.com/docs/integration/sdks/javascript"><img width=60 title="Next.js" src="https://raw.githubusercontent.com/surrealdb/icons/main/nextjs.svg" /></a>
	&nbsp;
    <a href="https://surrealdb.com/docs/integration/sdks/ember"><img width=60 title="Ember.js" src="https://raw.githubusercontent.com/surrealdb/icons/main/emberjs.svg" /></a>
</p>

<h2><img height="20" src="/img/cloud.svg?raw=true">&nbsp;&nbsp;Surreal Cloud</h2>

<a href="https://surrealdb.com/cloud#gh-dark-mode-only" target="_blank">
    <img width="100%" src="/img/white/cloud.png" alt="Surreal Cloud">
</a>
<a href="https://surrealdb.com/cloud#gh-light-mode-only" target="_blank">
    <img width="100%" src="/img/black/cloud.png" alt="Surreal Cloud">
</a>

SurrealDB is available as a [hosted platform](https://surrealist.app/cloud). Forget about infrastructure operations, monitoring, backups or capacity planning. [Surreal Cloud](https://surrealdb.com/cloud) allows you to focus on building great products using the power and flexibility of SurrealDB in just a few clicks. Grow from prototype to enterprise-scale. The Surreal Cloud scalable architecture allows your database to evolve as your application grows, ensuring you are always ahead of demand. However if you want to deploy SurrealDB yourself, keep reading below.

<h2><img height="20" src="./img/installation.svg">&nbsp;&nbsp;Installation</h2>

SurrealDB is designed to be simple to install and simple to run - using just one command from your terminal. In addition to traditional installation, SurrealDB can be installed and run with HomeBrew, Docker, or using any other container orchestration tool such as Docker Compose, Docker Swarm, Rancher, or in Kubernetes.

<h4><a href="https://surrealdb.com/install"><img width="20" src="./img/apple.svg"></a>&nbsp;Install on macOS</h4>

The quickest way to get going with SurrealDB on macOS is to use Homebrew. This will install both the command-line tools, and the SurrealDB server as a single executable. If you don't use Homebrew, follow the instructions for Linux below to install SurrealDB.

```bash
brew install surrealdb/tap/surreal
```

If you want to test a version with the latest features, published every night, install the `nightly` version:

```bash
brew install surrealdb/tap/surreal-nightly
```

<h4><a href="https://surrealdb.com/install"><img width="20" src="./img/linux.svg"></a>&nbsp;Install on Linux</h4>

The easiest and preferred way to get going with SurrealDB on Unix operating systems is to install and use the SurrealDB command-line tool. Run the following command in your terminal and follow the on-screen instructions.

```bash
curl --proto '=https' --tlsv1.2 -sSf https://install.surrealdb.com | sh
```

If you want to run a beta release, before the next version is released, the `beta` version:

```bash
curl --proto '=https' --tlsv1.2 -sSf https://install.surrealdb.com | sh -s -- --beta
```

If you want to test a version with the latest features, published every night, install the `nightly` version:

```bash
curl --proto '=https' --tlsv1.2 -sSf https://install.surrealdb.com | sh -s -- --nightly
```

<h4><a href="https://surrealdb.com/install"><img width="20" src="./img/windows.svg"></a>&nbsp;Install on Windows</h4>

The easiest and preferred way to get going with SurrealDB on Windows is to install and use the SurrealDB command-line tool. Run the following command in your terminal and follow the on-screen instructions.

```ps1
iwr https://windows.surrealdb.com -useb | iex
```

If you want to test a version with the latest features, published every night, install the `nightly` version:

```ps1
iex "& { $(irm https://windows.surrealdb.com) } -Nightly"
```

<h4><a href="https://surrealdb.com/install"><img width="20" src="./img/docker.svg"></a>&nbsp;Run using Docker</h4>

Docker can be used to manage and run SurrealDB database instances without the need to install any command-line tools. The SurrealDB docker container contains the full command-line tools for importing and exporting data from a running server, or for running a server itself.

```bash
docker run --rm --pull always --name surrealdb -p 8000:8000 surrealdb/surrealdb:latest start
```

For just getting started with a development server running in memory, you can pass the container a basic initialization to set the user and password as root and enable logging.

```bash
docker run --rm --pull always --name surrealdb -p 8000:8000 surrealdb/surrealdb:latest start --log info --user root --pass root memory
``` 

<h2><img height="20" src="./img/features.svg">&nbsp;&nbsp;Quick look</h2>

With strongly-typed data types, data can be fully modelled right in the database.

```sql
UPDATE person SET
    waist = <int> "34",
    height = <float> 201,
    score = <decimal> 0.3 + 0.3 + 0.3 + 0.1
;
```

Store dynamically computed fields which are calculated when retrieved.

```sql
CREATE person SET
    birthday = <datetime> "2007-06-22",
    can_drive = <future> { time::now() > birthday + 18y }
;
```

Easily work with unstructured or structured data, in schema-less or schema-full mode.

```sql
-- Create a schemafull table
DEFINE TABLE user SCHEMAFULL;

-- Specify fields on the user table
DEFINE FIELD name ON TABLE user TYPE object;
DEFINE FIELD name.first ON TABLE user TYPE string;
DEFINE FIELD name.last ON TABLE user TYPE string;
DEFINE FIELD email ON TABLE user TYPE string ASSERT string::is::email($value);

-- Add a unique index on the email field preventing duplicate values
DEFINE INDEX email ON TABLE user COLUMNS email UNIQUE;

-- Create a new event whenever a user changes their email address
DEFINE EVENT email ON TABLE user WHEN $before.email != $after.email THEN (
    CREATE event SET user = $value, time = time::now(), value = $after.email, action = 'email_changed'
);
```

Connect records together with fully directed graph edge connections.

```sql
-- Add a graph edge between user:tobie and article:surreal
RELATE user:tobie->write->article:surreal
    SET time.written = time::now()
;

-- Add a graph edge between specific users and developers
LET $from = (SELECT users FROM company:surrealdb);
LET $devs = (SELECT * FROM user WHERE tags CONTAINS 'developer');
RELATE $from->like->$devs UNIQUE
    SET time.connected = time::now()
;
```

Query data flexibly with advanced expressions and graph queries.

```sql
-- Select a nested array, and filter based on an attribute
SELECT emails[WHERE active = true] FROM person;

-- Select all 1st, 2nd, and 3rd level people who this specific person record knows, or likes, as separate outputs
SELECT ->knows->(? AS f1)->knows->(? AS f2)->(knows, likes AS e3 WHERE influencer = true)->(? AS f3) FROM person:tobie;

-- Select all person records (and their recipients), who have sent more than 5 emails
SELECT *, ->sent->email->to->person FROM person WHERE count(->sent->email) > 5;

-- Select other products purchased by people who purchased this laptop
SELECT <-purchased<-person->purchased->product FROM product:laptop;

-- Select products purchased by people in the last 3 weeks who have purchased the same products that we purchased
SELECT ->purchased->product<-purchased<-person->(purchased WHERE created_at > time::now() - 3w)->product FROM person:tobie;
```

Store GeoJSON geographical data types, including points, lines and polygons.

```sql
UPDATE city:london SET
    centre = (-0.118092, 51.509865),
    boundary = {
        type: "Polygon",
        coordinates: [[
            [-0.38314819, 51.37692386], [0.1785278, 51.37692386],
            [0.1785278, 51.61460570], [-0.38314819, 51.61460570],
            [-0.38314819, 51.37692386]
        ]]
    }
;
```

Write custom embedded logic using JavaScript functions.

```sql
CREATE film SET
    ratings = [
        { rating: 6, user: user:bt8e39uh1ouhfm8ko8s0 },
        { rating: 8, user: user:bsilfhu88j04rgs0ga70 },
    ],
    featured = function() {
        return this.ratings.filter(r => {
            return r.rating >= 7;
        }).map(r => {
            return { ...r, rating: r.rating * 10 };
        });
    }
;
```

Specify granular access permissions for client and application access.

```sql
-- Specify access permissions for the 'post' table
DEFINE TABLE post SCHEMALESS
    PERMISSIONS
        FOR select
            -- Published posts can be selected
            WHERE published = true
            -- A user can select all their own posts
            OR user = $auth.id
        FOR create, update
            -- A user can create or update their own posts
            WHERE user = $auth.id
        FOR delete
            -- A user can delete their own posts
            WHERE user = $auth.id
            -- Or an admin can delete any posts
            OR $auth.admin = true
;
```

<h2><img height="20" src="./img/features.svg">&nbsp;&nbsp;Why SurrealDB?</h2>

<p>
	<img width="49%" src="./img/illustrations/database-api-security.jpg" />
	<img width="49%" src="./img/illustrations/multimodel-database.jpg" />
</p>

### Database, API, and permissions

SurrealDB combines the database layer, the querying layer, and the API and authentication layer into one platform. Advanced table-based and row-based customisable access permissions allow for granular data access patterns for different types of users. There's no need for custom backend code and security rules with complicated database development.

### Tables, documents, and graph

As a multi-model database, SurrealDB enables developers to use multiple techniques to store and model data, without having to choose a method in advance. With the use of tables, SurrealDB has similarities with relational databases, but with the added functionality and flexibility of advanced nested fields and arrays. Inter-document record links allow for simple to understand and highly-performant related queries without the use of JOINs, eliminating the N+1 query problem.

<p>
	<img width="49%" src="./img/illustrations/graph-database.jpg" />
	<img width="49%" src="./img/illustrations/schema-direct-to-client.jpg" />
</p>

### Advanced inter-document relations and analysis. No JOINs. No pain.

With full graph database functionality SurrealDB enables more advanced querying and analysis. Records (or vertices) can be connected to one another with edges, each with its own record properties and metadata. Simple extensions to traditional SQL queries allow for multi-table, multi-depth document retrieval, efficiently in the database, without the use of complicated JOINs and without bringing the data down to the client.

### Simple schema definition for frontend and backend development

With SurrealDB, specify your database and API schema in one place, and define column rules and constraints just once. Once a schema is defined, database access is automatically granted to the relevant users. No more custom API code, and no more GraphQL integration. Simple, flexible, and ready for production in minutes not months.

<p>
	<img width="49%" src="./img/illustrations/browser-sync.jpg" />
	<img width="49%" src="./img/illustrations/multiple-integrations.jpg" />
</p>

### Connect and query directly from web-browsers and client devices

Connect directly to SurrealDB from any end-user client device. Run SurrealQL queries directly within web-browsers, ensuring that users can only view or modify the data that they are allowed to access. Highly-performant WebSocket connections allow for efficient bi-directional queries, responses and notifications.

### Query the database with the tools you want

Your data, your choice. SurrealDB is designed to be flexible to use, with support for SurrealQL, GraphQL (coming soon), CRUD support over REST, and JSON-RPC querying and modification over WebSockets. With direct-to-client connection with in-built permissions, SurrealDB speeds up the development process, and fits in seamlessly into any tech stack.

<p>
	<img width="49%" src="./img/illustrations/realtime-live-queries.jpg" />
	<img width="49%" src="./img/illustrations/scale-up.jpg" />
</p>

### Realtime live queries and data changes direct to application

SurrealDB keeps every client device in-sync with data modifications pushed in realtime to the clients, applications, end-user devices, and server-side libraries. Live SQL queries allow for advanced filtering of the changes to which a client subscribes, and efficient data formats, including DIFFing and PATCHing enable highly-performant web-based data syncing.

### Scale effortlessly to hundreds of nodes for high-availability and scalability

SurrealDB can be run as a single in-memory node, or as part of a distributed cluster - offering highly-available and highly-scalable system characteristics. Designed from the ground up to run in a distributed environment, SurrealDB makes use of special techniques when handling multi-table transactions, and document record IDs - with no use of table or row locks.

<p>
	<img width="49%" src="./img/illustrations/database-plugins.jpg" />
	<img width="49%" src="./img/illustrations/cloud-or-embedded.jpg" />
</p>

### Extend your database with JavaScript functions

Embedded JavaScript functions allow for advanced, custom functionality, with computation logic being moved to the data layer. This improves upon the traditional approach of moving data to the client devices before applying any computation logic, ensuring that only the necessary data is transferred remotely. These advanced JavaScript functions, with support for the ES2020 standard, allow any developer to analyse the data in ever more simple-yet-advanced ways.

### Designed to be embedded or to run distributed in the cloud

Built entirely in Rust as a single library, SurrealDB is designed to be used as both an embedded database library with advanced querying functionality, and as a database server which can operate in a distributed cluster. With low memory usage and cpu requirements, the system requirements have been specifically thought through for running in all types of environment.

<h2><img height="20" src="./img/community.svg">&nbsp;&nbsp;Community</h2>

Join our growing community around the world, for help, ideas, and discussions regarding SurrealDB.

- View our official [Blog](https://surrealdb.com/blog)
- Chat live with us on [Discord](https://surrealdb.com/discord)
- Follow us on [X](https://x.com/surrealdb)
- Connect with us on [LinkedIn](https://www.linkedin.com/company/surrealdb/)
- Visit us on [YouTube](https://www.youtube.com/@surrealdb)
- Join our [Dev community](https://dev.to/surrealdb)
- Questions tagged #surrealdb on [Stack Overflow](https://stackoverflow.com/questions/tagged/surrealdb)

<h2><img height="20" src="./img/contributing.svg">&nbsp;&nbsp;Contributing</h2>

We would &nbsp;<img width="15" src="./img/love.svg">&nbsp; for you to get involved with SurrealDB development! If you wish to help, you can learn more about how you can contribute to this project in the [contribution guide](CONTRIBUTING.md).

<h2><img height="20" src="./img/security.svg">&nbsp;&nbsp;Security</h2>

For security issues, view our [vulnerability policy](https://github.com/surrealdb/surrealdb/security/policy), view our [security policy](https://surrealdb.com/legal/security), and kindly email us at [security@surrealdb.com](mailto:security@surrealdb.com) instead of posting a public issue on GitHub.

<h2><img height="20" src="./img/license.svg">&nbsp;&nbsp;License</h2>

Source code for SurrealDB is variously licensed under a number of different licenses. A copy of each license can be found in [each repository](https://github.com/surrealdb).

- Libraries and SDKs, each located in its own distinct repository, are released under either the [Apache License 2.0](https://github.com/surrealdb/license/blob/main/APL.txt) or [MIT License](https://github.com/surrealdb/license/blob/main/MIT.txt).
- Certain core database components, each located in its own distinct repository, are released under the [Apache License 2.0](https://github.com/surrealdb/license/blob/main/APL.txt).
- Core database code for SurrealDB, located in [this repository](https://github.com/surrealdb/surrealdb), is released under the [Business Source License 1.1](/LICENSE).

For more information, see the [licensing information](https://github.com/surrealdb/license).


---

# xasm-rs

Created: 2025-02-23T07:47:41Z
Updated: 2026-01-16T14:29:23Z
URL: https://github.com/mahoraga94/xasm-rs


## README

# Xasm : assembly code generator for rust
> NOTE: Xasm only support linux 64/32 bit though we do have plans for windows and mac in future
---
# Installation
add the crate to your ``Cargo.toml`` file by running the following command:
```
cargo add xasm-rs
```
---
# Using Xasm
first create a mutable xasm class in folllowing way:
```rust
let mut xasm = init::Xasm::new();
```
and now start adding tokens,vars,functions to xasm
here is a sample stuff you can do
```rust
let mut xasm = Xasm::new();
    let rax = xasm.get_reg(init::Register::RAX, true);
    let rdi = xasm.get_reg(init::Register::RDI, true);
    let rsi = xasm.get_reg(init::Register::RSI, true);
    let rdx = xasm.get_reg(init::Register::RDX, true);

    xasm.emit(Instruction::MovImm {
        dst: rax,
        imm: 1,
    });
    xasm.emit(Instruction::MovImm {
        dst: rdi,
        imm: 1,
    });
    xasm.add_variable(Variables::Str("hello world"), "v1");
    xasm.emit(Instruction::MovVar {
        reg: rsi,
        var_name: "v1",
    });
    xasm.emit(Instruction::MovImm {
        dst: rdx,
        imm: 12,
    });
    xasm.emit(Instruction::SYSCALL);
    for inst in xasm.dump(){
        println!("{}",inst);
    }
    println!("xasm:\n{:#?}", xasm);
```
---
Check out the full documentation for xasm to know more on how to use it!

---

A big bear hug to ([neva](https://github.com/nevakrien)) for being an absolute star and guiding light in the journey of creating xasm!


---

# ryx

Created: 2025-02-24T14:58:36Z
Updated: 2025-02-25T04:29:30Z
URL: https://github.com/mahoraga94/ryx


## README

# Ryx
<img src="logo.png" style="width:10em;height:10em"></img>
### A low-level programming language designed to produce ultra-minimal, high-performance binaries

Ryx is a programming language that operates just above the assembly level, providing developers with fine-grained control over system resources while retaining modern language features. Its primary objective is to generate the smallest possible binaries that run at maximum speed—ideal for performance-critical applications, embedded systems, and other scenarios where efficiency is paramount.

Key Characteristics:
- **Ultra-Minimal Binaries:** Ryx is engineered to produce binaries with minimal overhead, ensuring that every byte is optimized for performance.
- **High Performance:** By working close to the hardware, Ryx enables you to harness the full potential of your system's capabilities.
- **Low-Level Control:** Gain direct access to low-level operations and resources, similar to assembly programming, but with a more readable and maintainable syntax.
- **Modern Conveniences:** While it offers the precision of low-level programming, Ryx also includes features that streamline development and enhance code clarity.

Whether you're developing for highly constrained environments or pushing the boundaries of software performance, Ryx provides the tools and flexibility to achieve your goals.

---
> ## Ryx is under the Joay License Version 1.5 that can be found [here](LICENSE.md)

---

# Joay-License

Created: 2024-11-17T00:44:47Z
Updated: 2025-02-25T03:00:13Z
URL: https://github.com/mahoraga94/Joay-License


## No README


---

# gob

Created: 2025-02-19T08:32:28Z
Updated: 2025-02-20T15:06:21Z
URL: https://github.com/mahoraga94/gob


## README

# Gob: The Universal Package Manager for All Linux Distributions

### ***Welcome to Gob, the gobbling package manager designed to unify and simplify package management across all Linux distributions!***
---
**Gob** isn't your run-of-the-mill Linux package manager. It's a native Linux package manager that fetches binaries directly from the source and installs them for you. No extra runtime bloat, no unnecessary dependencies—just pure, unadulterated efficiency. It just works!

---
install gob with the following command:
```bash
curl -sSfL https://raw.githubusercontent.com/skubed0007/gob/main/install.sh | sudo sh
```
after installing gob please run the following command as your first command!
```bash
gob update
```
---
gob commands are as following:

- ```gob search <package_names_sep_by_space>```  : searches for packages having the match

- ``gob about <package_names_sep_by_space>`` : get information about package(s)

- ``gob install <package_names_sep_by_space>"`` : install the package 

---
## Upcoming commands
- ``gob local <path_to_package>`` : install a package from a zip file / binary file
- ``gob remove <package(s) name>``: remove/uninstall packages

---

# ndraey_downloader

Created: 2025-02-20T12:48:58Z
Updated: 2025-02-20T13:24:04Z
URL: https://github.com/mahoraga94/ndraey_downloader


## README

![Crates.io](https://img.shields.io/crates/d/ndraey-downloader)

<h1 align="center"> ndraey_downloader </h1>
This is a simple Rust library for downloading large files with progress bar


---

# gobins

Created: 2025-02-19T06:10:07Z
Updated: 2025-02-20T08:40:16Z
URL: https://github.com/mahoraga94/gobins


## No README


---

# Fyrox

Created: 2025-02-15T15:14:43Z
Updated: 2025-02-15T15:14:43Z
URL: https://github.com/mahoraga94/Fyrox


## README

<div align="center">
  <a href="https://fyrox.rs/">
    <img src="pics/logo.png" width="128" height="128" alt="Fyrox" />
  </a>
  <h1>Fyrox - a modern Rust game engine</h1>
</div>

[![License (MIT)](https://img.shields.io/crates/l/fyrox)](https://github.com/FyroxEngine/Fyrox/blob/master/LICENSE.md)
[![CI Status](https://github.com/FyroxEngine/Fyrox/actions/workflows/ci.yml/badge.svg)](https://github.com/FyroxEngine/Fyrox/actions/workflows/ci.yml)
[![Dependency status](https://deps.rs/repo/github/FyroxEngine/Fyrox/status.svg)](https://deps.rs/repo/github/FyroxEngine/Fyrox)
[![Crates.io](https://img.shields.io/crates/v/fyrox)](https://crates.io/crates/fyrox)
[![docs.rs](https://img.shields.io/badge/docs-website-blue)](https://docs.rs/Fyrox/)
[![Discord](https://img.shields.io/discord/756573453561102427)](https://discord.gg/xENF5Uh)
[![Lines of code](https://tokei.rs/b1/github/FyroxEngine/Fyrox)](https://github.com/FyroxEngine/Fyrox)

A feature-rich, production-ready, general purpose 2D/3D game engine written in Rust with a scene editor.
_Formerly known as rg3d_

## [Learning materials](https://fyrox-book.github.io/)

[Read the official Fyrox book here.](https://fyrox-book.github.io/) It contains comprehensive information about many aspects of the engine, starting
by "how to build" and ending by various tutorials.

## Community

You can always ask your question in Discord server - [Join the Discord server](https://discord.gg/xENF5Uh), or directly in 
[Discussions](https://github.com/FyroxEngine/Fyrox/discussions).

## Examples

You can run examples directly in your web browser, the full list of demo projects is [available here](https://fyrox.rs/examples.html).
Source code for each demo project [can be found here](https://github.com/FyroxEngine/Fyrox-demo-projects).

## Support

If you want to support the development of the project, click the link below. Preferrable way is to use [Boosty](https://boosty.to/fyrox) - this way the money
will be available for the development immediately. Alternatively you can can use [Patreon](https://www.patreon.com/mrdimas), but in this case the money will
be on-hold for unknown period of time ([details are here](https://github.com/FyroxEngine/Fyrox/issues/363)).

## Contributing

Contributions are very welcome! See the [contributions guidelines](CONTRIBUTING.md) for more info. Check the [good first issue](https://github.com/FyroxEngine/Fyrox/issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22) label to 
see where you can help.

## Sponsors

The engine is supported by very generous people, their donations provides sustainable development of the engine:

[Brandon Thomas](https://www.patreon.com/user?u=34951681) | [Taylor C. Richberger](https://www.patreon.com/user/creators?u=60141723) | [Avery Wagar](https://www.patreon.com/user?u=41863848) |
[George Atkinson](https://www.patreon.com/user?u=61771027) | [Erlend Sogge Heggen](https://www.patreon.com/amethystengine/creators) | [Mitch Skinner](https://www.patreon.com/user/creators?u=60141723) | [ozkriff](https://www.patreon.com/ozkriff) | [Taylor Gerpheide](https://www.patreon.com/user/creators?u=32274918) |
[zrkn](https://www.patreon.com/user/creators?u=23413376) | [Aleks Row](https://www.patreon.com/user/creators?u=51907853) | [Edward L](https://www.patreon.com/user/creators?u=53507198) | [L.apz](https://www.patreon.com/user/creators?u=5448832) | [Luke Jones](https://www.patreon.com/flukejones) | [toyboot4e](https://www.patreon.com/user/creators?u=53758973) | [Vish Vadlamani](https://www.patreon.com/user/creators?u=42768509) |
[Alexey Kuznetsov](https://www.patreon.com/user?u=39375025) | [Daniel Simon](https://www.patreon.com/user/creators?u=43754885) | [Jesper Nordenberg](https://www.patreon.com/jesnor) | [Kornel](https://www.patreon.com/user?u=59867) | [Parham Gholami](https://www.patreon.com/user?u=33009238) | [Yuki Ishii](https://www.patreon.com/user/creators?u=9564103) |
[Joseph Catrambone](https://www.patreon.com/user?u=4738580) | [MGlolenstine](https://github.com/MGlolenstine) | [zamar lomax](https://www.patreon.com/user?u=65928523) | [Gheorghe Ugrik](https://www.patreon.com/user?u=54846813) |
[Anton Zelenin](https://www.patreon.com/user?u=62378966) | [Barugon](https://www.patreon.com/user?u=11344465) | [Tom Leys](https://www.patreon.com/user?u=222856) | [Jay Sistar](https://www.patreon.com/user?u=284041) | [tc](https://www.patreon.com/user?u=11268466) | [false](https://www.patreon.com/user?u=713537) | [BlueSkye](https://www.patreon.com/EmotionalSnow) |
[Ben Anderson](https://www.patreon.com/user/creators?u=14436239) | [Thomas](https://www.patreon.com/user?u=317826) | [Iulian Radu](https://www.patreon.com/user?u=8698230) | [Vitaliy (ArcticNoise) Chernyshev](https://www.patreon.com/user?u=2601918)

### JetBrains

JetBrains provided an open-source all-products license for their products which drastically helps in development of the engine.

<img src="https://resources.jetbrains.com/storage/products/company/brand/logos/jb_beam.png" alt="JetBrains logo." width="200" height="200">

_Copyright © 2000-2021 [JetBrains](https://jb.gg/OpenSource) s.r.o. JetBrains and the JetBrains logo are registered trademarks of JetBrains s.r.o._


---

# yt-grab

Created: 2025-02-11T11:41:05Z
Updated: 2025-02-11T12:42:39Z
URL: https://github.com/mahoraga94/yt-grab


## README

# ytgrab

ytgrab is a Rust-based command-line tool for downloading YouTube videos and audio using `yt-dlp`. It provides an enhanced wrapper around `yt-dlp` with custom progress bars and detailed debugging output.

## Features

- Download YouTube videos and audio
- Choose video quality (360p, 480p, 720p, 1080p)
- Download audio-only files in MP3 format
- Download captions and metadata
- Custom progress bar and detailed logging

## Installation

### Prerequisites

- Rust and Cargo installed on your system
- `yt-dlp` installed and available in your system's PATH

### Installing ytgrab

You can install ytgrab using the following `curl` command on Linux:

```sh
curl -sSL https://raw.githubusercontent.com/skubed0007/yt-grab/main/install.sh | sudo sh


---

# jimmer

Created: 2025-01-31T14:48:50Z
Updated: 2025-02-01T13:38:57Z
URL: https://github.com/mahoraga94/jimmer


## README

# Jimmer Timer
================

A customizable timer application with a quote widget, built using Rust and the TUI library.

## Table of Contents
-----------------

* [Features](#features)
* [Installation](#installation)
* [Usage](#usage)
* [Configuration](#configuration)
* [Themes](#themes)
* [Quote Widget](#quote-widget)
* [Audio](#audio)
* [Troubleshooting](#troubleshooting)
* [Contributing](#contributing)
* [License](#license)

## Features
-----------

* Customizable timer duration in minutes and seconds
* 10 pre-defined themes for the timer and quote widgets
* Quote widget with a collection of inspirational quotes
* Audio playback during the timer (loop and end sounds)
* User-friendly interface with keyboard navigation

## Installation
------------
``curl -sSfL https://raw.githubusercontent.com/skubed0007/jimmer/master/ci.sh | sudo bash``
## Usage
-----

To use the Jimmer Timer, simply run the application and follow the on-screen instructions.

* Use the `--minute` and `--second` options to set the timer duration.
* Use the `--theme` option to choose a theme (1-10).
* Press `P` to pause the timer.
* Press `Q` to quit the application.

## Configuration
-------------

The Jimmer Timer can be configured using the following options:

* `--minute=<value>`: Set the timer duration in minutes.
* `--second=<value>`: Set the timer duration in seconds.
* `--theme=<number>`: Choose a theme for the timer and quotes (1-10).

## Themes
--------

The Jimmer Timer comes with 10 pre-defined themes for the timer and quote widgets. You can choose a theme using the `--theme` option.

## Quote Widget
-------------

The Jimmer Timer features a quote widget that displays inspirational quotes. The quotes are randomly selected from a collection of quotes.

## Audio
------

The Jimmer Timer plays audio during the timer (loop and end sounds). You can customize the audio by providing your own audio files , they shall be in the ``current working directory`` and there shall be 2 files - ``audio.mp3`` and ``end.mp3``  , by default jimmer installation will ship with audio.mp3 and places it at ``/usr/local/bin`` but it also looks for one in current dir, atleast ``audio.mp3`` shall be there

## Contributing
------------

Contributions to the Jimmer Timer are welcome! If you have any ideas or bug fixes, please submit a pull request.

## License
-------

The Jimmer Timer is licensed under the MIT License.

## Default ``audio.mp3``
-------
the ``audio.mp3`` found in this repo is the music named ჩუბინა by chub1na.ge found here :~ https://www.youtube.com/watch?v=cJvCuBlYUqc&pp=ygUd4YOp4YOj4YOR4YOY4YOc4YOQIGNodWIxbmEuZ2U%3D




---

# neitc

Created: 2025-01-23T11:52:51Z
Updated: 2025-01-23T12:26:54Z
URL: https://github.com/mahoraga94/neitc


## README

# Neitc Programming Language

A minimal systems programming language that compiles to C, focusing on explicit memory management and direct system calls.

## Features

- Direct C interoperability via `cimport`
- Low-level I/O operations with `__wrt__` and more
- Explicit buffer size management
- File descriptor based I/O
- Zero-cost abstractions
and many more to come! am working on it!

License: [Apache 2.0](LICENSE.md)

---

# joyroom

Created: 2025-01-17T15:06:22Z
Updated: 2025-01-17T15:26:37Z
URL: https://github.com/mahoraga94/joyroom


## No README


---

# joy

Created: 2024-11-25T15:14:14Z
Updated: 2024-12-11T15:30:29Z
URL: https://github.com/mahoraga94/joy


## No README


---

# clua

Created: 2024-11-16T15:56:13Z
Updated: 2024-11-17T00:41:17Z
URL: https://github.com/mahoraga94/clua


## README

# Clua - Lua to C Compiler

A Lua compiler written in Rust that translates Lua code into C and generates executables.

## Overview

Clua is a specialized Lua compiler that converts Lua source code into C code and then compiles it into an executable. It's designed to be simple and efficient, focusing on basic Lua functionality.

## Features

- Converts Lua code to C
- Handles basic print statements
- AST-based code generation
- Error reporting and validation
- Automatic cleanup of temporary files

## Project Structure

- `src/main.rs`: Entry point and CLI handling
- `src/ast.rs`: Abstract Syntax Tree definitions
- `src/lex.rs`: Lexical analysis and tokenization
- `src/l2c.rs`: Lua to C code generation
- `src/check.rs`: Code validation and error checking

## Requirements

- Rust (latest stable version)
- Clang compiler
- Linux environment

## Usage

```bash
# Basic usage
clua <filename>.lua

# Show help
clua --help
clua -h
```

## Example

Create a file `test.lua`:
```lua
print("Hello, World!")
```

Compile and run:
```bash
clua test.lua
./test.out
```

## Current Limitations

- Only supports basic print statements with string literals
- No support for variables yet
- Limited error handling
- Single file compilation only (directory support coming soon)

## License

### License

This project is licensed under the [***Joay License***](LICENSE), with the following exception:

**Jaytirth Kundan**, the *author* and *creator* of this program, who lives in **India**, with the following emails:
- **joy0094@proton.me**
- **skub007@proton.me**

grants a special, unrestricted license to **Akshat Raj**, his friend, for full usage, modification, and distribution rights of this program. This exception applies **only** to **Akshat Raj**, and the terms of the Joay License apply to **all other users** and entities.

#### Identity Verification:

- **Jaytirth Kundan**, the **original author** of this project, can be uniquely identified as follows:
  - GitHub: [Jaytirth Kundan GitHub](https://github.com/skub007)
  - Email: **joy0094@proton.me** and **skub007@proton.me**
  - Residence: **India**
  - Public references and contributions: This project, along with other open-source projects at Oxumlabs git [here](https://github.com/Oxumlabs), can be found on GitHub. These references and contributions, can be traced back to the original author through commit history, repository ownership, and project documentation

- **Akshat Raj** is granted an exemption based on a **personal relationship** with **Jaytirth Kundan** and is the **only individual** who is not bound by the Joay License in full.

#### Important Conditions:

1. **Special License Exception**:
    - The license exception is **exclusive** to **Akshat Raj known by jaytirth kundan**. No one else is allowed to use this exception, and the standard Joay License applies to all other users.

2. **No Transferability**:
    - This exception is **non-transferable**. Akshat Raj **cannot** transfer or extend this special exception to others.

3. **Anti-Fraud Clause**:
    - Any individual or entity falsely claiming to be **Jaytirth Kundan** (the author) in order to bypass the terms of this license will have their claims considered **fraudulent** and **invalid**.
    - Identity verification is required for any disputes regarding ownership. Official verification can be done through **GitHub** and **email correspondence** linked above.

4. **Documentation and Proof of Ownership**:
    - As the **original creator**, **Jaytirth Kundan** maintains full control over the project, and this project can be verified by checking the **commit history**, **repository ownership**, and **public announcements** made by the author across platforms.
## Note

This project is made specially for Akshat and not intended for outside use.

---

# coreasm

Created: 2024-11-12T14:16:06Z
Updated: 2024-12-28T05:44:38Z
URL: https://github.com/mahoraga94/coreasm


## README

# CoreASM
Allows generating assembly code very easily and quickly for 32-BIT , 64-BIT and ARM for linux (windows code is there but aint tested yet)

## Usage

```rust
use arch::Target;
use coreasm::{CoreAsm, Print, PrintToken};
use maker::maker;

mod arch;
mod coreasm;
mod maker;

fn main() {
    // Initialize CoreAsm and add variables
    let mut asm = CoreAsm::new();
    // add variables to data section
    asm.data.mkvar(
        "name".to_string(),
        coreasm::Types::String,
        coreasm::VarValue::String("joy".to_string()),
    );
    //print
    let mut print = Print::new();
    print.tokens.push(PrintToken::Text("Hello ".to_string()));
    print.tokens.push(PrintToken::Variable("name".to_string()));
    print.tokens.push(PrintToken::Text("!".to_string()));
    print.tokens.push(PrintToken::Newline);
    asm.prints.push(print);
    //generate assembly code
    asm.exit(true);
    let asmcode = maker(
        &asm,
        Target::new(arch::Bit::X64, arch::Arch::X86, arch::OS::Linux),
    );
    println!("{}", asmcode);
}
```

## License

This is free and unencumbered software released into the public domain under the [Unlicense License](LICENSE) with extra requirement being to atleast mention my name ``"joy"`` or ``"jay tirth kundan"`` if not the link to this repo

---

# cutils

Created: 2024-11-07T14:58:41Z
Updated: 2024-11-07T15:34:46Z
URL: https://github.com/mahoraga94/cutils


## README

# cutils : Re-Implementation of core-utils in C

This is the re-implementation of core-utils in C and is designed to be portable across different platforms and compilers
## Functions

### echo_command

Prints the command line arguments to standard output. If an argument starts with '$', it is treated as an environment variable, and its value is printed. Otherwise, the argument itself is printed.

#### Parameters

- text/command/env_var: The (text or command output or environment variable) to be printed to standard output 

#### Example Usage
```bash
cutils echo hello 
cutils echo $HOME
cutils echo $PATH
```
### ls

Lists the files and directories in the specified directory. If the directory does not exist, the function prints an error message and exits.

#### Parameters

- `path`: The path of the directory to list.

#### Example Usage

```bash
cutils ls /home/user
```
### touch

Creates a file with the specified path. If the file already exists, the function prints an error message and exits.

#### Parameters

- `path`: The path of the file to create.

#### Example Usage

```bash
cutils touch /home/user/some_file.txt
```

### cat

reads from file and prints the contents to standard output. If the file does not exist, the function prints an error message and exits.

#### Parameters

- `path`: The path of the file to read.

#### Example Usage

```bash
cutils cat /home/user/some_file.txt
```

### copy

Copies a file from one location to another. If the source file does not exist, the function prints an error message and exits.

#### Parameters

- `source_path`: The path of the file to copy.
- `destination_path`: The path of the destination file.

#### Example Usage

```bash
cutils cp /home/user/some_file.txt /home/user/new_file.txt
```

### move

Moves a file from one location to another. If the source file does not exist, the function prints an error message and exits.

#### Parameters

- `source_path`: The path of the file to move.
- `destination_path`: The path of the destination file.

#### Example Usage

### mkdir

Creates a directory with the specified path. If the directory already exists, the function prints an error message and exits.

#### Parameters

- `path`: The path of the directory to create.directory. Default value is.

#### Example Usage
```bash
cutils mkdir /home/user/some_dir
```

### rm
#### removes the specified file
```bash
cutils rm ./tst.txt
```

### rmdir
#### removes the specified directory
```bash
cutils rmdir ./tst_dir
```
## License

This project is licensed under the GPL License. See the [LICENSE](LICENSE) file for details.

---

# jock

Created: 2024-10-30T04:22:23Z
Updated: 2024-10-30T09:52:07Z
URL: https://github.com/mahoraga94/jock


## README


# Jock Encryption Tool

## Overview

**Jock** is a simple command-line encryption and decryption tool written in Rust. It allows users to securely lock (encrypt) and open (decrypt) files, folders, or messages using a password. The tool provides an interactive command-line interface for quick encryption/decryption of messages and supports both file and folder operations.

## Features

- **Encrypt and Decrypt**: Securely encrypt files using a password.
- **Interactive CLI**: An interactive mode for quickly encrypting or decrypting messages.
- **Progress Indication**: Provides a visual spinner to indicate progress during operations.
- **Custom Output**: Option to specify custom output filenames for encrypted files.

## Installation

1. **Clone the repository**:

   ```bash
   git clone https://github.com/skubed0007/jock
   cd jock
   ```

2. **Build the project**:

   Make sure you have Rust and Cargo installed. Then, run:

   ```bash
   cargo build --release
   ```

3. **Run the tool**:

   After building, you can run the executable:

   ```bash
   ./target/release/jock <command> <args>
   ```

# OR

**Download binary file from release section**
## Usage

### Commands

- **lock**: Encrypts the specified files, folders, or messages.
- **open**: Decrypts the specified files or folders.
- **cli**: Encrypts or decrypts messages interactively.

### Arguments

- `<password>`: A password to secure the encryption or decryption process.
- `<file_or_message>`: The paths of files or folders to encrypt or decrypt, or messages to encrypt.
- `--output <output_file>`: Specify a custom output file path. If not provided, the original files will be saved with a `.jock` extension.

### Examples
> Giving ``--output file_name`` is not needed

1. **Encrypt Files**:

   ```bash
   jock lock mypassword /path/to/file1 /path/to/file2 --output /path/to/encrypted_file
   ```

2. **Decrypt Files**:

   ```bash
   jock open mypassword /path/to/encrypted_file.jock --output /path/to/decrypted_file
   ```

3. **Interactive CLI**:

   ```bash
   jock cli
   ```

> #### Folder Encryption In Work

## License

This project is licensed under the **GNU General Public License v3.0** (GPL-3.0) and the **Creative Commons Attribution 4.0 International License** (CC BY 4.0).

### GNU General Public License v3.0 (GPL-3.0)

The GPL-3.0 license allows users to freely use, modify, and distribute the software, provided that any derivative works are also licensed under the GPL-3.0. This ensures that the software remains free and open-source while protecting the rights of users.

**Key Points**:
- Users can modify and redistribute the code.
- All modifications must be shared under the same GPL-3.0 license.

### Creative Commons Attribution 4.0 International License (CC BY 4.0)

The CC BY 4.0 license allows users to use the material in any way they choose, as long as they provide appropriate credit to the original author. This license is more flexible and can be applied to both open-source and proprietary projects.

**Key Points**:
- Users can use, modify, and distribute the software in any form.
- Users must give credit to the original author, Jaytirth Kundan.

## Author

**Jaytirth Kundan** (2024)

For any questions or suggestions, feel free to open an issue in the repository.

---

---

# bvm

Created: 2024-07-10T14:35:19Z
Updated: 2024-10-16T16:15:00Z
URL: https://github.com/mahoraga94/bvm


## README

# BVM
**BVM or the Bimble Virtual Machine is the Runtime Compute System For the Bimble Programming Language**
___
___
## The Concept
- The BVM takes in the binary code **generated** by the bimble generator and turns it into Bimble Code Back and then go through the codes get all the required functions and everything else necessary and gets ready to execute it
- It then calls the "***main***"  function and starts executing code from there

### ***Please Read The BB DOC present on the Bimble Repo at [Bimble Language](https://github.com/vstartups/BBDoc)***  

---

# bimble

Created: 2024-08-07T10:59:59Z
Updated: 2024-08-10T13:51:48Z
URL: https://github.com/mahoraga94/bimble


## No README


---

# cprojmaker

Created: 2024-06-05T15:58:15Z
Updated: 2024-06-05T15:58:21Z
URL: https://github.com/mahoraga94/cprojmaker


## No README


---

