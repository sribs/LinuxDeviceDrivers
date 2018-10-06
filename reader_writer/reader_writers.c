#include<linux/module.h> // Linux Kernel module
#include<linux/kernel.h> // Linux Kernel libraries
#include<linux/fs.h> //file operations if any
#include<linux/cdev.h> //character driver initialization
#include<linux/kthread.h> // Linux Kernel thread function
//#include<linux/wait.h> libraries for wait_queue
#include<linux/device.h> // device and class creating without making use of mknod
#include<linux/semaphore.h>
#include<linux/uaccess.h>

#define DEVICE_NAME "reader_writer_device"
#define MAX_READERS 10

dev_t dev_num;
int ret;
struct cdev *mycdev;
static struct class *dev_class;
int reader_writer_flag = 0;

struct c_device {
    char dev_data[100];
    struct semaphore read_sem;
    struct semaphore write_sem;
} v_device;

ssize_t dev_read(struct file *filep, char *buff_data, size_t count, loff_t *offset)
{
    while(&v_device.write_sem.count<=0);
    if(v_device.read_sem.count <=0)
    {
        printk(KERN_ALERT "reader_writer: Maximum Readers reached");
        return -EBUSY;
    }
    if(down_interruptible(&v_device.read_sem)!=0)
    {
        printk(KERN_ALERT "reader_writer: Unable to Lock Reader semaphore while still not reached Maximum readers");
        return -EAGAIN;
    }
    printk(KERN_INFO "reader_writer: Reading from device");
    ret = copy_to_user(buff_data,v_device.dev_data,count);
    if(ret)
    {
        printk(KERN_ALERT "reader_writer: Unable to Read from Device");
        return -EFAULT;
    }
    up(&v_device.read_sem);
    return (ssize_t)ret;
}

ssize_t dev_write(struct file *filep, const char *buff_data, size_t count, loff_t *offset)
{
    while(v_device.read_sem.count<=0 || v_device.write_sem.count<=0);
    if(down_interruptible(&v_device.write_sem)!=0)
    {
        printk(KERN_ALERT "reader_writer: Unable to lock Writer Semaphore while both Reader and writer are free");
        return -EAGAIN;
    }
    printk(KERN_INFO "reader_writer: Locking and Writing to the Device");
    ret = copy_from_user(v_device.dev_data,buff_data,count);
    if(ret)
    {
        printk(KERN_ALERT "reader_writer: Unable to Write to the Device");
        return -EFAULT;
    }
    v_device.dev_data[count]='\0';
    up(&v_device.write_sem);
    return (ssize_t)ret;
}

int dev_open(struct inode *inode,struct file *filep)
{
    printk(KERN_INFO "reader_writer: Opened the device");
    return 0;
}

int dev_close(struct inode *inode, struct file *filep)
{
    printk(KERN_INFO "reader_writer: Closing the Device");
    return 0;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = dev_read,
    .open = dev_open,
    .release = dev_close
};

static int __init reader_writer_init(void)
{
    class_destroy(dev_class);
    ret = alloc_chrdev_region(&dev_num,0,1,DEVICE_NAME);
    if(ret<0)
    {
        printk(KERN_ALERT "reader_writer: Failed to allocate a Major Number");
        return ret;
    }
    printk(KERN_INFO " reader_writer: Suucessfully allocated Major Number: %d Minor Number: %d",MAJOR(dev_num),MINOR(dev_num));
    // Creating the Character device structure
    mycdev = cdev_alloc();
    mycdev->ops = &fops;
    mycdev->owner = THIS_MODULE;
    // Once the Device is created, we need to add to the kernel.
    ret = cdev_add(mycdev,dev_num,1);
    if(ret<0)
    {
        printk(KERN_ALERT "reader_writer: Unable to add cdev to the kernel");
        return ret;
    }

    if((dev_class = class_create(THIS_MODULE,"reader_writer_class") == NULL))
    {
        printk(KERN_ALERT "reader_writer: Unable to Create Class");
        unregister_chrdev_region(dev_num,1);
        return -EAGAIN;
    }
    if(device_create(dev_class,NULL,dev_num,NULL,DEVICE_NAME)==NULL)
    {
        printk(KERN_ALERT "reader_writer: Unable to Create Device");
        class_destroy(dev_class);
        unregister_chrdev_region(dev_num,1);
        return -EAGAIN;
    }
    sema_init(&v_device.read_sem,MAX_READERS);
    sema_init(&v_device.write_sem,1);
    return 0;
}

static void __exit reader_writer_exit(void)
{
    device_destroy(dev_class,dev_num);
    class_destroy(dev_class);
    cdev_del(mycdev);
    unregister_chrdev_region(dev_num,1);
    printk(KERN_INFO "reader_writer: Device Driver Exitting");
}

module_init(reader_writer_init);
module_exit(reader_writer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sriharsha B S");
MODULE_DESCRIPTION("Readers Writer Problem Implementation");