#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define PROCESS_NAME_LEN 32   // 进程名字长度
#define MIN_SLICE 10          // 最小碎片大小
#define DEFAULT_MEM_SIZE 1024 // 默认的内存大小
#define DEFAULT_MEM_START 0   // 默认起始地址

#define MA_FF 1 // 首次适应算法
#define MA_BF 2 // 最佳适应算法
#define MA_WF 3 // 最坏适应算法

// 空闲分区的结构体
typedef struct freeBlockType
{
    int size;
    int start_addr;
    struct freeBlockType *next;
} freeBlockType;
freeBlockType *freeBlock;

// 已分配分区的结构体
typedef struct allocatedBlock
{
    int pid;
    int size;
    int start_addr;
    char process_name[PROCESS_NAME_LEN];
    struct allocatedBlock *next;
} allocatedBlock;

struct allocatedBlock *allocatedBlockHead = NULL;

int memSize = DEFAULT_MEM_SIZE;
int ma_algorithm = MA_FF;
static int pid = 0;
int flag = 0;

// 函数声明
void displayMenu();
int setMemSize();
void set_algorithm();
void rearrange(int algorithm);
int new_process();
int allocate_mem(struct allocatedBlock *ab);
void kill_process();
int free_mem(struct allocatedBlock *ab);
int dispose(struct allocatedBlock *free_ab);
int display_mem_usage();
allocatedBlock *find_process(int pid);
void rearrange_FF();
void rearrange_BF();
void rearrange_WF();

// 初始化空闲分区
freeBlockType *initFreeBlock(int mem_size)
{
    freeBlockType *fb;
    fb = (freeBlockType *)malloc(sizeof(freeBlockType));
    if (fb == NULL)
    {
        printf("没有内存 !\n");
        return NULL;
    }
    fb->size = mem_size;
    fb->start_addr = DEFAULT_MEM_START;
    fb->next = NULL;
    return fb;
}

// 显示主菜单
void displayMenu()
{
    printf("\n");
    // printf("1 - Set memory size (default=%d)\n", DEFAULT_MEM_SIZE);
    // printf("2 - Select memory allocation algorithm\n");
    // printf("3 - New process \n");
    // printf("4 - Terminate a process \n");
    // printf("5 - Display memory usage \n");
    // printf("0 - Exit\n");

    printf("1 - 设置内存大小 ( 默认 = %d )\n", DEFAULT_MEM_SIZE);
    printf("2 - 选择内存分配算法 \n");
    printf("3 - 开启新进程 \n");
    printf("4 - 终止一个进程 \n");
    printf("5 - 显示内存使用率 \n");
    printf("0 - 退出 \n");
    printf("input : ");
}

// 设置总内存大小
int setMemSize()
{
    int size;
    if (flag != 0)
    { // flag标志防止内存被再次设置
        printf("不能再次设置内存大小 !\n");
        return 0;
    }
    printf("设置总内存大小 = ");
    scanf("%d", &size);
    if (size > 0)
    {
        memSize = size;
        freeBlock->size = memSize; // 设置初始大小为 1024
    }
    flag = 1;
    system("cls");
    return 1;
}
// 选择当前算法
void set_algorithm()
{
    int algorithm;
    printf("\t1 - First Fit\n");
    printf("\t2 - Best Fit \n");
    printf("\t3 - Worst Fit \n");
    printf("Please input your choice : ");
    scanf("%d", &algorithm);
    if (algorithm >= 1 && algorithm <= 3)
        ma_algorithm = algorithm;

    rearrange(ma_algorithm);
}

// 为每一个进程分配完内存以后重新按已选择的算法再次排序
void rearrange(int algorithm)
{
    switch (algorithm)
    {
    case MA_FF:
        rearrange_FF();
        break;
    case MA_BF:
        rearrange_BF();
        break;
    case MA_WF:
        rearrange_WF();
        break;
    }
}
// 首次适应算法，按地址的大小由小到达排序
void rearrange_FF()
{
    freeBlockType *temp, *p = NULL;
    freeBlockType *head = NULL;
    int current_min_addr;

    if (freeBlock)
    {
        temp = freeBlock;
        current_min_addr = freeBlock->start_addr;
        while (temp->next != NULL)
        {
            if (temp->next->start_addr < current_min_addr)
            {
                current_min_addr = temp->next->start_addr;
                p = temp;
            }
            temp = temp->next;
        }
        if (p != NULL)
        {
            temp = p->next;
            p->next = p->next->next;
            temp->next = freeBlock;
            freeBlock = temp;
        }
        head = freeBlock;
        p = head;
        temp = head->next;
        while (head->next != NULL)
        {
            current_min_addr = head->next->start_addr;
            while (temp->next != NULL)
            {
                if (temp->next->start_addr < current_min_addr)
                {
                    current_min_addr = temp->next->start_addr;
                    p = temp;
                }
                temp = temp->next;
            }
            if (p->next != head->next)
            {
                temp = p->next;
                p->next = p->next->next;
                temp->next = head->next;
                head->next = temp;
            }
            head = head->next;
            temp = head->next;
            p = head;
        }
    }
    return;
}

// 最佳适应算法，按内存块的大小由小到大排序
void rearrange_BF()
{
    freeBlockType *temp, *p = NULL;
    freeBlockType *head = NULL;
    int current_min_size = freeBlock->size;

    temp = freeBlock;
    while (temp->next != NULL)
    {
        if (temp->next->size < current_min_size)
        {
            current_min_size = temp->next->size;
            p = temp;
        }
        temp = temp->next;
    }
    if (p != NULL)
    {
        temp = p->next;
        p->next = p->next->next;
        temp->next = freeBlock;
        freeBlock = temp;
    }
    head = freeBlock;
    p = head;
    temp = head->next;
    while (head->next != NULL)
    {
        current_min_size = head->next->size;
        while (temp->next != NULL)
        {
            if (temp->next->size < current_min_size)
            {
                current_min_size = temp->next->size;
                p = temp;
            }
            temp = temp->next;
        }
        if (p->next != head->next)
        {
            temp = p;
            p->next = p->next->next;
            temp->next = head->next;
            head->next = temp;
        }
        head = head->next;
        temp = head->next;
        p = head;
    }
}

// 最坏适应算法，按地址块的大小从大到小排序
void rearrange_WF()
{
    freeBlockType *temp, *p = NULL;
    freeBlockType *head = NULL;
    int current_max_size = freeBlock->size;
    temp = freeBlock;
    while (temp->next != NULL)
    {
        if (temp->next->size > current_max_size)
        {
            current_max_size = temp->next->size;
            p = temp;
        }
        temp = temp->next;
    }
    if (p != NULL)
    {
        temp = p;
        p->next = p->next->next;
        temp->next = freeBlock;
        freeBlock = temp;
    }
    head = freeBlock;
    p = head;
    temp = head->next;
    while (head->next != NULL)
    {
        current_max_size = head->next->size;
        while (temp->next != NULL)
        {
            if (temp->next->size > current_max_size)
            {
                current_max_size = temp->next->size;
                p = temp;
            }
            temp = temp->next;
        }
        if (p->next != head->next)
        {
            temp = p->next;
            p->next = p->next->next;
            temp->next = head->next;
            head->next = temp;
        }
        head = head->next;
        temp = head->next;
        p = head;
    }
    return;
}
// 创建一个新的进程
int new_process()
{
    struct allocatedBlock *ab;
    int size;
    int ret;
    ab = (struct allocatedBlock *)malloc(sizeof(struct allocatedBlock));
    if (!ab)
        exit(-5);
    ab->next = NULL;
    pid++;
    sprintf(ab->process_name, "PROCESS-%02d", pid);
    ab->pid = pid;
    printf("Memory for %s:", ab->process_name);
    printf("Please input you want to allocate process' size : ");
    scanf("%d", &size);
    if (size > 0)
    {

        ab->size = size;
    }
    ret = allocate_mem(ab);
    if ((ret == 1) && (allocatedBlockHead == NULL))
    {
        allocatedBlockHead = ab;
        return 1;
    }

    else if (ret == 1)
    {
        ab->next = allocatedBlockHead;
        allocatedBlockHead = ab;
        return 2;
    }
    else if (ret == -1)
    {
        printf("Allocation fail\n");
        pid--;
        free(ab);
        return -1;
    }
    return 3;
}

// 内存分配
int allocate_mem(struct allocatedBlock *ab)
{
    freeBlockType *fbt, *pre;
    freeBlockType *temp, *p, *p1;
    allocatedBlock *q;
    int request_size = ab->size;
    int sum = 0;
    int max;
    fbt = pre = freeBlock;
    if (fbt)
    {
        if (ma_algorithm == MA_WF)
        {
            if (fbt == NULL || fbt->size < request_size)
                return -1;
        }
        else
        {
            while (fbt != NULL && fbt->size < request_size)
            {
                pre = fbt;
                fbt = fbt->next;
            }
        }
        if (fbt == NULL || fbt->size < request_size)
        {
            if (freeBlock->next != NULL)
            {
                sum = freeBlock->size;
                temp = freeBlock->next;
                while (temp != NULL)
                {
                    sum += temp->size;
                    if (sum >= request_size)
                        break;
                    temp = temp->next;
                }
                if (temp == NULL)
                    return -1;
                else
                {
                    pre = freeBlock;
                    max = freeBlock->start_addr;
                    fbt = freeBlock;
                    while (temp->next != pre)
                    {
                        if (max < pre->start_addr)
                        {
                            max = pre->start_addr;
                            fbt = pre;
                        }
                        pre = pre->next;
                    }
                    pre = freeBlock;

                    while (pre != temp->next)
                    {
                        q = allocatedBlockHead;
                        p = freeBlock;

                        while (q != NULL)
                        {
                            if (q->start_addr > pre->start_addr)
                                q->start_addr = q->start_addr - pre->size;
                            q = q->next;
                        }
                        while (p != NULL)
                        {
                            if (p->start_addr > pre->start_addr)
                                p->start_addr = p->start_addr - pre->size;
                            p = p->next;
                        }

                        pre = pre->next;
                    }

                    pre = freeBlock;
                    while (pre != temp->next)
                    {

                        p1 = pre->next;
                        if (pre == fbt)
                            break;
                        free(pre);
                        pre = p1;
                    }
                    q = allocatedBlockHead;
                    freeBlock = fbt;
                    freeBlock->start_addr = q->start_addr + q->size;

                    freeBlock->size = sum;
                    freeBlock->next = temp->next;
                    if (freeBlock->size - request_size < MIN_SLICE)
                    {
                        ab->size = freeBlock->size;
                        ab->start_addr = freeBlock->start_addr;
                        pre = freeBlock;
                        freeBlock = freeBlock->next;
                        free(pre);
                    }
                    else
                    {
                        ab->start_addr = freeBlock->start_addr;
                        freeBlock->start_addr = freeBlock->start_addr + request_size;
                        freeBlock->size = freeBlock->size - request_size;
                    }
                }
            }
            else
                return -1;
        }
        else
        {
            if (fbt->size - request_size < MIN_SLICE)
            {
                ab->size = fbt->size;
                ab->start_addr = fbt->start_addr;
                if (pre->next == freeBlock)
                {
                    freeBlock = fbt->next;
                }
                else
                {
                    pre->next = fbt->next;
                }
                freeBlock = fbt->next;
                free(fbt);
            }
            else
            {
                ab->start_addr = fbt->start_addr;
                fbt->start_addr = fbt->start_addr + request_size;
                fbt->size = fbt->size - request_size;
            }
        }
        rearrange(ma_algorithm);
        return 1;
    }
    else
    {
        printf("Free Memory already has been allocated over: ");
        return -1;
    }
}

// 选择杀死一个进程

void kill_process()
{
    struct allocatedBlock *ab;
    int pid;
    printf("Kill Process, pid=");
    scanf("%d", &pid);
    ab = find_process(pid);
    if (ab != NULL)
    {
        free_mem(ab);
        dispose(ab);
    }
}
// 找到要杀死的进程的标号
allocatedBlock *find_process(int pid)
{
    allocatedBlock *abb;
    abb = allocatedBlockHead;
    if (abb->pid == pid)
    {
        return abb;
    }
    abb = allocatedBlockHead->next;
    while (abb->next != NULL)
    {
        if (abb->pid == pid)
            return abb;
        abb = abb->next;
    }
    return abb;
}

// 释放杀死进程的内存块
int free_mem(struct allocatedBlock *ab)
{
    int algorithm = ma_algorithm;
    struct freeBlockType *fbt, *pre;
    fbt = (struct freeBlockType *)malloc(sizeof(struct freeBlockType));
    pre = (struct freeBlockType *)malloc(sizeof(struct freeBlockType));
    if (!fbt)
        return -1;

    fbt->start_addr = ab->start_addr;
    fbt->size = ab->size;
    fbt->next = freeBlock;
    freeBlock = fbt;
    rearrange_FF();
    pre->next = freeBlock;
    pre->size = 0;
    while (pre->next && (pre->next->start_addr != fbt->start_addr))
        pre = pre->next;
    if (pre->size != 0 && fbt->next != NULL)
    {
        if (((pre->start_addr + pre->size) == fbt->start_addr) && ((fbt->start_addr + fbt->size) == fbt->next->start_addr))
        {
            pre->size = pre->size + fbt->size + fbt->next->size;
            pre->next = fbt->next->next;
            free(fbt->next);
            free(fbt);
        }
        else if ((pre->start_addr + pre->size) == fbt->start_addr)
        {
            pre->size = pre->size + fbt->size;
            pre->next = fbt->next;
            free(fbt);
        }
        else if (fbt->start_addr + fbt->size == fbt->next->start_addr)
        {
            fbt->size = fbt->size + fbt->next->size;
            fbt->next = fbt->next->next;
            free(fbt->next);
        }
    }
    else if ((pre->size == 0) && fbt->next)
    {
        if ((fbt->start_addr + fbt->size) == fbt->next->start_addr)
        {
            fbt->size = fbt->size + fbt->next->size;
            fbt->next = fbt->next->next;
            freeBlock = fbt;
            free(fbt->next);
        }
    }
    else if (fbt->next == NULL)
    {
        if ((pre->start_addr + pre->size) == fbt->start_addr)
        {
            pre->size = pre->size + fbt->size;
            pre->next = fbt->next;
            free(fbt);
        }
    }
    rearrange(algorithm);

    return 1;
}

// 销毁杀死进程的结点
int dispose(struct allocatedBlock *free_ab)
{
    struct allocatedBlock *pre, *ab;

    if (free_ab == allocatedBlockHead)
    {
        allocatedBlockHead = allocatedBlockHead->next;
        free(free_ab);
        return 1;
    }
    pre = allocatedBlockHead;
    ab = allocatedBlockHead->next;
    while (ab != free_ab)
    {
        pre = ab;
        ab = ab->next;
    }
    pre->next = ab->next;
    free(ab);
    return 2;
}

// 显示内存使用情况

int display_mem_usage()
{
    struct freeBlockType *fbt = freeBlock;
    struct allocatedBlock *ab = allocatedBlockHead;
    printf("----------------------------------------------------------\n");

    if (fbt == NULL)
    {
        printf("Free Memory already used over !\n");
    }
    printf("----------------------------------------------------------\n");

    if (fbt)
    {
        printf("Free Memory:\n");
        printf("%20s %20s\n", " start_addr", " size");
        while (fbt != NULL)
        {
            printf("%20d %20d\n", fbt->start_addr, fbt->size);
            fbt = fbt->next;
        }
    }

    printf("\nUsed Memory:\n");
    printf("%10s %20s %10s %10s\n", "PID", "ProcessName", "start_addr", " size");
    while (ab != NULL)
    {
        printf("%10d %20s %10d %10d\n", ab->pid, ab->process_name, ab->start_addr, ab->size);
        ab = ab->next;
    }
    printf("----------------------------------------------------------\n");
    return 0;
}

// 退出，销毁所有链表
void do_exit()
{
    freeBlockType *temp;
    allocatedBlock *temp1;
    temp = freeBlock->next;
    while (temp != NULL)
    {
        freeBlock->next = temp->next;
        free(temp);
        temp = freeBlock->next;
    }
    free(freeBlock);
    temp1 = allocatedBlockHead->next;
    while (temp1 != NULL)
    {
        allocatedBlockHead->next = temp1->next;
        free(temp1);
        temp1 = allocatedBlockHead->next;
    }
    free(allocatedBlockHead->next);
}

// 主函数
int main()
{
    char choice;
    pid = 0;
    freeBlock = initFreeBlock(memSize);
    while (1)
    {
        displayMenu();
        fflush(stdin);

        choice = getchar();
        switch (choice)
        {
        case '1':
            setMemSize();
            break;
        case '2':
            set_algorithm();
            flag = 1;
            break;
        case '3':
            new_process();
            flag = 1;
            break;
        case '4':
            kill_process();
            flag = 1;
            break;
        case '5':
            display_mem_usage();
            flag = 1;
            break;
        case '0':
            do_exit();
            exit(0);
        default:
            break;
        }
    }
    return 0;
}