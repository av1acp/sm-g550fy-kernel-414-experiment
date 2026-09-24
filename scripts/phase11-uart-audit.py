#!/usr/bin/env python3
"""Phase 11 regression gate for the Exynos3475 stock-DTB UART contract.

The stock FY DTBH uses samsung,exynos3475-uart with the separate-uart-clk
property and three named clocks.  The Linux 4.14 Samsung UART driver must
recognise that contract before a full zImage can be called flashable.
"""
from pathlib import Path
import os
import sys

ROOT = Path(__file__).resolve().parents[1]
TREE = Path(os.environ.get("PHASE11_KERNEL_TREE", str(ROOT / ".local-test/linux"))).resolve()
SRC = TREE / "drivers/tty/serial/samsung.c"
HDR = TREE / "drivers/tty/serial/samsung.h"

source = SRC.read_text(encoding="utf-8", errors="replace")
header = HDR.read_text(encoding="utf-8", errors="replace")

checks = [
    ("Exynos3475 DT match", 'compatible = "samsung,exynos3475-uart"' in source),
    ("Exynos3475 platform id", '.name\t\t= "exynos3475-uart"' in source),
    ("separate-uart-clk property", 'samsung,separate-uart-clk' in source),
    ("gate_uart clock lookup", 'gate_uart%d' in source),
    ("gate_pclk clock lookup", 'gate_pclk%d' in source),
    ("sclk_uart baud clock lookup", 'sclk_uart%d' in source),
    ("separated clock state", "struct clk\t\t\t*separated_clk;" in header),
    ("Exynos3475 earlycon", 'OF_EARLYCON_DECLARE(exynos3475' in source),
]

failed = [name for name, ok in checks if not ok]
for name, ok in checks:
    print(("PASS" if ok else "FAIL") + ": " + name)

if failed:
    print(f"Phase 11 UART audit failed ({len(failed)} missing contract(s)).", file=sys.stderr)
    sys.exit(1)

print("PHASE11_UART_AUDIT_PASS")
