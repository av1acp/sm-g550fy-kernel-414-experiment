/*
 * Copyright (c) 2010-2011 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * EXYNOS - Memory map definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __ASM_ARCH_MAP_H
#define __ASM_ARCH_MAP_H __FILE__

#include <plat/map-base.h>

#include <plat/map-s5p.h>

#define EXYNOS_PA_CHIPID		0x10000000

#define EXYNOS4_PA_COREPERI		0x10500000

/* Exynos3475 (universal3475) physical register blocks. */
#define EXYNOS3475_PA_CMU_TOP		0x10010000
#define EXYNOS3475_PA_CMU_MIF		0x10430000
#define EXYNOS3475_PA_CMU_CPU		0x10900000
#define EXYNOS3475_PA_CMU_IMEM		0x10C60000
#define EXYNOS3475_PA_CMU_G3D		0x11460000
#define EXYNOS3475_PA_CMU_BUS0		0x11800000
#define EXYNOS3475_PA_CMU_MFCMSCL	0x12CB0000
#define EXYNOS3475_PA_CMU_BUS2		0x13000000
#define EXYNOS3475_PA_CMU_FSYS		0x13730000
#define EXYNOS3475_PA_CMU_PERI		0x139F0000
#define EXYNOS3475_PA_CMU_ISP		0x144D0000
#define EXYNOS3475_PA_CMU_DISPAUD	0x14870000
#define EXYNOS3475_PA_PMU		0x10460000
#define EXYNOS3475_PA_SYSRAM_NS		0x02055000

#define EXYNOS3475_VA_PMU		S3C_ADDR(0x02180000)
#define EXYNOS3475_VA_SYSRAM_NS		S3C_ADDR(0x02410000)

#endif /* __ASM_ARCH_MAP_H */
