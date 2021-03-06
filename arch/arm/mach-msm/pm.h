/* arch/arm/mach-msm/pm.h
 *
 * Copyright (C) 2007 Google, Inc.
 * Copyright (c) 2009-2012, The Linux Foundation. All rights reserved.
 * Author: San Mehat <san@android.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __ARCH_ARM_MACH_MSM_PM_H
#define __ARCH_ARM_MACH_MSM_PM_H

#include <linux/types.h>
#include <linux/cpuidle.h>

/* This constant is used in bootloader to decide actions. */
#define RESTART_REASON_BOOT_BASE	0x77665500
#define RESTART_REASON_BOOTLOADER	(RESTART_REASON_BOOT_BASE | 0x00)
#define RESTART_REASON_REBOOT		(RESTART_REASON_BOOT_BASE | 0x01)
#define RESTART_REASON_RECOVERY		(RESTART_REASON_BOOT_BASE | 0x02)
#define RESTART_REASON_RAMDUMP		(RESTART_REASON_BOOT_BASE | 0xAA)
#define RESTART_REASON_POWEROFF		(RESTART_REASON_BOOT_BASE | 0xBB)
#define RESTART_REASON_ERASE_FLASH	(RESTART_REASON_BOOT_BASE | 0xEF)

/*
   This restart constant is used for oem commands.
   The actual value is parsed from reboot commands.
   RIL FATAL will use oem-99 to restart a device.
*/
#define RESTART_REASON_OEM_BASE		0x6f656d00
#define RESTART_REASON_RIL_FATAL	(RESTART_REASON_OEM_BASE | 0x99)

#ifdef CONFIG_SMP
extern void msm_secondary_startup(void);
#else
#define msm_secondary_startup NULL
#endif

enum msm_pm_sleep_mode {
	MSM_PM_SLEEP_MODE_POWER_COLLAPSE_SUSPEND,
	MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
	MSM_PM_SLEEP_MODE_APPS_SLEEP,
	MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT,
	MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT,
	MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN,
	MSM_PM_SLEEP_MODE_POWER_COLLAPSE_STANDALONE,
	MSM_PM_SLEEP_MODE_NR
};

#define MSM_PM_MODE(cpu, mode_nr)  ((cpu) * MSM_PM_SLEEP_MODE_NR + (mode_nr))

struct msm_pm_platform_data {
	u8 idle_supported;   /* Allow device to enter mode during idle */
	u8 suspend_supported; /* Allow device to enter mode during suspend */
	u8 suspend_enabled;  /* enabled for suspend */
	u8 idle_enabled;     /* enabled for idle low power */
	u32 latency;         /* interrupt latency in microseconds when entering
				and exiting the low power mode */
	u32 residency;       /* time threshold in microseconds beyond which
				staying in the low power mode saves power */
};

struct msm_pm_sleep_status_data {
	void *base_addr;
	uint32_t cpu_offset;
	uint32_t mask;
};

void msm_pm_set_platform_data(struct msm_pm_platform_data *data, int count);
int msm_pm_idle_prepare(struct cpuidle_device *dev);
int msm_pm_idle_enter(enum msm_pm_sleep_mode sleep_mode);
void msm_pm_cpu_enter_lowpower(unsigned int cpu);

void __init msm_pm_init_sleep_status_data(
		struct msm_pm_sleep_status_data *sleep_data);
#ifdef CONFIG_MSM_PM8X60
void msm_pm_set_rpm_wakeup_irq(unsigned int irq);
int msm_pm_wait_cpu_shutdown(unsigned int cpu);
bool msm_pm_verify_cpu_pc(unsigned int cpu);
#else
static inline void msm_pm_set_rpm_wakeup_irq(unsigned int irq) {}
static inline int msm_pm_wait_cpu_shutdown(unsigned int cpu) { return 0; }
static inline bool msm_pm_verify_cpu_pc(unsigned int cpu) { return true; }
#endif

#ifdef CONFIG_HOTPLUG_CPU
int msm_platform_secondary_init(unsigned int cpu);
#else
static inline int msm_platform_secondary_init(unsigned int cpu) { return 0; }
#endif
#endif  /* __ARCH_ARM_MACH_MSM_PM_H */
