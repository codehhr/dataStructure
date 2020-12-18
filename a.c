// #include <stdio.h>
// #include <bits/stdc++.h>
// using namespace std;

// 定义内存的大小为 100
#define MEMSIZE 100

// 如果内存大小小于此值，将不再进行分割
#define MINSIZE 2

// 内存分区空间表结构
typedef struct _MemoryInfomation
{
    /*起始地址*/
    int start;
    /*大小*/
    int Size;
    /*状态 F:空闲(Free) U:占用(Used) E 结束(End)*/
    char status;
} MEMINFO;

/*内存空间信息表*/
MEMINFO MemList[MEMSIZE];

/*显示内存状态*/
void Display()
{
    int i, used = 0; //记录可以使用的总空间量
    printf("\n---------------------------------------------------\n");
    printf("%5s%15s%15s%15s", "| 序号", "\t起始", "\t大小", "\t状态 |");
    printf("\n---------------------------------------------------\n");
    for (i = 0; i < MEMSIZE && MemList[i].status != 'e'; i++)
    {
        if (MemList[i].status == 'u')
        {
            used += MemList[i].Size;
        }
        printf("%5d%15d%15d%15s\n", i, MemList[i].start, MemList[i].Size, MemList[i].status == 'u' ? "已用" : "空闲");
    }
    printf("\n----------------------------------------------\n");
    printf("Totalsize:%-10d Used:%-10d Free:%-10d\n", MEMSIZE, used, MEMSIZE - used);
}

/*初始化所有变量*/
void InitMemList()
{
    int i;
    MEMINFO temp = {0, 0, 'e'};
    //初始化空间信息表
    for (i = 0; i < MEMSIZE; i++)
    {
        MemList[i] = temp;
    }
    //起始地址为0
    MemList[0].start = 0;
    //空间初始为最大
    MemList[0].Size = MEMSIZE;
    //状态为空闲
    MemList[0].status = 'f';
}

/*最先适应算法*/

/*算法原理分析：
将空闲的内存区按其在储存空间中的起始地址递增的顺序排列，为作业分配储存空间时，从空闲区链的始端开始查找，选择第一个满足要求的空闲区，而不管它究竟有多大

优点:
1.在释放内存分区的时候，如果有相邻的空白区就进行合并，使其成为一个较大的空白区
2.此算法的实质是尽可能的利用储存器的低地址部分，在高地址部分则保留多的或较大的空白区，以后如果需要较大的空白区，就容易满足

缺点：
1.在低地址部分很快集中了许多非常小的空白区，因而在空白区分配时，搜索次数增加，影响工作效率。*/

void FirstFit_new()
{
    int i, j, flag = 0;
    int request;
    printf("[首次适应算法]分配内存的大小 : \n");
    scanf("%d", &request);
    //遍历数组
    for (i = 0; i < MEMSIZE && MemList[i].status != 'e'; i++)
    {
        //满足所需要的大小,且是空闲空间
        if (MemList[i].Size >= request && MemList[i].status == 'f')
        {
            //如果小于规定的最小差则将整个空间分配出去
            if (MemList[i].Size - request <= MINSIZE)
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
    Display();
}
/*最坏适应算法

算法原理分析：
扫描整个空闲分区或者链表，总是挑选一个最大的空闲分区分割给作业使用

优点：可以使得剩下的空闲分区不至于太小，产生碎片的几率最小，对中小作业有利，同时该算法的查找效率很高

缺点：会使得储存器中缺乏大的空闲分区
*/
void BadFit_new()
{
    int i, j, k, flag, request;
    printf("[最坏适应算法]分配内存的大小 : \n");
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
    else if (MemList[i].Size - request <= MINSIZE)
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
    Display();
}

//释放一块内存
void del_t()
{
    int i, number;
    printf("\n请输入你想关掉的进程的序号:\n");
    scanf("%d", &number);
    //输入的空间是使用的
    if (MemList[number].status == 'u')
    {
        MemList[number].status = 'f';          //标志为空闲
        if (MemList[number + 1].status == 'f') //右侧空间为空则合并
        {
            MemList[number].Size += MemList[number].Size;                          //大小合并
            for (i = number + 1; i < MEMSIZE - 1 && MemList[i].status != 'e'; i++) //i后面的空间信息表元素后移
            {
                if (i > 0)
                {
                    MemList[i] = MemList[i + 1];
                }
            }
        }
        //左测空间空闲则合并
        if (number > 0 && MemList[number - 1].status == 'f')
        {
            MemList[number - 1].Size += MemList[number].Size;
            for (i = number; i < MEMSIZE - 1 && MemList[i].status != 'e'; i++)
            {
                MemList[i] = MemList[i + 1];
            }
        }
    }
    else
    {
        printf("此序号不存在或没有被使用\n");
    }
    Display();
}

/*最佳适应算法

算法原理分析：
从全部空闲区中找出满足作业要求的，且大小最小的空闲分区的一种计算方法，这种方法能使得碎片尽量小，为适应此算法，空闲分区表中的空闲分区要按从小到大进行排序，自表头开始查找第一个满足要求的自由分区分配

优点：能使得碎片尽量的小,保留了最大空闲区

缺点：造成了许多小的空闲区
*/
void BestFit_new()
{
    int i, j, t, flag, request;
    printf("[最好适应算法]分配内存的大小 : \n");
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
    else if (MemList[i].Size - request <= MINSIZE) //如果小于规定的最小差则将整个空间分配出去
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
    Display();
}

int main()
{
    int x;
    InitMemList(); //变量初始化
    while (1)
    {
        printf("------------------------------------------------\n");
        printf("\t1 - 使用 [首次适应算法] 分配一块新的内存\n");
        printf("\t2 - 使用 [最好适应算法] 分配一块新的内存\n");
        printf("\t3 - 使用 [最坏适应算法] 分配一块新的内存\n");
        printf("\t4 - 回收一块内存\n");
        printf("\t5 - 显示内存使用情况\n");
        printf("\t0 - Exit \n");
        printf("------------------------------------------------\n");
        printf("input : ");
        scanf("%d", &x);
        switch (x)
        {
        case 1:
            FirstFit_new(); //首次适应算法
            break;
        case 2:
            BestFit_new(); //最佳适应算法
            break;
        case 3:
            BadFit_new(); //最坏适应算法
            break;
        case 4:
            del_t(); //删除已经使用完毕的空间
            break;
        case 5:
            Display(); //显示内存分配情况
            break;
        case 0:
            exit(0);
        }
    }
    return 0;
}