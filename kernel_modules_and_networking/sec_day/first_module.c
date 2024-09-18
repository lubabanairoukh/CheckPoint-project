#include <linux/module.h>    // Needed for all kernel modules
#include <linux/kernel.h>    // Needed for KERN_INFO
#include <linux/init.h>      // Needed for the macros

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaniv&Lubaba");
MODULE_DESCRIPTION("A Simple Kernel Module");

static int __init my_module_init(void) {
   
    printk(KERN_INFO "Basic Module: Loaded successfully!\n");
    
    return 0;  // Return 0 indicates successful loading
}

static void __exit my_module_exit(void) {
    printk(KERN_INFO "Basic Module: Unloaded successfully!\n");
}

module_init(my_module_init);
module_exit(my_module_exit);
