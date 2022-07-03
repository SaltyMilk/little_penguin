// SPDX-License-Identifier: GPL-2.0+
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hid.h>
#include <linux/usb.h>

MODULE_AUTHOR("IROH");
MODULE_DESCRIPTION("Print a message to the kernel log once a USB device is attached");
MODULE_LICENSE("GPL");

static const struct usb_device_id mod_table[2] = {
	{ USB_INTERFACE_INFO(
			USB_INTERFACE_CLASS_HID,
			USB_INTERFACE_SUBCLASS_BOOT,
			USB_INTERFACE_PROTOCOL_KEYBOARD) },
	{ }
};

MODULE_DEVICE_TABLE(usb, mod_table);

static int __init main_init(void) {
	printk(KERN_INFO "A USB keyboard has been attached\n");
	return 0;
}

static void __exit main_clean(void) {
	printk(KERN_INFO "Cleaning up the module\n");
}

module_init(main_init);
module_exit(main_clean);

