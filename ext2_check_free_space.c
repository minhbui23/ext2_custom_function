#include <linux/buffer_head.h>
#include "ext2.h"

unsigned long ext2_check_free_space(struct super_block *sb) {

    struct ext2_sb_info *sbi = EXT2_SB(sb);  
    unsigned long free_blocks = 0;          
    int i;

    
    for (i = 0; i < sbi->s_groups_count; i++) {
        struct ext2_group_desc *desc;
        struct buffer_head *bh;
        unsigned char *bitmap;
        int j;

        // Get group descriptor
        desc = ext2_get_group_desc(sb, i, NULL);
        if (!desc) {
            printk(KERN_ERR "ext2: Failed to get group descriptor for group %d\n", i);
            continue;
        }

        // Get block bitmap
        bh = sb_bread(sb, le32_to_cpu(desc->bg_block_bitmap));
        if (!bh) {
            printk(KERN_ERR "ext2: Failed to read block bitmap for group %d\n", i);
            continue;
        }

        bitmap = (unsigned char *)bh->b_data;

        // Count the number of free blocks (bit 0 in bitmap)
        for (j = 0; j < sb->s_blocksize; j++) {
            unsigned char byte = bitmap[j];
            for (int k = 0; k < 8; k++) {
                if (!(byte & (1 << k))) {
                    free_blocks++;
                }
            }
        }

        brelse(bh);  // Release buffer head
    }

    // Calculate free space in bytes
    unsigned long free_space = free_blocks * sb->s_blocksize;
    printk("Free block count: %lu\n", free_blocks);

    return free_space;
}

EXPORT_SYMBOL(ext2_check_free_space);