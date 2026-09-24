# Phase 7 — Exynos3475 Clock Driver Port (Research + Implementation Plan)

## Status
**Foundation complete; implementation is a multi-day kernel engineering task.**

## Verified source facts (2026-09-24)
- **4.14 Exynos4 CCF registration point**:
  `drivers/clk/samsung/clk-exynos4.c` → `ext_clk_match[]` at line 1275.
- **Exynos3475 clock implementation (3.10 donor)**:
  `arch/arm/mach-exynos/exynos-clk.c` (Samsung custom clock framework,
  NOT CCF) + `regs-clock-exynos3475.h`.
- **4.14 pinctrl framework** already exists at
  `drivers/pinctrl/samsung/pinctrl-exynos*.c` (verified compile).
- **Reference implementation**: upstream Linux Exynos3250 patch (785-line
  `clk-exynos3250.c` + DTS + bindings) is the same-generation
  architecture approach for a newer Exynos SoC.

## Required implementation
1. Create `drivers/clk/samsung/clk-exynos3475.c` from donor clock tables,
   converted to Common Clock Framework (4.14 APIs).
2. Add `include/dt-bindings/clock/exynos3475.h` and register the SoC in
   `clk-exynos4.c` `ext_clk_match[]`.
3. Exynos3250 upstream patch = exact same pattern/roadmap.
4. Machine init: port `mach-universal3475.c`, `pm-exynos3475.c`,
   `pm_domains-exynos3475.c`, `pmu-exynos3475.c` from donor to 4.14.
5. DTS: convert `exynos3475-universal3475.dts` to 4.14 phandle syntax.

## Gate
- Compile-only for every stage.
- No device flash until a real device-tree + early console proof exists.
- Working 3.10.9 + KSU is isolated in
  `~/Downloads/j2/backups_3.10.9_KSU_WORKING/` and will not be modified.

## Honest estimate
1-2 weeks full-time for a real early-boot port; 2-3 months for full
Android userspace/peripherals including MobiCore/TEE, display, camera.
