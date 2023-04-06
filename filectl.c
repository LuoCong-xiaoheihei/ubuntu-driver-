#include <linux/module.h>
#include <linux/fs.h>
#include <linux/major.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/stat.h>
#include <linux/tty.h>

static int major = 0; //确定主设备号，0默认系统分配
static struct class *filectl_class;
unsigned char kbuffer[1024]; // 内核空间的buffer
unsigned char ubuffer[1024]; // 申请一段空间用于存放用户数据

/*
  * FILECTL_READ 是和测试传递的参数是一样的才能匹配上 
  * FILECTL_WRITE 同样如此
  */

#define FILECTL_READ       _IOR('c', 0, unsigned int)   //对文件进行读取
#define FILECTL_WRITE      _IOW('c', 0, unsigned int)   //对文件进行写入


static long filectl_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int ret;
	printk("[%s %d]1111\n", __FUNCTION__, __LINE__);

	switch(cmd) {
		case FILECTL_READ:
            ret = copy_to_user((void __user *)arg, ubuffer, sizeof(ubuffer)); // 将内核空间的buffer拷贝至用户空间
            if(ret < 0) {
                printk("[%s %d]copy_to_user error\n ",__FUNCTION__, __LINE__);
			}
			printk("[%s %s %d]\n", __FILE__, __func__, __LINE__);
			break;
		case FILECTL_WRITE:
            ret = copy_from_user(ubuffer, (void __user *)arg, sizeof(ubuffer)); 
            if(ret < 0) {
                printk("[%s %d]copy_from_user error \n ",__FUNCTION__, __LINE__);
            }
			printk("[%s %s %d]\n", __FILE__, __func__, __LINE__);
			break;
	}
    return 0;
}

ssize_t filectl_read(struct file *file, char __user *buf, size_t size, loff_t *pops)
{
    int ret;
	printk("[%s %d]\n", __FUNCTION__, __LINE__);
    ret = copy_to_user(buf, kbuffer, size);
    if(ret < 0) {
        printk("[%s %d]copy_to_user error \n ",__FUNCTION__, __LINE__ );
    }
	printk("[%s %s %d]\n", __FILE__, __func__, __LINE__);
    return size;
}

ssize_t filectl_write(struct file *file,const char __user *buf, size_t size, loff_t *pops)
{
    int ret;
	printk("[%s %d]\n", __FUNCTION__, __LINE__);
    ret = copy_from_user(kbuffer, buf, size);
    if(ret < 0) {
        printk("[%s %d]copy_from_user error \n ",__FUNCTION__, __LINE__ );
    }
    return size;
}

int filectl_open(struct inode *inode, struct file *file)
{
	printk("[%s %d]\n", __FUNCTION__, __LINE__);
    return 0;
}

int filectl_close(struct inode *inode, struct file *file)
{
	printk("[%s %d]\n", __FUNCTION__, __LINE__);
    return 0;
}

// 定义自己的驱动的 file_operations
static struct file_operations filectl_ops = {
    .owner	 = THIS_MODULE,
    .read    = filectl_read,
    .write   = filectl_write,
    .open    = filectl_open,
    .release = filectl_close, // 好像没有close这个函数
    .unlocked_ioctl = filectl_ioctl, //添加ioclt操作函数
};

static int __init filectl_init(void)
{
    // 在初始化的时候进行驱动注册, 设备号, 设备名，核心操作函数
    major = register_chrdev(0,"filectl",&filectl_ops); 
    if(major < 0) {
        printk("[%s %d] filectl error\n", __FUNCTION__, __LINE__); // 注册失败
        return -1;
    }

    filectl_class = class_create(THIS_MODULE, "filectl"); // class_create 动态创建dev的类
    // IS_ERR 查看指针是否有错误
    if(IS_ERR(filectl_class)) {
        printk("[%s %d] class_create error\n", __FUNCTION__, __LINE__);
        unregister_chrdev(major,"filectl");
        return -1;
    }
	// 创建字符设备
    device_create(filectl_class, NULL, MKDEV(major, 0),NULL, "filectl");
    printk("[%s %d] filectl driver create success\n", __FUNCTION__, __LINE__);

    return 0;
}

static void __exit filectl_exit(void) {
    device_destroy(filectl_class, MKDEV(major, 0));
    class_destroy(filectl_class);
    // 注销字符设备
	unregister_chrdev(major,"filectl");
    printk("[%s %d]goodbye filectl driver\n",  __FUNCTION__, __LINE__);
}

module_init(filectl_init);
module_exit(filectl_exit);
MODULE_LICENSE		("GPL");
MODULE_AUTHOR		("cong.luo");
MODULE_DESCRIPTION	("First file contrl module");

