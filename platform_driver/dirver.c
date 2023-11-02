#include<linux/init.h>
#include<linux/module.h>
#include<linux/platform_device.h>

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
	}
};

static int __init hello_init(void)
{
	int ret;
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
	platform_driver_unregister(&platform_driver_hello);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE     ("GPL");
MODULE_AUTHOR      ("cong.luo");
MODULE_DESCRIPTION ("A hello module");
