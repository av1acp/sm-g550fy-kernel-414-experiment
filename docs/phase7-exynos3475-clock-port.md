# Phase 7 — Exynos3475 Clock Driver Port (Compile Gate)

## Status
**Phase 7a/7b implementation is in the experiment repository. The two new
clock objects compile locally; GitHub Actions full-kernel verification is the
next gate. This is not a hardware-boot claim.**

## Corrected source facts (verified 2026-09-24)
- The donor Exynos3475 CCF implementation is actually
  `drivers/clk/samsung/clk-exynos3475.c`, not
  `arch/arm/mach-exynos/exynos-clk.c`.
- Its dependency is the donor vendor composite implementation
  `drivers/clk/samsung/composite.c` plus `composite.h`.
- The 4.14 tree already has a common Samsung CCF layer in
  `drivers/clk/samsung/clk.c`, `clk-pll.c`, and `clk.h`; the donor composite
  layer was therefore namespaced and adapted to the 4.14 public clock API.
- Linux 4.14 does not provide the donor's virtual CMU map. The compile gate
  uses an explicitly marked placeholder virtual window; physical CMU mapping
  belongs to the later machine-init/early-boot gate.

## Files added/changed
- `drivers/clk/samsung/clk-exynos3475.c`
- `drivers/clk/samsung/composite-exynos3475.c`
- `drivers/clk/samsung/composite-exynos3475.h`
- `arch/arm/mach-exynos/include/mach/regs-clock-exynos3475.h`
- `include/dt-bindings/clock/exynos3475.h`
- `drivers/clk/samsung/Makefile`
- `arch/arm/mach-exynos/Kconfig`
- `scripts/dtc/dtc-parser.tab.c_shipped` (known GCC-10+ yylloc fix)

## Local verification
Both objects compile with Linux 4.14 headers:

```
drivers/clk/samsung/composite-exynos3475.o
 drivers/clk/samsung/clk-exynos3475.o
```

The workflow `.github/workflows/phase7-exynos3475-clock-port.yml` fetches a
fresh kernel.org 4.14 tree, the pinned FY donor tree, the era-matched ARM32
toolchain, compiles both objects, then builds `zImage` and `dtbs`.

## Remaining hardware gate
- Replace the placeholder virtual CMU map with real early machine mapping.
- Port the Exynos3475 machine/pinctrl/DT and prove an early serial console.
- Do not flash or claim phone boot from this compile gate.
- Working 3.10.9 + KernelSU remains isolated in
  `~/Downloads/j2/backups_3.10.9_KSU_WORKING/`.
