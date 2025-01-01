#ifndef _EXT2_QUOTA_H
#define _EXT2_QUOTA_H

#include <linux/uidgid.h>
#include <linux/hashtable.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>

// Định nghĩa cấu trúc ext2_quota_info
struct ext2_quota_info {
    uid_t uid;                // ID người dùng
    unsigned long limit;      // Giới hạn dung lượng sử dụng
    unsigned long usage;      // Dung lượng đã sử dụng
    struct hlist_node node;   // Node cho bảng băm
};

#define QUOTA_HASH_BITS 8
extern DEFINE_HASHTABLE(quota_hash_table, QUOTA_HASH_BITS);

// Khai báo các hàm
void add_quota(uid_t uid, unsigned long limit);
void show_quota_table(void);
void cleanup_quota_table(void);
bool check_quota(uid_t uid, unsigned long size);
void update_quota(uid_t uid, unsigned long size);

// Khai báo kprobe và các handler
extern struct kprobe kp_write;
extern int write_pre_handler(struct kprobe *p, struct pt_regs *regs);
extern void write_post_handler(struct kprobe *p, struct pt_regs *regs, unsigned long flags);

extern struct kprobe kp_unlink;
extern void unlink_post_handler(struct kprobe *p, struct pt_regs *regs, unsigned long flags);


#endif // _EXT2_QUOTA_H
