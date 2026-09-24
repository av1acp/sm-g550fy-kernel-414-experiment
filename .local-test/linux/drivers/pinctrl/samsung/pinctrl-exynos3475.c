/*
 * Exynos3475 pinctrl data for the Linux 4.14 Samsung pinctrl driver.
 *
 * The data is taken from the FY Exynos3475 donor tree.  The surrounding
 * driver remains the upstream 4.14 Samsung pinctrl implementation; this
 * file only supplies the SoC-specific bank layout and DT match data.
 *
 * This is a compile/bring-up slice, not a claim that the pin controller
 * has been electrically validated on the phone.
 */

#include <linux/of.h>

#include "pinctrl-samsung.h"
#include "pinctrl-exynos.h"

static const struct samsung_pin_bank_type exynos3475_bank_type_off = {
	.fld_width = { 4, 1, 2, 4, 2, 2, },
	.reg_offset = { 0x00, 0x04, 0x08, 0x0c, 0x10, 0x14, },
};

static const struct samsung_pin_bank_type exynos3475_bank_type_alive = {
	.fld_width = { 4, 1, 2, 4, },
	.reg_offset = { 0x00, 0x04, 0x08, 0x0c, },
};

#define E3475_PIN_BANK_EINTG(pins, reg, id, offs) \
	{ \
		.type       = &exynos3475_bank_type_off, \
		.pctl_offset = reg, \
		.nr_pins    = pins, \
		.eint_type  = EINT_TYPE_GPIO, \
		.eint_offset = offs, \
		.name       = id, \
	}

#define E3475_PIN_BANK_EINTW(pins, reg, id, offs) \
	{ \
		.type       = &exynos3475_bank_type_alive, \
		.pctl_offset = reg, \
		.nr_pins    = pins, \
		.eint_type  = EINT_TYPE_WKUP, \
		.eint_offset = offs, \
		.name       = id, \
	}

static const struct samsung_pin_bank_data exynos3475_pin_banks0[] __initconst = {
	E3475_PIN_BANK_EINTW(8, 0x000, "gpa0", 0x00),
	E3475_PIN_BANK_EINTW(8, 0x020, "gpa1", 0x04),
	E3475_PIN_BANK_EINTW(3, 0x040, "gpq0", 0x08),
};

static const struct samsung_pin_bank_data exynos3475_pin_banks1[] __initconst = {
	E3475_PIN_BANK_EINTG(4, 0x000, "gpz0", 0x00),
	E3475_PIN_BANK_EINTG(4, 0x020, "gpz1", 0x04),
};

static const struct samsung_pin_bank_data exynos3475_pin_banks2[] __initconst = {
	E3475_PIN_BANK_EINTG(4, 0x000, "gpc4", 0x00),
};

static const struct samsung_pin_bank_data exynos3475_pin_banks3[] __initconst = {
	E3475_PIN_BANK_EINTG(3, 0x000, "gpr0", 0x00),
	E3475_PIN_BANK_EINTG(8, 0x020, "gpr1", 0x04),
	E3475_PIN_BANK_EINTG(2, 0x040, "gpr2", 0x08),
	E3475_PIN_BANK_EINTG(4, 0x060, "gpr3", 0x0c),
	E3475_PIN_BANK_EINTG(6, 0x080, "gpr4", 0x10),
};

static const struct samsung_pin_bank_data exynos3475_pin_banks4[] __initconst = {
	E3475_PIN_BANK_EINTG(2, 0x000, "gpm0", 0x00),
};

static const struct samsung_pin_bank_data exynos3475_pin_banks5[] __initconst = {
	E3475_PIN_BANK_EINTG(4, 0x000, "gpc2", 0x00),
};

static const struct samsung_pin_bank_data exynos3475_pin_banks6[] __initconst = {
	E3475_PIN_BANK_EINTG(8, 0x020, "gpb0", 0x00),
	E3475_PIN_BANK_EINTG(2, 0x040, "gpb1", 0x04),
	E3475_PIN_BANK_EINTG(3, 0x060, "gpc0", 0x08),
	E3475_PIN_BANK_EINTG(4, 0x080, "gpc1", 0x0c),
	E3475_PIN_BANK_EINTG(8, 0x0a0, "gpd0", 0x10),
	E3475_PIN_BANK_EINTG(8, 0x0c0, "gpd1", 0x14),
	E3475_PIN_BANK_EINTG(4, 0x0e0, "gpd2", 0x18),
	E3475_PIN_BANK_EINTG(3, 0x100, "gpe0", 0x1c),
	E3475_PIN_BANK_EINTG(4, 0x120, "gpf0", 0x20),
	E3475_PIN_BANK_EINTG(4, 0x140, "gpf1", 0x24),
};

static const struct samsung_pin_bank_data exynos3475_pin_banks7[] __initconst = {
	E3475_PIN_BANK_EINTG(3, 0x000, "gpc3", 0x00),
};

const struct samsung_pin_ctrl exynos3475_pin_ctrl[] __initconst = {
	{
		.pin_banks = exynos3475_pin_banks0,
		.nr_banks = ARRAY_SIZE(exynos3475_pin_banks0),
		.eint_gpio_init = exynos_eint_gpio_init,
		.eint_wkup_init = exynos_eint_wkup_init,
		.suspend = exynos_pinctrl_suspend,
		.resume = exynos_pinctrl_resume,
	}, {
		.pin_banks = exynos3475_pin_banks1,
		.nr_banks = ARRAY_SIZE(exynos3475_pin_banks1),
		.eint_gpio_init = exynos_eint_gpio_init,
		.suspend = exynos_pinctrl_suspend,
		.resume = exynos_pinctrl_resume,
	}, {
		.pin_banks = exynos3475_pin_banks2,
		.nr_banks = ARRAY_SIZE(exynos3475_pin_banks2),
		.eint_gpio_init = exynos_eint_gpio_init,
		.suspend = exynos_pinctrl_suspend,
		.resume = exynos_pinctrl_resume,
	}, {
		.pin_banks = exynos3475_pin_banks3,
		.nr_banks = ARRAY_SIZE(exynos3475_pin_banks3),
		.eint_gpio_init = exynos_eint_gpio_init,
		.suspend = exynos_pinctrl_suspend,
		.resume = exynos_pinctrl_resume,
	}, {
		.pin_banks = exynos3475_pin_banks4,
		.nr_banks = ARRAY_SIZE(exynos3475_pin_banks4),
		.eint_gpio_init = exynos_eint_gpio_init,
		.suspend = exynos_pinctrl_suspend,
		.resume = exynos_pinctrl_resume,
	}, {
		.pin_banks = exynos3475_pin_banks5,
		.nr_banks = ARRAY_SIZE(exynos3475_pin_banks5),
		.eint_gpio_init = exynos_eint_gpio_init,
		.suspend = exynos_pinctrl_suspend,
		.resume = exynos_pinctrl_resume,
	}, {
		.pin_banks = exynos3475_pin_banks6,
		.nr_banks = ARRAY_SIZE(exynos3475_pin_banks6),
		.eint_gpio_init = exynos_eint_gpio_init,
		.suspend = exynos_pinctrl_suspend,
		.resume = exynos_pinctrl_resume,
	}, {
		.pin_banks = exynos3475_pin_banks7,
		.nr_banks = ARRAY_SIZE(exynos3475_pin_banks7),
		.eint_gpio_init = exynos_eint_gpio_init,
		.suspend = exynos_pinctrl_suspend,
		.resume = exynos_pinctrl_resume,
	},
};
