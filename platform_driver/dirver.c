#include<linux/init.h>
#include<linux/module.h>
#include<linux/platform_device.h>

// static void hello_release(struct device *dev)
// {
// 	printk(KERN_EMERG"hello_release start!\n");
// }
// /***********************************/
// struct platform_device platform_device_hello = {
// 	.name = "hello_device",
// 	.id = -1,
// 	.dev = {
// 		.release = hello_release,
// 	}
// };

/**************************************/
/**************************************/
static int hello_probe(struct platform_device *pdev)
{
	printk("pdev->name is %s!\n",pdev->name);
	
	pdev->dev.release(&pdev->dev);

	return 0;
}
static int hello_remove(struct platform_device *pdev)
{
	printk(KERN_EMERG"%s's driver is removed!\n",pdev->name);
	return 0;
}

/*static const struct of_device_id of_device_dt_match[] = {
	{.compatible = DRIVER_NAME},
	{},
};
MODULE_DEVICE_TABLE(of,of_leds_dt_match);
用于设备树的匹配*/
/***************************************/
struct platform_driver platform_driver_hello = {
	.probe = hello_probe,
	.remove = hello_remove,
	.driver = {
		.name = "hello_device",
		//.of_match_table = of_device_dt_match,用于设备树的匹配
	}
};

static int __init hello_init(void)
{
	int ret;
	// //device register
	// ret = platform_device_register(&platform_device_hello);
	// if(ret)
	// {
	// 	printk(KERN_EMERG"platform device register failed!\n");
	// 	return ret;
	// }

	//driver register
	ret = platform_driver_register(&platform_driver_hello);
	if(ret)
	{
		printk(KERN_EMERG"platform driver register failed!\n");
		return ret;
	}	
        
	return ret;
}

static void __exit hello_exit(void)
{
	//release driver
	platform_driver_unregister(&platform_driver_hello);

	//release device
	// platform_device_unregister(&platform_device_hello);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE     ("GPL");
MODULE_AUTHOR      ("cong.luo");
MODULE_DESCRIPTION ("A hello module");
