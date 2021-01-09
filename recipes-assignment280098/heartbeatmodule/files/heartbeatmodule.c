#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include "data.h"

static dev_t mymod_dev;
struct cdev mymod_cdev;
static char buffer[64];
static char ppg_buffer[5];

ssize_t mymod_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	int status;
	sprintf(ppg_buffer, "%d", ppg[(int)*f_pos]);
	status = copy_to_user(buf, ppg_buffer, 5);
	if (status != 0)
		printk(KERN_INFO " [heartrate] error reading value from heartrate sensor\n" );
	else
		printk(KERN_INFO " [heartrate] read (val=%d count=%d, offset=%d)\n", ppg[(int)*f_pos], (int)count, (int)*f_pos );
	*f_pos+=1;
	if (*f_pos == 2048)
		*f_pos = 0;
	return count;
}

struct file_operations mymod_fops = {
	.owner = THIS_MODULE,
	.read = mymod_read,
};

static int __init mymod_module_init(void)
{
	printk(KERN_INFO "Loading heartrate_module\n");
	alloc_chrdev_region(&mymod_dev, 0, 1, "mymod_dev");
	printk(KERN_INFO "%s\n", format_dev_t(buffer, mymod_dev));
	cdev_init(&mymod_cdev, &mymod_fops);
	mymod_cdev.owner = THIS_MODULE;
	cdev_add(&mymod_cdev, mymod_dev, 1);
	return 0;
}

static void __exit mymod_module_cleanup(void)
{
	printk(KERN_INFO "Cleaning-up heartrate_module.\n");
	cdev_del(&mymod_cdev);
	unregister_chrdev_region(mymod_dev, 1);
}

module_init(mymod_module_init);
module_exit(mymod_module_cleanup);

MODULE_AUTHOR("Andrea Pignata");
MODULE_LICENSE("GPL");
