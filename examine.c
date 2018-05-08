#include <linux/module.h>  
#include <linux/kernel.h>  
#include <linux/fs.h> 
#include <linux/init.h>
#include <linux/cdev.h>  
#include <linux/sched.h>  //jiffies在此头文件中定义
#include <linux/timer.h>
struct timer_list mytimer;//定义定时器
 
void mytimer_fun(unsigned long arg)
{
	printk("abcdefghijlmnopqrstuvwxyz");
	printk("123456789012345678901234567890");
	add_timer(&mytimer);
}

static int txt_init(void)
{
	init_timer(&mytimer);
	mytimer.expires = jiffies + 300;
	mytimer.function = mytimer_fun;
	add_timer(&mytimer);
	
	return 0;
}
  
static void txt_exit(void)  
{   
	del_timer(&mytimer); 
}  
  
module_init(txt_init);  
module_exit(txt_exit);  
  
MODULE_LICENSE("GPL");   

