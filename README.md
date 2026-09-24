# Linux 4.5 Port Experiment — Samsung SM-G550FY (Exynos3475 / universal3475)

**EXPERIMENTAL / RESEARCH-ONLY BRANCH — NOT PRODUCTION**

## Status

| Phase | State | Evidence |
|---|---|---|
| 0. Feasibility research | DONE | No 4.x Exynos3475 tree exists upstream, Samsung opensource, or community |
| 1. Source selection | DONE | Linux 4.5 base (closest reachable to 3.10; 2016-2017 era) |
| 2. Driver inventory | DONE | 793 MB Samsung downstream tree, ~47.5k files |
| 3. Board bring-up port | NEXT | arch/arm/mach-exynos + mach-universal3475 + defconfig |
| 4. Boot test (serial/QEMU first) | PENDING | Stage-0: U-Boot/QEMU virt, Stage-1: hardware |
| 5. Peripheral bring-up | PENDING | display, touch, camera, WiFi, TEE |
| 6. Android 7.1 HAL validation | PENDING | vendor blobs, gatekeeper, camera HAL |

## Hard feasibility facts (verified 2026-09-24, MCP research)

- Linux mainline 7.x DTS list contains Exynos2200/5433/7420/7870/7885/850/8855/8890/8895/9810/990 —
  **Exynos3475 is absent in every version.** The SoC exists only in Samsung downstream trees.
- E3475 org kernel repo (`Exynos3475/android_kernel_samsung_exynos3475`, 5 stars, last push 2023-03)
  has branches `cm-14.1`, `cm-14.1-rebase`, `lineage-15.1`, `lineage-17.1`, `clean_base` —
  **all are 3.10.x**. No 4.x branch has ever existed.
- Community/XDA evidence: 2019 J2lte Nougat kernel got to boot animation then stalled
  (Gatekeeper HAL / TZDaemon); 2018 dotOS 3.10 build shipped with no camera and no speaker
  audio. No working 4.x boot on Exynos3475 has ever been reported.
- ARM32 is still supported upstream (armv7l), so architecture is not the blocker;
  **the blocker is that Exynos3475 board support and its vendor drivers are
  3.10-only downstream code.**

## What a 4.5 port actually requires

1. **Board/SoC layer** — port `mach-exynos`, `mach-universal3475`, clock/pinmux/power-domain
   drivers from 3.10 to 4.5 APIs (4.4+ replaced `struct device` platform hooks, moved to
   `of_`/pinctrl drivers, converted clock gating to CCF).
2. **DT rewrite** — the 3.10 tree uses flattened non-DT `exynos-4.1`-style nodes
   (`samsung,universal3475-dtb` appended DTB). 4.5 requires proper DT (`include/dt/`
   bindings + `-pinctrl` + `power-domains` + `clocks` phandles).
3. **Vendor driver ports** — these do not build on 4.x without rewrites:
   - `drivers/gpu/arm/mali-midgard` (UMT/T72X, proprietary)
   - `drivers/video/exynos/decon_3475` (DECON display controller)
   - `drivers/media/samsung` (ISP/fin/MTF, 3.10 FIMC/FIMV ABI)
   - `drivers/sensors/...`, `drivers/input/touchscreen/sec_*` (I2C HID ABI changes)
   - `drivers/mmc` host (dw_mmc), `drivers/rtc`, `drivers/staging/samsung/ipa`
   - `drivers/misc/secure_ctrl` (TEE/MobiCore) — **required for Gatekeeper/RPM**
4. **Bootloader** — Samsung Download Mode bootloader must accept a 4.5 `zImage`.
   3.10 zImage and 4.x zImage differ in header expectations; this is a separate
   bring-up task and may not be achievable without bootloader patching.
5. **Android userspace** — Android 7.1 (2016) vendor HALs were built against 3.10
   ABI. A 4.5 kernel is not ABI compatible: `binder`, `ashmem`, `dm-verity` ioctls,
   `v4l2` ISP, gatekeeper TEE interface all need revalidation.

## Phase plan (the experiment)

### Phase 0 — Non-hardware proof (safe, CI-only)
- Fetch Linux 4.5 source (kernel.org v4.5 tarball).
- Add `mach-exynos3475` skeleton + defconfig that compiles **without** device drivers.
- Build `zImage` on GitHub Actions to prove 4.5 cross-compiles for ARMv7.
- **No flashing.** Deliverable: a green CI artifact + compilability report.

### Phase 1 — QEMU/virt bring-up
- Verify 4.5 boots under QEMU `virt` ARM (`qemu-system-arm -M virt`).
- This validates the 4.5 tree + toolchain + basic boot, not the SoC.

### Phase 2 — Early SoC bring-up (hardware, risky)
- Add only: clock, pinctrl, UART (`ttySAC`), timer, watchdog, GPIO.
- Flash via Heimdall `--BOOT`, boot with `console=ttySAC2` capture.
- **Success criterion: kernel reaches `init` (userspace), not just the banner.**

### Phase 3 — Storage + userspace
- eMMC (`dw_mmc`) root mount, initramfs/`/system`, adb over USB.
- **Success: adb devices + shell prompt.**

### Phase 4 — Peripherals
- Display (DECON/Mali), touch, WiFi/BT, audio, camera, sensors, GPS, TEE.
- Each is independently gated; a missing one is not a boot failure.

### Phase 5 — Android parity
- vendor blobs, HAL init, SELinux policy, gatekeeper/RPM, DRM, media codecs.
- **Success: lockscreen + camera + network = device-replacement-grade.**

## Risk register

| Risk | Probability | Impact | Mitigation |
|---|---|---|---|
| Bootloader rejects 4.5 zImage | High | Fatal | Header/bootparam diff analysis, early phase-2 spike |
| TEE/Gatekeeper cannot init | Very high | Boot hangs (as 2019 J2 report) | Phase-2 gate; no real /data use until gatekeeper works |
| Camera/ISP never init | Very high | Camera dead | Accept; use 3.10 kernel for camera |
| Display/Mali port weeks of work | High | Black screen | Keep 3.10 fallback; stage-gated |
| SELinux policy/kernels ABI mismatch | High | Randomized crashes | Run `enforcing` only after `permissive` is stable |

## Guiding principle

Every phase is a separate GitHub Actions run with an artifact. Nothing is flashed
to the device until Phase 2 has a green CI build and an explicit human go-ahead.
The known-good `boot_FY_KSU15.img` remains the daily driver throughout.

## Evidence sources (this experiment's research)

- `mcp__donsetch` / `mcp__exa`: mainline DTS list (7.3 Samsung DTS pull) — no Exynos3475.
- `gh api orgs/E3475/repos` — kernel repo, branches all 3.10.x.
- XDA: `[CLOSED][J2 lte] Found a working Nougat kernel` (2019) — Gatekeeper/TZDaemon stall.
- XDA: `DOT OS for Samsung Galaxy On5` (2018) — 3.10, camera/audio broken.
- XDA: LineageOS 14.1 for o5prolte (2018) — 3.10.x, "works perfectly except cam/audio".
