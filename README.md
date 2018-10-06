# LinuxDeviceDrivers
Linux Device Drivers Programming - Concepts and Examples

# Introduction
Device drivers take on a special role in the Linux kernel. They are distinct “blackboxes” that make a particular piece of hardware respond to a well-defined internal programming interface; they hide completely the details of how the device works. User activities are performed by means of a set of standardized calls that are independent of the specific driver; mapping those calls to device-specific operations that act on real hardware is then the role of the device driver. This programming interface is such that drivers can be built separately from the rest of the kernel and “plugged in” at runtime when needed. This modularity makes Linux drivers easy to write, to the point that there are now hundreds of them available.

## Modularity
A simple way to understand modularity is to treat every Device Driver code as a module which can be loaded on-demand. Let us now look how we load the Linux Driver to the Operating System.

``` bash
insmod /path/to/driver_name.ko # This is to insert the module on to the Linux Kernel.
rmmod /pth/to/driver_name.ko # This is to remove the module that is currently running.
modinfo /path/to/driver_name.ko # Obtain Information about the current module.
lsmod # Gets the list of modules loaded to the Linux Kernel - /proc/modules
```

## Table of Contents
1. [Hello World to our Linux Drivers](https://github.com/sribs/LinuxDeviceDrivers/tree/master/hello_world)
2. [Advancing from the Hello World - Passing parameter to our Module](https://github.com/sribs/LinuxDeviceDrivers/tree/master/module_param)
3. [Character Drivers - A Simple Virtual Driver](https://github.com/sribs/LinuxDeviceDrivers/tree/master/char_driver)
4. [Advancing the Character Driver - Process Synchronization - Reader Writer Problem](https://github.com/sribs/LinuxDeviceDrivers/tree/master/reader_writer)
5. [IOCTL - Input Output Control](https://github.com/sribs/LinuxDeviceDrivers/tree/master/ioctl)
6. [Procfs - Process File System](https://github.com/sribs/LinuxDeviceDrivers/tree/master/procfs)
7. [Wait queue](https://github.com/sribs/LinuxDeviceDrivers/tree/master/wait_queue)
8. [Sysfs](https://github.com/sribs/LinuxDeviceDrivers/tree/master/sysfs)
9. [Interrupts](https://github.com/sribs/LinuxDeviceDrivers/tree/master/interrupts)
10. [Work Queue](https://github.com/sribs/LinuxDeviceDrivers/tree/master/work_queue)
11. [Linked List](https://github.com/sribs/LinuxDeviceDrivers/tree/master/linker_list)
12. [Kernel Thread](https://github.com/sribs/LinuxDeviceDrivers/tree/master/kernel_thread)
13. [Mutex](https://github.com/sribs/LinuxDeviceDrivers/tree/master/mutex)
14. [Spinlock](https://github.com/sribs/LinuxDeviceDrivers/tree/master/spinlocks)