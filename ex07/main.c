// SPDX-License-Identifier: GPL-2.0+
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/device.h>
#include <linux/usb.h>
#include <linux/mod_devicetable.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/debugfs.h>

MODULE_AUTHOR("IROH");
MODULE_DESCRIPTION("A simple login module");
MODULE_LICENSE("GPL");

#define INTRA_LOGIN "hsmits"

static struct dentry *dfs_dir;
static struct dentry *id_file;
static struct dentry *jiffies_file;

static ssize_t id_read(struct file *file, char __user *buff, size_t size, loff_t *_offset)
{
	return simple_read_from_buffer(buff, size, _offset, INTRA_LOGIN, sizeof(INTRA_LOGIN) - 1);
}

static ssize_t jiffies_read(struct file *file, char __user *buff, size_t size, loff_t *_offset)
{
	char buffer[64];

	memset(buffer, 0, 64);
	snprintf(buffer, 64, "%lu", jiffies);
	return (simple_read_from_buffer(buff, size, _offset, buffer, 64));
}

static ssize_t id_write(struct file *file, const char __user *buff, size_t size, loff_t *_offset)
{
	if (size != sizeof(INTRA_LOGIN) - 1)
		return -EINVAL;
	if (memcmp(buff, INTRA_LOGIN, sizeof(INTRA_LOGIN) - 1))
		return -EINVAL;
	return sizeof(INTRA_LOGIN) - 1;
}

static struct file_operations id_fops = {

	.read = &id_read,
	.write = &id_write,
};

static struct file_operations jiffies_fops = {

	.read = &jiffies_read,
};

static int __init init(void)
{
	printk(KERN_INFO "Module init.\n");

	dfs_dir = debugfs_create_dir("fortytwo", NULL);
	id_file = debugfs_create_file("id", 0777, dfs_dir, NULL, &id_fops);
	jiffies_file = debugfs_create_file("jiffies", 0444, dfs_dir, NULL, &jiffies_fops);
	return (0);

}

static void __exit cleanup(void)
{
	printk(KERN_INFO "Module cleanup.\n");
	debugfs_remove(id_file);
	debugfs_remove(jiffies_file);
	debugfs_remove(dfs_dir);
}


module_init(init);
module_exit(cleanup);
