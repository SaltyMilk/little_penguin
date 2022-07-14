// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/mount.h>
#include <linux/kallsyms.h>
#include <linux/fs_struct.h>


MODULE_AUTHOR("sel-melc");
MODULE_ALIAS("Iroh");
MODULE_LICENSE("GPL v2");

#define MOD_NAME "mymounts"


static void *ct_seq_start(struct seq_file *s, loff_t *pos)
{
	if (*pos >= 1)
		return NULL;
	return (void *)0x1;
}

static void *ct_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	return NULL;
}

static void ct_seq_stop(struct seq_file *s, void *v)
{
}

static int ct_seq_show(struct seq_file *s, void *v)
{
	struct dentry *curdentry;

	seq_puts(s, "root\t/\n");
	list_for_each_entry(curdentry, &current->fs->root.mnt->mnt_root->d_subdirs, d_child) {
		if (curdentry->d_flags & DCACHE_MOUNTED)
			seq_printf(s, "%s\t/%s\n", curdentry->d_name.name, curdentry->d_name.name);
	}

	return 0;
}

static const struct seq_operations  ct_seq_ops = {
	.start = ct_seq_start,
	.next = ct_seq_next,
	.stop = ct_seq_stop,
	.show = ct_seq_show,
};

static int  mymounts_open(struct inode *inode, struct file *file)
{
	struct path	   *root;

	root = __seq_open_private(file, &ct_seq_ops, sizeof(struct path));
	if (IS_ERR(root))
		return -ENOMEM;
	*root = current->fs->root;
	return 0;
}

static const struct proc_ops pops = {
	.proc_open = &mymounts_open,
	.proc_release = &seq_release_private,
	.proc_read = &seq_read,
	.proc_lseek = seq_lseek,
};

static struct proc_dir_entry *mounts_file;

static int __init init(void)
{
	mounts_file = proc_create(MOD_NAME, 0444, NULL, &pops);
	if (mounts_file == NULL) {
		printk(KERN_WARNING MOD_NAME ": Failed to create '" MOD_NAME "' /proc file");
		return -EPERM;
	}
	return (0);
}

static void __exit cleanup(void)
{
	printk(KERN_INFO MOD_NAME ": Cleaning up module.\n");
	proc_remove(mounts_file);
}

module_init(init);
module_exit(cleanup);
