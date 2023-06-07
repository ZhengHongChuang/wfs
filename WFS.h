#include <stddef.h>
#define FS_BLOCK_SIZE 512
#define SUPER_BLOCK 1
#define BITMAP_BLOCK 1280
#define ROOT_DIR_BLOCK 1
#define MAX_DATA_IN_BLOCK 504
#define MAX_DIR_IN_BLOCK 8
#define MAX_FILENAME 8
#define MAX_EXTENSION 3
long TOTAL_BLOCK_NUM;

//超级块中记录的，大小为 24 bytes（3个long），占用1块磁盘块
struct super_block {
    long fs_size; //size of file system, in blocks（以块为单位）
    long first_blk; //first block of root directory（根目录的起始块位置，以块为单位）
    long bitmap; //size of bitmap, in blocks（以块为单位）
};

//记录文件信息的数据结构,统一存放在目录文件里面，也就是说目录文件里面存的全部都是这个结构，大小为 40 bytes，占用1块磁盘块
struct file_directory {
    char fname[MAX_FILENAME + 1]; //文件名 (plus space for nul)
    char fext[MAX_EXTENSION + 1]; //扩展名 (plus space for nul)
    time_t atime; 	/* 上次访问时间 */
    time_t mtime;	/*上次修改时间 */
    // time_t ctime; 	/* 上次文件状态改变时间 */
    int uid;        //
    int mode;       //
    size_t fsize;   //文件大小（file size）
    long nStartBlock; //目录开始块位置（where the first block is on disk）
    int flag; //indicate type of file. 0:for unused; 1:for file; 2:for directory
};

//文件内容存放用到的数据结构，大小为 512 bytes，占用1块磁盘块
struct data_block {
    size_t size; //文件使用了这个块里面的多少Bytes
    long nNextBlock; //（该文件太大了，一块装不下，所以要有下一块的地址）   long的大小为4Byte
    char data[MAX_DATA_IN_BLOCK];// And all the rest of the space in the block can be used for actual data storage.
};
//磁盘存放路径，程序运行时请自行修改对应路径
char *disk_path="/home/zhc/wfs/diskimg";
//将后者file_directory *b中字段的内容赋值给struct file_directory *a
void read_cpy_file_dir(struct file_directory *a,struct file_directory *b);
//打开文件，将FILE指针移动到文件的第blk_no块位置读取该块数据给data_block *data_blk
int read_cpy_data_block(long blk_no,struct data_block *data_blk);
//打开文件，将FILE指针移动到文件的第blk_no块位置将data_block *data_blk数据写入
int write_data_block(long blk_no,struct data_block *data_blk);
//分割路径获取文件/目录的名称和后缀名以及父目录的起始块和file_directory
int divide_path(char *name, char *ext, const char *path, long *par_dir_stblk, int flag, int *par_size);
//找寻该目录下是否已经存在相同的struct file_directory *file_dir
int exist_check(struct file_directory *file_dir, char *p, char *q, int *offset, int *pos, int size, int flag);
//设置文件的第long start_blk开为已经使用块
int set_blk_use(long start_blk,int flag);
//const char* path找寻该路径是否已经存在文件或目录
int path_is_emp(const char* path);
// 将file_directory的数据赋值给path相应文件或目录的file_directory
int setattr(const char* path, struct file_directory* attr, int flag);
//从next_blk起清空data_blk后续块
void ClearBlocks(long next_blk, struct data_block* data_blk);
//采用首次适应法找寻文件中符合的第一片大小大于num的连续区域，返回找到的最多空闲块（不一定为num）
int get_empty_blk(int num, long* start_blk);
//数据存取不够时扩展一个新块
int increase_blk(long par_dir_stblk, struct file_directory *file_dir,struct data_block *data_blk, long *tmp, char*p, char*q, int flag);
//设置struct file_directory *file_dir中的时间，uid，权限等字段
void init_file_data(struct file_directory *file_dir, char *m, char *n, int flag);
//根据文件的路径，到相应的目录寻找该文件的file_directory，并赋值给attr
int get_fd_to_attr(const char * path,struct file_directory *attr);
//创建path所指的文件或目录的file_directory，并为该文件（目录）申请空闲块，创建成功返回0，创建失败返回-1
int create_file_dir(const char* path, int flag);
//删除path所指的文件或目录的file_directory和文件的数据块，成功返回0，失败返回-1
int remove_file_dir(const char *path, int flag);



// int fuse_session_loop();
