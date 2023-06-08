# 使用教程

##### 一、进入目录

cd wfs/

##### 二、创建磁盘

truncate -s 5M diskimg

##### 三、编译代码

make

##### 四、初始化磁盘

./init_disk

##### 五、复制磁盘到/tmp

cp disking /tmp

##### 六、挂载磁盘

调试状态挂载则：./WFS -d -s tt该方式需另开终端

直接挂载./WFS tt

更多挂载方式请输入./WFS -h查看

