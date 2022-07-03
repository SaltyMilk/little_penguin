// SPDX-License-Identifier: GPL-2.0+
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/slab.h>

MODULE_AUTHOR("IROH");
MODULE_LICENSE("GPL");

int do_work(int *my_int, int retval)
{
	int x;
	int y = *my_int;
	int z;


	(void)retval;
	for (x = 0; x < *my_int; ++x)
		udelay(10);

	if (y < 10)
		pr_info("We slept a long time!");

	z = x * y;
	return z;
}

int my_init(void)
{
	int ret = 10;

	ret = do_work(&ret, ret);
	return ret;
}

void my_exit(void)
{
}

module_init(my_init);
module_exit(my_exit);

