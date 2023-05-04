#include <linux/module.h>
#include <linux/fs.h>
#include <linux/major.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/stat.h>
#include <linux/tty.h>

static int major = 0; //确定主设备号，0默认系统分配
static struct class *filectl_class;

static struct platform_dev{
	char name[20];
	int number;
	int state;
};

static struct platform_dev chardev_1 = {
	.name = "dev_1",
	.number = 1,
	.state = 1
};

ssize_t filectl_read(struct file *file, char __user *buf, size_t size, loff_t *pops)
{
	struct platform_dev *pdev = file->private_data; // 引用私有数据
	printk("[%s %d] %s %d %d\n", __FUNCTION__, __LINE__, pdev->name, pdev->number, pdev->state);
    return size;
}

ssize_t filectl_write(struct file *file,const char __user *buf, size_t size, loff_t *pops)
{
	struct platform_dev *pdev = file->private_data;
	int ret;
	ret = copy_from_user(pdev->name, buf, size); // 将用户空间的buf直接传递到内核空间中去
    if(ret < 0) {
        printk("[%s %d]copy_from_user error \n ",__FUNCTION__, __LINE__);
    }

	printk("[%s %d] %s\n", __FUNCTION__, __LINE__, pdev->name);
    return size;
}

int filectl_open(struct inode *inode, struct file *file)
{
    printk("[%s %d]\n", __FUNCTION__, __LINE__);
	file->private_data =  &chardev_1; //定义私有数据范围
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
