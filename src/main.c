// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * DRM driver for 2.7" Sharp Memory LCD
 *
 * Copyright 2023 Andrew D'Angelo
 */

#include <linux/module.h>
#include <linux/spi/spi.h>

#include "drm_iface.h"
#include "ioctl_iface.h"
#include "params_iface.h"

static int sharp_memory_probe(struct spi_device *spi)
{
	int ret;

	printk(KERN_INFO "sharp_memory: entering sharp_memory_probe\n");

	if ((ret = drm_probe(spi))) {
		return ret;
	}

	if ((ret = params_probe())) {
		return ret;
	}

	if ((ret = ioctl_probe())) {
		return ret;
	}

	printk(KERN_INFO "sharp_memory: successful probe\n");

	return 0;
}

static void sharp_memory_remove(struct spi_device *spi)
{
	ioctl_remove();
	params_remove();
	drm_remove(spi);
}

static void sharp_memory_shutdown(struct spi_device *spi)
{
	sharp_memory_remove(spi);
}

static const struct spi_device_id sharp_drm_spi_id[] = {
	{"sharp-drm", 0},
	{},
};
MODULE_DEVICE_TABLE(spi, sharp_drm_spi_id);

static const struct of_device_id sharp_drm_spi_of_match[] = {
	{.compatible = "sharp-drm"},
	{},
};
MODULE_DEVICE_TABLE(of, sharp_drm_spi_of_match);

static struct spi_driver sharp_memory_spi_driver = {
	.driver = {
		.name = "sharp-drm",
		.of_match_table = of_match_ptr(sharp_drm_spi_of_match),
	},
	.id_table = sharp_drm_spi_id,
	.probe = sharp_memory_probe,
	.remove = sharp_memory_remove,
	.shutdown = sharp_memory_shutdown,
};
module_spi_driver(sharp_memory_spi_driver);

MODULE_VERSION("1.1");
MODULE_DESCRIPTION("Sharp Memory LCD DRM driver");
MODULE_AUTHOR("Andrew D'Angelo");
MODULE_LICENSE("GPL");
