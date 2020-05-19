#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <trace/events/sched.h>

#define VOLTAGE_PATH "/sys/class/power_supply/battery/voltage_now"
#define FREQUENCY_PATH "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq"

#define CREATE_TRACE_POINTS
#include "cpufrqvol-trace.h"

struct file *file_open(const char *path, int flags, int rights) 
{
    struct file *filp = NULL;
    mm_segment_t oldfs;
    int err = 0;

    oldfs = get_fs();
    set_fs(KERNEL_DS);
    filp = filp_open(path, flags, rights);
    set_fs(oldfs);	
    if (IS_ERR(filp)) {
        err = PTR_ERR(filp);
        return NULL;
    }
    return filp;
}

void file_close(struct file *file) 
{
    filp_close(file, NULL);
}

int file_read(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size) 
{
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(KERNEL_DS);

    ret = vfs_read(file, data, size, &offset);

    set_fs(oldfs);
    return ret;
}

int file_write(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size) 
{
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(KERNEL_DS);

    ret = vfs_write(file, data, size, &offset);

    set_fs(oldfs);
    return ret;
}

int snprintf(char *buf, size_t size, const char *fmt, ...)
{
    va_list args;
    int ret;

    va_start(args, fmt);
    ret = vsnprintf(buf, size, fmt, args);
    va_end(args);

    return ret;
}

static int voltage(void)
{
    int voltage = 0;
    struct file *voltage_now = file_open(VOLTAGE_PATH, O_RDONLY, 0644);
    if(voltage_now == NULL) {
        printk("File %s doesn't exisist\n", VOLTAGE_PATH);
        return 0;
    }
    char buf[64];
    int ret = file_read(voltage_now, 0, buf, sizeof buf-1);
    if(ret <=  0) {
        printk("Can't read %s\n", voltage_now_path);
        return 0;
    }
    buf[ret] = '\0';
    sscanf(buf, "%d", &voltage);
    file_close(voltage_now);
    /* µV to V              */
    /* voltage / 1000000.0  */
    return voltage;
} 

static int frequency(void)
{
    int frequency = 0;
    struct file *frequency_now = file_open(FREQUENCY_PATH, O_RDONLY, 0644);
    if(frequency_now == NULL) {
        printk("File %s doesn't exisist\n", FREQUENCY_PATH);
        return 0;
    }
    char buf[64];
    int ret = file_read(frequency_now, 0, buf, sizeof buf-1);
    if(ret <=  0) {
        printk("Can't read %s\n", frequency_now_path);
        return 0;
    }
    buf[ret] = '\0';
    sscanf(buf, "%d", &frequency);
    file_close(frequency_now);
    /* Hz to MHz                */
    /* frequency / 1000000.0    */
    return frequency ;
}

static int f = 0;
static int v = 0;

static int silly_thread(void *arg)
{
	while (!kthread_should_stop()) {
        set_current_state(TASK_INTERRUPTIBLE);
	    schedule_timeout(20*HZ);
        if(trace_get_stats_enabled()) {
            v = voltage();
            f = frequency();
            trace_get_stats(v,f);
        }
    }

	return 0;
}

static struct task_struct *silly_tsk;

static int __init stats_init(void)
{
    printk("Stats module succefully started.");
    silly_tsk = kthread_run(silly_thread, NULL, "silly-thread");
	if (IS_ERR(silly_tsk))
		return -1;
   	return 0;
}

static void __exit stats_exit(void)
{   
    kthread_stop(silly_tsk);
    printk("Stats module succefully finished.");
}

module_init(stats_init);
module_exit(stats_exit);

MODULE_AUTHOR("Dmitriy Butakov");
MODULE_LICENSE("GPL");