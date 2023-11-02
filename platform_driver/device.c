#include<linux/init.h>
#include<linux/module.h>
/*The header of driver register,include struct ,register function and unregister function about driver*/
#include<linux/platform_device.h>

static void hello_release(struct device *dev)
{
	printk("hello_module release\n");
}

struct platform_device platform_device_hello_module = {
	.name = "hello_device",
	.id   = -1,
       .dev   = {
       		.release=hello_release,
 	},
};

static int __init hello_init(void)
{
	platform_device_register(&platform_device_hello_module);
	return 0;
}
static void __exit hello_exit(void)
{
	platform_device_unregister(&platform_device_hello_module);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE     ("GPL");
MODULE_AUTHOR      ("cong.luo");
MODULE_DESCRIPTION ("A test module");
