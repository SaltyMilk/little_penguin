// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/mount.h>
#include <linux/kallsyms.h>
#include <linux/fs_struct.h>

MODULE_AUTHOR("hsmits <hsmits@student.codam.nl>");
MODULE_ALIAS("Iroh");
MODULE_LICENSE("GPL v2");

#define MOD_NAME "ft_mount"

static void *ct_seq_start(struct seq_file *s, loff_t *pos)
{
	if (*pos >= 1)
		return NULL;
	return (void*)0x1;
}

static void *ct_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	return NULL;
}

static void ct_seq_stop(struct seq_file *s, void *v)
{
}

static int		ct_show_mount(struct vfsmount *mount, void *data)
{
	struct super_block  *sb;
	struct path	    dev_path = { .mnt = mount,
					 .dentry = mount->mnt_root };

	sb = mount->mnt_sb;
	if (sb->s_op->show_devname) {
		sb->s_op->show_devname(data, mount->mnt_root);
	} else {
		seq_printf(data, "%s", sb->s_id);
	}
	seq_puts(data, "\t\t");
	seq_path(data, (const struct path *)&dev_path, "\t\n\b\r\a ");
	seq_putc(data, '\n');
	return (0);
}

static int ct_seq_show(struct seq_file *s, void *v)
{
	struct vfsmount	   *mounts;

	mounts = collect_mounts((const struct path *)&current->fs->root);
	if (IS_ERR(mounts))
		return -EPERM;
	return (iterate_mounts(&ct_show_mount, s, mounts));
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

static struct proc_dir_entry *mounts_file = NULL;

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
