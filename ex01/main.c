// SPDX-License-Identifier: GPL-2.0+
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_AUTHOR("531-M31C");
MODULE_LICENSE("GPL");


static int __init main_init(void)
{
	printk(KERN_INFO "Hello world !\n");
	return 0;
}

static void __exit main_clean(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
}
module_init(main_init);
module_exit(main_clean);
