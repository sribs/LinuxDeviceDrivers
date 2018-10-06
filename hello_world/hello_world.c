#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>

static int __init hello_world_init(void)
{
    printk(KERN_INFO "hello_world: This is Kernel Information");
    printk(KERN_ALERT "hello_world: This is Kernel Alert");
    printk(KERN_ERR "hello_word: This is Kernel Error");
    printk(KERN_DEBUG "hello_word: This is Kernel Debug");

    return 0;
}

void __exit hello_world_exit(void)
{
    printk(KERN_ALERT "hello_world: This is a clean up module");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_AUTHOR("Sriharsha B S");
MODULE_DESCRIPTION("Hello World Module");
MODULE_LICENSE("GPL");