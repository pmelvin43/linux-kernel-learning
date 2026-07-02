/*
 * chardev,c: Creates a read-only char device that says how many times
 * you have read from the dev file
 *
 * comments and notes by Preston Melvin
 */

#include <linux/atomic.h> /* Atomic variables/operations, used for safe counters or state shared across contexts. */
#include <linux/cdev.h> /* Character device registration helpers: struct cdev, cdev_init(), cdev_add(), etc. */
#include <linux/delay.h> /* Sleep/delay helpers like msleep(), udelay(). Not always needed in simple drivers. */
#include <linux/fs.h> /* Core file/device structures: struct file, struct inode, struct file_operations. */
#include <linux/init.h>   /* __init, __exit, module_init(), module_exit(). */
#include <linux/kernel.h> /* General kernel helpers, including sprintf()/snprintf()-style helpers. */
#include <linux/module.h> /* Needed by all modules: MODULE_LICENSE, MODULE_AUTHOR, module metadata. */
#include <linux/printk.h>  /* pr_info(), pr_warn(), pr_err(), printk-style logging. */
#include <linux/types.h>   /* Kernel types like dev_t, size_t, ssize_t, loff_t, etc. */
#include <linux/uaccess.h> /* Safe copying between kernel space and userspace: copy_to_user(), copy_from_user(), get_user(), put_user(). */
#include <linux/version.h> /* Kernel version macros, useful for compatibility checks. */

#include <asm/errno.h> /* Error codes like -EINVAL, -EBUSY, -EFAULT. Often linux/errno.h is preferred in modern code. */

/* Prototypes - this would normally go in a .h file */
static int
device_open(struct inode *,
            struct file *); // inode, the metadata of the file, and the the struct file / filp
static int device_release(struct inode *, struct file *); // same
static ssize_t device_read(struct file *, char __user *, size_t,
                           loff_t *); // file struct, pointer to userspace, size, long offset, all
                                      // of signed size return type
static ssize_t device_write(struct file *, const char __user *, size_t, loff_t *);

#define DEVICE_NAME "chardev" /* Dev name as it appears in /proc/devices */
#define BUF_LEN 80            /* Max length of the message from the device */

/* Global variables are declared as static, so are global within the file. */

static int major; /* major number assigned to our device driver */

enum {
    CDEV_NOT_USED,
    CDEV_EXCLUSIVE_OPEN,
};

/* Is device open? Used to prevent multiple access to device */
static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED);

static char msg[BUF_LEN + 1]; /* The msg the device will give when asked, + 1 for null term */

static struct class *cls;

static struct file_operations chardev_fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
};

// chardev_init jobs
// 0. register the character driver
// 1. get a major number for the device
// 2. create a device class
// 3. create /dev/chardev
// 4. return 0 if setup worked

static int __init chardev_init(void)
{
    major = register_chrdev(0, DEVICE_NAME,
                            &chardev_fops); // register the cdd, give any available major number,
                                            // call it chardev, and use our fops we made

    if (major < 0) {
        pr_alert("Registering char device failed with %d\n", major);
        return major;
    }

    pr_info("I was assigned major number %d.\n", major);

#if LINUX_VERSION_CODE >=                                                                          \
    KERNEL_VERSION(6, 4, 0) // preproccessor conditional. if compiling for linux 6.4.0 or newer,
                            // class create takes one arg, otherwise take two. API changed.
    cls = class_create(DEVICE_NAME);
#else
    cls = class_create(THIS_MODULE, DEVICE_NAME);
#endif

    if (IS_ERR(cls)) // checks if pointer is actually an encoded error
    {
        pr_err("Failed to create class for device\n");
        unregister_chrdev(major, DEVICE_NAME);
        return PTR_ERR(cls); // returns it printed
    }

    device_create(cls, NULL, MKDEV(major, 0), NULL,
                  DEVICE_NAME); // create the actual device node / interface.

    pr_info("Device created on /dev/%s\n", DEVICE_NAME);

    return 0;
}

static void __exit chardev_exit(void)
{
    device_destroy(cls, MKDEV(major, 0));
    class_destroy(cls);

    /* Unregister the device */
    unregister_chrdev(major, DEVICE_NAME);
}

/* Methods */

/* Called when a process tries to open the device file, like
 * "sudo cat /dev/chardev"
 */
static int device_open(struct inode *inode, struct file *file)
{
    static int counter = 0;

    if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN)) {
        return -EBUSY;
    }

    sprintf(msg, "I already told you %d times Hello world!\n", counter++);

    return 0;
}

/* Called when a process closes the device file. */
static int device_release(struct inode *inode, struct file *file)
{
    /* We're now ready for our next caller */

    atomic_set(&already_open, CDEV_NOT_USED);

    return 0;
}

/* Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file *filp,   /* see include/linux/fs.h   */
                           char __user *buffer, /* buffer to fill with data */
                           size_t length,       /* length of the buffer     */
                           loff_t *offset)
{
    /* Number of bytes actually written to the buffer */
    int bytes_read = 0;
    const char *msg_ptr = msg;

    if (!*(msg_ptr + *offset)) { /* we are at the end of message */
        *offset = 0;             /* reset the offset */
        return 0;                /* signify end of file*/
    }

    msg_ptr += *offset;

    /* Actually put the data into the buffer */
    while (length && *msg_ptr) {
        /* The buffer is in the user data segment, not the kernel
         * segment so "*" assignment won't work.  We have to use
         * put_user which copies data from the kernel data segment to
         * the user data segment.
         */
        put_user(*(msg_ptr++), buffer++);
        length--;
        bytes_read++;
    }
    *offset += bytes_read;

    /* Most read functions return the number of bytes put into the buffer. */
    return bytes_read;
}

/* Called when a process writes to dev file: echo "hi" | sudo tee /dev/chardev */
static ssize_t device_write(struct file *filp, const char __user *buff, size_t len, loff_t *off)
{
    pr_alert("Sorry, this operation is not supported.\n");
    return -EINVAL;
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");