/*
 *  XWRT M-AP300G board support
 *
 *  Copyright (C) 2019 Chen Minqiang <ptpt52@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/pci.h>
#include <linux/phy.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/ath9k_platform.h>
#include <linux/ar8216_platform.h>

#include <asm/mach-ath79/ar71xx_regs.h>

#include "common.h"
#include "dev-ap9x-pci.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-spi.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#include "machtypes.h"

#define XWRT_GPIO_LED_STATUS		12
#define XWRT_GPIO_LED_SYSTEM		13
#define XWRT_GPIO_LED_485_1			0
#define XWRT_GPIO_LED_485_2			3
#define XWRT_GPIO_LED_LAN1			1
#define XWRT_GPIO_LED_LAN2			2
#define XWRT_GPIO_LED_SFP1			15
#define XWRT_GPIO_LED_SFP2			14
#define XWRT_GPIO_LED_SFP3			19
#define XWRT_GPIO_LED_SIGNAL		16

#define XWRT_GPIO_BTN_RESET			17

#define XWRT_KEYS_POLL_INTERVAL	20	/* msecs */
#define XWRT_KEYS_DEBOUNCE_INTERVAL	(3 * XWRT_KEYS_POLL_INTERVAL)

#define XWRT_MAC0_OFFSET		0
#define XWRT_WMAC_CALDATA_OFFSET	0x1000

static struct gpio_led xwrt_leds_gpio[] __initdata = {
	{
		.name		= "xwrt:green:status",
		.gpio		= XWRT_GPIO_LED_STATUS,
		.active_low	= 1,
	},
	{
		.name		= "xwrt:green:system",
		.gpio		= XWRT_GPIO_LED_SYSTEM,
		.active_low	= 1,
	},
	{
		.name		= "xwrt:green:led485_1",
		.gpio		= XWRT_GPIO_LED_485_1,
		.active_low	= 1,
	},
	{
		.name		= "xwrt:green:led485_2",
		.gpio		= XWRT_GPIO_LED_485_2,
		.active_low	= 1,
	},
	{
		.name		= "xwrt:green:lan1",
		.gpio		= XWRT_GPIO_LED_LAN1,
		.active_low	= 1,
	},
	{
		.name		= "xwrt:green:lan2",
		.gpio		= XWRT_GPIO_LED_LAN2,
		.active_low	= 1,
	},
	{
		.name		= "xwrt:green:sfp1",
		.gpio		= XWRT_GPIO_LED_SFP1,
		.active_low	= 1,
	},
	{
		.name		= "xwrt:green:sfp2",
		.gpio		= XWRT_GPIO_LED_SFP2,
		.active_low	= 1,
	},
	{
		.name		= "xwrt:green:sfp3",
		.gpio		= XWRT_GPIO_LED_SFP3,
		.active_low	= 1,
	},
	{
		.name		= "xwrt:green:signal",
		.gpio		= XWRT_GPIO_LED_SIGNAL,
		.active_low	= 1,
	},
};

static struct gpio_keys_button xwrt_gpio_keys[] __initdata = {
	{
		.desc		= "Reset button",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = XWRT_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= XWRT_GPIO_BTN_RESET,
		.active_low	= 1,
	},
};

static void __init xwrt_setup(void)
{
	u8 *art = (u8 *) KSEG1ADDR(0x1fff0000);
	u8 tmpmac[ETH_ALEN];

	ath79_register_m25p80(NULL);
	ath79_register_leds_gpio(-1, ARRAY_SIZE(xwrt_leds_gpio),
				 xwrt_leds_gpio);
	ath79_register_gpio_keys_polled(-1, XWRT_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(xwrt_gpio_keys),
					xwrt_gpio_keys);

	ath79_init_mac(tmpmac, art + XWRT_MAC0_OFFSET, 2);
	ath79_register_wmac(art + XWRT_WMAC_CALDATA_OFFSET, tmpmac);

	/* enable usb */
	ath79_register_usb();
	ath79_register_mdio(1, 0x0);

#if 0
	/* register eth0 as WAN, eth1 as LAN */
	ath79_init_mac(ath79_eth0_data.mac_addr, art + XWRT_MAC0_OFFSET, 1);
	ath79_switch_data.phy4_mii_en = 1;
	ath79_switch_data.phy_poll_mask = BIT(4);
	ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_MII;
	ath79_eth0_data.phy_mask = BIT(4);
	ath79_eth0_data.mii_bus_dev = &ath79_mdio1_device.dev;
	ath79_register_eth(0);
#endif

	ath79_init_mac(ath79_eth1_data.mac_addr, art + XWRT_MAC0_OFFSET, 0);
	ath79_eth1_data.phy_if_mode = PHY_INTERFACE_MODE_GMII;
	ath79_register_eth(1);
}

MIPS_MACHINE(ATH79_MACH_XWRT_M_AP300G, "XWRT-M-AP300G",
	     "XWRT M-AP300G",
	     xwrt_setup);
