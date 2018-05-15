#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h> //struct file_operations
#include <linux/cdev.h> //struct cdev + 设备号
#include <linux/uaccess.h> //copy_to_user

//定义设备号
static dev_t dev;

//定义字符设备对象
static struct cdev led_cdev;

//调用关系：应用程序open->....->led_open
static int led_open(struct inode *inode, struct file *file)
{
   
    return 0; 
}

//调用关系：应用程序close->...->led_close
static int led_close(struct inode *inode, struct file *file)
{
   
    return 0; //执行成功返回0，执行失败返回负值
}

//调用关系：应用程序read->...->led_read
static ssize_t led_read(struct file *file,
                        char __user *buf,
                        size_t count,
                        loff_t *ppos)
{
    //定义初始化内核缓冲区(存储空间再后1G虚拟内存中)
    char kdata[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    //将内核数据上报给用户
    //切记：buf虽然保存的用户缓冲区的首地址，但不能直接访问
    //*(int *)buf = kdata;错误
    copy_to_user(buf, kdata, sizeof(kdata));
    printk("%s\n", __func__);
    return sizeof(kdata); //失败返回负值，成功返回实际读取的字节数
}

//定义初始化硬件操作方法
static struct file_operations led_fops =
{
    .owner = THIS_MODULE,
    .open = led_open, //打开设备
    .release = led_close, //关闭设备
    .read = led_read //读取设备
};

static int led_init(void)
{
    //申请设备号
    alloc_chrdev_region(&dev, 0, 1, "LEDREAD");

    //初始化字符设备对象
    cdev_init(&led_cdev, &led_fops);

    //注册字符设备对象到内核
    cdev_add(&led_cdev, dev, 1);

    
    return 0;
}

static void led_exit(void)
{
    
    //卸载字符设备对象
    cdev_del(&led_cdev);
    //释放设备号
    unregister_chrdev_region(dev, 1);
}
module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");

