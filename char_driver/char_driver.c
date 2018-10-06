#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/semaphore.h>
#include<linux/uaccess.h> //copy_to_user and copy_from_user

#define DEVICE_NAME "mydevice"

struct some_device {
    char data[100];
    struct semaphore sema;
} virtual_device;

struct cdev *vdevice;
int major_number;
int ret;

dev_t dev_num;

int device_open(struct inode *inode, struct file *filep)
{
    if(down_interruptible(&virtual_device.sema)!=0)
    {
        printk(KERN_ALERT "char_driver: Could not lock device during open");
        return -1;
    }
    printk(KERN_INFO "char_driver: Successfully opened the device");
    return 0;
}

int device_release(struct inode *inode, struct file *filep)
{
    // release the semaphore
    up(&virtual_device.sema);
    printk(KERN_INFO "char_driver: Closed the device");
    return 0;
}

static ssize_t device_read(struct file *filp,char *buff_data,size_t buff_count, loff_t *offset)
{
    unsigned int ret;
    if(buff_count > sizeof(virtual_device.data)-1)
        return -EINVAL;
    ret = copy_to_user(buff_data,virtual_device.data,buff_count);
    if(ret)
        return -EFAULT;
    return buff_count;
}

static ssize_t device_write(struct file *filp,const char *buf,size_t count,loff_t *offset)
{
unsigned int ret;
if(count > sizeof(virtual_device.data) - 1)
    return -EINVAL;
ret = copy_from_user(virtual_device.data,buf,count);
if(ret)
    return -EFAULT;
virtual_device.data[count] = '\0';
printk(KERN_ALERT"the ptr:: %p",offset);
printk(KERN_ALERT"the ptr after::%p",offset);
return count;
}
struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
    .write = device_write,
    .read = device_read
};


static int __init char_driver_init(void)
{
    ret = alloc_chrdev_region(&dev_num,0,1,DEVICE_NAME);
    if(ret<0)
    {
        printk(KERN_ALERT "char_driver: Failed to allocate a Major Number");
        return ret;
    }
    major_number = MAJOR(dev_num);
    printk(KERN_INFO " char_driver: Suucessfully allocated Major Number: %d",major_number);
    // Creating the Character device structure
    vdevice = cdev_alloc();
    vdevice->ops = &fops;
    vdevice->owner = THIS_MODULE;
    // Once the Device is created, we need to add to the kernel.
    ret = cdev_add(vdevice,dev_num,1);
    if(ret<0)
    {
        printk(KERN_ALERT "char_driver: Unable to add cdev to the kernel");
        return ret;
    }
    sema_init(&virtual_device.sema,1);
    return 0;
}


static void __exit char_driver_exit(void)
{
    //Unregister everything in the reverse order
    cdev_del(vdevice);
    //Deallocate from chardev_region
    unregister_chrdev_region(dev_num,1);
    printk(KERN_ALERT "char_driver: Unloading the Character Driver module");
}


module_init(char_driver_init);
module_exit(char_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sriharsha B S");
