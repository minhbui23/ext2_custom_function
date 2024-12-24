#include <linux/module.h>
#include <linux/fs.h>
#include <linux/list.h>
#include "ext2.h"
// External function declaration
extern unsigned long ext2_check_free_space(struct super_block *sb);

static int __init test_init(void)
{
    struct file_system_type *fs_type;
    struct super_block *sb;
    
    fs_type = get_fs_type("ext2");
    if (!fs_type) {
        printk(KERN_ERR "ext2 filesystem not found\n");
        return -ENODEV;
    }

    // Check if list is empty using hlist
    if (hlist_empty(&fs_type->fs_supers)) {
        printk(KERN_ERR "No mounted ext2 filesystems\n");
        return -ENODEV;
    }
    
    // Get first superblock using hlist
    sb = hlist_entry(fs_type->fs_supers.first, struct super_block, s_instances);
    
    printk(KERN_INFO "Free space: %lu\n", ext2_check_free_space(sb));

    module_put(THIS_MODULE);
    
    return 0;
}

static void __exit test_exit(void)
{
    printk(KERN_INFO "Test module unloaded\n");
}

module_init(test_init);
module_exit(test_exit);
MODULE_LICENSE("GPL");