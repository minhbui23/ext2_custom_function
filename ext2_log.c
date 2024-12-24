#include <linux/fs.h>
#include <linux/time.h>
#include <linux/timekeeping.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/rtc.h>
#include <linux/slab.h>
#include "ext2.h"

void ext2_log(const char *path, const char *type, const char *action)
{
    char log_buf[256];
    struct tm time_tm;
    char time_str[32];
    struct timespec64 ts;
    struct file *file;
    loff_t pos = 0;
    ssize_t bytes_written;

    // Get current time
    ktime_get_real_ts64(&ts);
    time64_to_tm(ts.tv_sec, 0, &time_tm);

   //Format time
    snprintf(time_str, sizeof(time_str), "%02ld/%02ld-%02ld:%02ld",
             (long)(time_tm.tm_mday),
             (long)(time_tm.tm_mon + 1),
             (long)(time_tm.tm_hour),
             (long)(time_tm.tm_min));

    // Create log message
    snprintf(log_buf, sizeof(log_buf),
             "[%s] %s %s: %s\n",
             time_str,
             action,
             type,
             path);

    // Open the log file
    file = filp_open("/var/log/ext2_log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (IS_ERR(file)) {
        pr_err("ext2_log: Failed to open log file, error code: %ld\n", PTR_ERR(file));
        return;
    }

    bytes_written = kernel_write(file, log_buf, strlen(log_buf), &pos);
    if (bytes_written < 0) {
        pr_err("ext2_log: Failed to write to log file, error code: %ld\n", bytes_written);
    }

    // Close the log file
    filp_close(file, NULL);
}
