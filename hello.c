#include <linux/module.h>
#include <linux/init.h>


static int hello_init(void) {
	printk(KERN_DEBUG "hello world\n");
	return 0;
}

static void hello_exit(void) {
	printk(KERN_DEBUG"goodbye my world\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE     ("GPL");
MODULE_AUTHOR      ("cong.luo");
MODULE_DESCRIPTION ("A test module");


