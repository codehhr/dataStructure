#include <stdio.h>

// 定义内存的大小为 100
#define MEMSIZE 100

// 如果内存大小小于此值，将不再进行分割
#define MINSIZE 2

// 内存分区空间表结构
typedef struct memoryInfomation
{
    // 起始地址
    int start;
    // 大小
    int Size;
    // 状态 f:空闲(free)  u:占用(used)  e:结束(end)
    char status;
} MEMINFO;

// 内存空间信息表
MEMINFO MemList[MEMSIZE];

// 显示内存状态
void display()
{
    system("cls");
    int i, used = 0; // 记录可以使用的总空间量
    printf("\n+----------------------------------------------+\n");
    printf("| %5s%15s%15s%15s  |", "序号", "起始", "大小", "状态");
    printf("\n------------------------------------------------\n");
    for (i = 0; i < MEMSIZE && MemList[i].status != 'e'; i++)
    {
        if (MemList[i].status == 'u')
        {
            used += MemList[i].Size;
        }
        printf("| %4d%12d%14d%15s  |\n", i + 1, MemList[i].start, MemList[i].Size, MemList[i].status == 'u' ? "已用" : "空闲");
    }
    printf("\n------------------------------------------------\n");
    printf("| 总内存:%-12d 已用:%-10d 空闲:%d  |", MEMSIZE, used, MEMSIZE - used);
    printf("\n+----------------------------------------------+\n");
    printf("\n\n");
}

// 初始化所有变量
void InitMemList()
{
    system("cls");
    MEMINFO item = {0, 0, 'e'};
    //初始化内存空间信息表
    for (int i = 0; i < MEMSIZE; i++)
    {
        MemList[i] = item;
    }
    //起始地址为0
    MemList[0].start = 0;
    //空间初始为最大
    MemList[0].Size = MEMSIZE;
    //状态为空闲
    MemList[0].status = 'f';
}

// 最先适应算法

/*算法原理分析：
将空闲的内存区按其在储存空间中的起始地址递增的顺序排列，分配储存空间时，从空闲区链的始端开始查找，选择第一个满足要求的空闲区，而不管它究竟有多大

优点:
1.在释放内存分区的时候，如果有相邻的空白区就进行合并，使其成为一个较大的空白区
2.此算法的实质是尽可能的利用储存器的低地址部分，在高地址部分则保留多的或较大的空白区，以后如果需要较大的空白区，就容易满足

缺点：
1.在低地址部分很快集中了许多非常小的空白区，因而在空白区分配时，搜索次数增加，影响工作效率。
*/

void firstFit()
{
    int i, j, flag = 0;
    int request;
    printf("[最先适应算法]请求分配内存的大小 : ");
    scanf("%d", &request);
    // 遍历数组
    for (i = 0; i < MEMSIZE && MemList[i].status != 'e'; i++)
    {
        // 满足所需要的大小,且是空闲空间
        if (MemList[i].Size >= request && MemList[i].status == 'f')
        {
            // 如果小于等于规定的最小差则将整个空间分配出去
            if (MemList[i].Size - request <= MINSIZE)
            {
                // 标记为已用 (used)
                MemList[i].status = 'u';
            }
            else
            {
                //将当前元素(i)后的信息表元素后移
                for (j = MEMSIZE - 2; j > i; j--)
                {
                    MemList[j + 1] = MemList[j];
                }

                //将i分成两部分，使用低地址部分
                MemList[i + 1].start = MemList[i].start + request;
                MemList[i + 1].Size = MemList[i].Size - request;
                MemList[i + 1].status = 'f';
                MemList[i].Size = request;
                MemList[i].status = 'u';
                flag = 1;
            }
            break;
        }
    }
    //没有找到符合分配的空间
    if (flag != 1 || i == MEMSIZE || MemList[i].status == 'e')
    {
        printf("内存不足 ! !\n");
    }
    system("cls");
    display();
}
/*最坏适应算法

算法原理分析：
扫描整个空闲分区或者链表，总是挑选一个最大的空闲分区来使用

优点：可以使链表中的节点大小趋于均匀，产生碎片的几率最小，对中小作业有利，同时该算法的查找效率很高

缺点：会使得储存器中缺乏大的空闲分区
*/
void worstFit()
{
    int i, j, k, flag, request;
    printf("[最坏适应算法]请求分配内存的大小 : ");
    scanf("%d", &request);
    j = 0;
    flag = 0;
    k = 0;
    //保存满足要求的最大空间
    for (i = 0; i < MEMSIZE - 1 && MemList[i].status != 'e'; i++)
    {
        if (MemList[i].Size >= request && MemList[i].status == 'f')
        {
            flag = 1;
            if (MemList[i].Size > k)
            {
                k = MemList[i].Size;
                j = i;
            }
        }
    }
    i = j;
    if (flag == 0)
    {
        printf("内存不足 ! !\n");
        j = i;
    }
    else if (MemList[i].Size - request <= MINSIZE) // 如果小于规定的最小差则将整个空间分配出去
    {
        MemList[i].status = 'u';
    }
    else
    {
        for (j = MEMSIZE - 2; j > i; j--)
        {
            MemList[j + 1] = MemList[j];
        }
        MemList[i + 1].start = MemList[i].start + request;
        MemList[i + 1].Size = MemList[i].Size - request;
        MemList[i + 1].status = 'f';
        MemList[i].Size = request;
        MemList[i].status = 'u';
    }
    system("cls");
    display();
}

/*最佳适应算法

算法原理分析：
从全部空闲区中找出满足作业要求的，且大小最小的空闲分区的一种计算方法，这种方法能使得碎片尽量小，为适应此算法，空闲分区表中的空闲分区要按从小到大进行排序，自表头开始查找第一个满足要求的自由分区分配

优点：能使得碎片尽量的小,保留了最大空闲区

缺点：造成了许多小的空闲区
*/
void bestFit()
{
    int i, j, t, flag, request;
    printf("[最佳适应算法]请求分配内存的大小 : ");
    scanf("%d", &request);
    j = 0;
    flag = 0;
    t = MEMSIZE;
    //保存满足要求的最大空间
    for (i = 0; i < MEMSIZE && MemList[i].status != 'e'; i++)
    {
        if (MemList[i].Size >= request && MemList[i].status == 'f')
        {
            flag = 1;
            if (MemList[i].Size < t)
            {
                t = MemList[i].Size;
                j = i;
            }
        }
    }
    i = j;
    if (flag == 0)
    {
        printf("内存不足 ! !\n");
        j = i;
    }
    else if (MemList[i].Size - request <= MINSIZE) // 如果小于规定的最小差则将整个空间分配出去
    {
        MemList[i].status = 'u';
    }
    else
    {
        //将i后的信息表元素后移
        for (j = MEMSIZE - 2; j > i; j--)
        {
            MemList[j + 1] = MemList[j];
        }

        //将i分成两部分，使用低地址部分
        MemList[i + 1].start = MemList[i].start + request;
        MemList[i + 1].Size = MemList[i].Size - request;
        MemList[i + 1].status = 'f';
        MemList[i].Size = request;
        MemList[i].status = 'u';
    }
    system("cls");
    display();
}

//释放一块内存
void deleteBlock()
{
    int i, number;
    printf("\n请输入你想关掉的进程序号:\n");
    scanf("%d", &number);
    number -= 1; // 为方便操作,序号先换为索引
    // 输入的空间是使用的
    if (MemList[number].status == 'u')
    {
        MemList[number].status = 'f';          // 标记为空闲
        if (MemList[number + 1].status == 'f') // 若右侧空间为空则合并
        {
            MemList[number].Size += MemList[number + 1].Size;                      //大小合并
            for (i = number + 1; i < MEMSIZE - 1 && MemList[i].status != 'e'; i++) //i后面的空间信息表元素后移
            {
                if (i > 0)
                {
                    MemList[i] = MemList[i + 1];
                }
            }
        }
        // 左测空间空闲则合并
        if (number > 0 && MemList[number - 1].status == 'f')
        {
            MemList[number - 1].Size += MemList[number].Size;                  // 左侧与当前合并
            for (i = number; i < MEMSIZE - 1 && MemList[i].status != 'e'; i++) // i后面的空间信息表元素后移
            {
                MemList[i] = MemList[i + 1];
            }
        }
    }
    else
    {
        printf("此序号没有被使用或者不存在\n");
    }
    system("cls");
    display();
}

int main()
{
    int n;
    InitMemList(); //变量初始化
    while (1)
    {
        printf("************************************************\n");
        printf("\t1 - 使用 [首次适应算法] 分配一块新的内存\n");
        printf("\t2 - 使用 [最好适应算法] 分配一块新的内存\n");
        printf("\t3 - 使用 [最坏适应算法] 分配一块新的内存\n");
        printf("\t4 - 回收一块内存\n");
        printf("\t5 - 显示内存使用情况\n");
        printf("\t0 - Exit \n");
        printf("************************************************\n");
        printf("input : ");
        scanf("%d", &n);
        switch (n)
        {
        case 1:
            firstFit(); // 最先适应算法
            break;
        case 2:
            bestFit(); // 最佳适应算法
            break;
        case 3:
            worstFit(); // 最坏适应算法
            break;
        case 4:
            deleteBlock(); // 删除已经使用的空间
            break;
        case 5:
            display(); // 显示内存分配情况
            break;
        case 0:
            exit(0);
        }
    }
    return 0;
}