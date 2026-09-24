#!/usr/bin/env python3
"""Phase 11 static regression gate for the stock Exynos3475 eMMC contract.

The gate is deliberately read-only.  It proves that the source recognizes the
stock DTBH UART/eMMC contract; it does not prove that a phone boots.  The
normal local run uses the extracted full stock DTBH when present.  CI supplies
PHASE11_KERNEL_TREE=source/linux and PHASE11_DTS=docs/phase11-stock-dtbh-contract.dtsi
because the large extracted DTBH is intentionally not committed.
"""
from pathlib import Path
import os
import re
import sys

ROOT = Path(__file__).resolve().parents[1]
TREE = Path(os.environ.get("PHASE11_KERNEL_TREE", str(ROOT / ".local-test/linux"))).resolve()
FULL_DTS = Path("/home/pi/Downloads/j2/phase10-package/stock-dtbh.expanded.dts")
FIXTURE = ROOT / "docs/phase11-stock-dtbh-contract.dtsi"
DTS = Path(os.environ.get("PHASE11_DTS", str(FULL_DTS if FULL_DTS.exists() else FIXTURE))).resolve()

paths = {
    "mmc": TREE / "drivers/mmc/host/dw_mmc-exynos.c",
    "core": TREE / "drivers/mmc/host/dw_mmc.c",
    "exynos_header": TREE / "drivers/mmc/host/dw_mmc-exynos.h",
    "core_header": TREE / "drivers/mmc/host/dw_mmc.h",
    "dts": DTS,
}
for name, path in paths.items():
    if not path.is_file():
        print(f"FAIL: missing {name}: {path}", file=sys.stderr)
        sys.exit(1)

texts = {name: path.read_text(encoding="utf-8", errors="replace") for name, path in paths.items()}


def node_block(address: str, text: str) -> str:
    """Return one balanced device-tree node block, including its label."""
    node = re.escape(address)
    label = r"[A-Za-z_][A-Za-z0-9_]*"
    match = re.search(rf"(?m)^\s*(?:{label}\s*:\s*)?{node}\s*\{{", text)
    if not match:
        return ""
    start = text.find("{", match.start())
    if start < 0:
        return ""
    depth = 0
    for pos in range(start, len(text)):
        if text[pos] == "{":
            depth += 1
        elif text[pos] == "}":
            depth -= 1
            if depth == 0:
                return text[match.start():pos + 1]
    return ""


mmc0 = node_block("dwmmc0@13540000", texts["dts"])
checks = [
    ("stock eMMC0 node present", bool(mmc0)),
    ("stock eMMC compatible", 'compatible = "samsung,exynos3475-dw-mshc"' in mmc0),
    ("stock eMMC clock names", all(
        name in mmc0 for name in ('"biu"', '"gate_ciu"', '"dout_mmc_a"', '"dout_mmc_b"')
    )),
    ("stock eMMC timing table", all(
        name in mmc0 for name in (
            "samsung,dw-mshc-sdr-timing",
            "samsung,dw-mshc-ddr-timing",
            "ciu_clkin",
        )
    )),
    ("driver Exynos3475 type", "DW_MCI_TYPE_EXYNOS3475" in texts["mmc"]),
    ("driver Exynos3475 match", 'samsung,exynos3475-dw-mshc' in texts["mmc"]),
    ("driver accepts gate_ciu", '"gate_ciu"' in texts["core"]),
    ("driver keeps generic ciu fallback", '"ciu"' in texts["core"]),
    ("driver consumes dout_mmc_a", '"dout_mmc_a"' in texts["mmc"]),
    ("driver consumes dout_mmc_b", '"dout_mmc_b"' in texts["mmc"]),
    ("Exynos3475 4-value timing encoding", "SDMMC_CLKSEL_TIMING4" in texts["exynos_header"]),
    ("Exynos3475 CLKSEL offset", "#define SDMMC_CLKSEL" in texts["exynos_header"] and "0x0A8" in texts["exynos_header"]),
    ("Exynos3475 IDSTS offset", "#undef SDMMC_IDSTS" in texts["core_header"] and "#define SDMMC_IDSTS" in texts["core_header"]),
    ("Exynos3475 IDINTEN offset", "#undef SDMMC_IDINTEN" in texts["core_header"] and "#define SDMMC_IDINTEN" in texts["core_header"]),
    ("Exynos3475 DSCADDR offset", "#undef SDMMC_DSCADDR" in texts["core_header"] and "#define SDMMC_DSCADDR" in texts["core_header"]),
    ("Exynos3475 BUFADDR offset", "#undef SDMMC_BUFADDR" in texts["core_header"] and "#define SDMMC_BUFADDR" in texts["core_header"]),
]

failed = [name for name, ok in checks if not ok]
for name, ok in checks:
    print(("PASS" if ok else "FAIL") + ": " + name)

if failed:
    print(f"Phase 11 eMMC audit failed ({len(failed)} missing contract(s)).", file=sys.stderr)
    sys.exit(1)

print("PHASE11_EMMC_AUDIT_PASS")
