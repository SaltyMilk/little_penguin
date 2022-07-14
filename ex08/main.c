// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("sel-melc");
MODULE_DESCRIPTION("Funny FD module");

static char str[PAGE_SIZE + 1];

ssize_t myfd_read(struct file *fp,
		char __user *user,
		size_t size,
		loff_t *offs)
{
	size_t	t;
	size_t	i;
	char	*tmp;
	ssize_t	ret = 0;
	size_t	length;

	length = strlen(str);
	if (length == 0)
		return 0;

	tmp = kmalloc(sizeof(str), GFP_KERNEL);
	if (tmp == NULL)
		return -ENOMEM;

	for (t = length - 1, i = 0; i < length; t--, i++)
		tmp[i] = str[t];

	tmp[i] = 0x0;
	ret = simple_read_from_buffer(user, size, offs, tmp, length);
	kfree(tmp);
	return ret;
}

ssize_t myfd_write(struct file *fp, const char __user *user, size_t size,
		loff_t *offs)
{
	ssize_t res;

	res = simple_write_to_buffer(str, sizeof(str) - 1, offs, user, size);
	if (res < 0)
		return -EIO;
	str[res] = '\0';
	return res;
}

static const struct file_operations myfd_fops = {
	.owner = THIS_MODULE,
	.read = &myfd_read,
	.write = &myfd_write
};

static struct miscdevice myfd_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "reverse",
	.fops = &myfd_fops
};

static int __init myfd_init(void)
{
	return misc_register(&myfd_device);
}

static void __exit myfd_cleanup(void)
{
	misc_deregister(&myfd_device);
}

module_init(myfd_init);
module_exit(myfd_cleanup);

