#include <linux/module.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>

#define MIN(a,b) (a<b ? a:b)

static int major = 0;
static char kernelBuf[1024];
static struct class *hello_class;

//3.实现对应的 open/read/write函数，填写入结构体
ssize_t hello_drv_read (struct file *file, char __user *buf, size_t size, loff_t *pops)
{   
    int err;
    printk("%s %s line %d",__FILE__,__FUNCTION__,__LINE__);
    err = copy_to_user(buf, kernelBuf, MIN(size,1024));

    return MIN(size,1024);
}

ssize_t hello_drv_write (struct file *file,const char __user *buf, size_t size, loff_t *pops)
{
    int err;
    printk("%s %s line %d",__FILE__,__FUNCTION__,__LINE__);
    err = copy_from_user(kernelBuf, buf, MIN(size,1024));

    return MIN(size,1024);
}

int hello_drv_open (struct inode *inode, struct file *file)
{
    printk("%s %s line %d",__FILE__,__FUNCTION__,__LINE__);

    return 0;
}

int hello_drv_close (struct inode *inode, struct file *file)
{
    printk("%s %s line %d",__FILE__,__FUNCTION__,__LINE__);

    return 0;
}

//2.定义自己的 file_operations 结构体
static struct file_operations hello_drv = {
    .owner	 = THIS_MODULE,
    .read    = hello_drv_read,
    .write   = hello_drv_write,
    .open    = hello_drv_open,
    .release = hello_drv_close
};
//4.把file_operations 结构体告诉内核，注册驱动程序
//5.谁来注册驱动程序？需要一个入口函数；安装驱动程序时，就会调用这个入口函数

static int __init hello_drv_init(void)
{   
    int err;

    printk("%s %s line %d",__FILE__,__FUNCTION__,__LINE__);
    major = register_chrdev(0,"hello",&hello_drv);
    hello_class = class_create(THIS_MODULE, "hello");
	err = PTR_ERR(hello_class);
	if (IS_ERR(hello_class))
	{          
        printk("%s %s line %d",__FILE__,__FUNCTION__,__LINE__);
        unregister_chrdev(major,"hello");
        return -1;
    }

    device_create(hello_class, NULL, MKDEV(major, 0),NULL, "hello");
    
    return 0;
}


//6.卸载驱动程序，调用出口函数
static void __exit hello_drv_exit(void)
{   
    printk("%s %s line %d",__FILE__,__FUNCTION__,__LINE__);
    device_destroy(hello_class, MKDEV(major, 0));
    class_destroy(hello_class);
    unregister_chrdev(major,"hello");
}


//7.其他，提供设备信息，创建设备节点

module_init(hello_drv_init);
module_exit(hello_drv_exit);
MODULE_LICENSE("GPL");

