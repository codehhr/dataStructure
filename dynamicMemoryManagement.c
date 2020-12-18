//（1）建立初始的内存状态数据：内存当前有几个空闲分区，每个分区的大小和起始地址。

//（2）能从界面输入要求分配的分区大小

//（3）程序根据分区管理数据的值确定为当前需求分配的分区大小及所分配的分区的起始地址

//（4）动态分区分配的模拟程序要求界面能显示当前分区的信息：空闲分区的个数、每个空闲分区的大小和起始地址

//（5）动态分区分配算法采用最佳适应算法、首次适应算法、循环首次使用算法之一即可，也可以在程序中实现三种算法。

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct mem_control
{
    int start;                // 起始地址
    int lenght;               // 长度
    struct mem_control *next; //
};

// 函数声明
void init();
void printToScreen();
void memController();
void allocateMem();
void reclaimMem();

struct mem_control *memEmpty;   //空闲分区
struct mem_control *memUsed;    //使用分区
struct mem_control *memRelease; //释放分区

int main()
{
    memEmpty = NULL;
    memUsed = NULL;
    memRelease = NULL;
    printf("----------------------------------------\n");
    init();
    while (1)
    {
        memController();
        printToScreen();
    }
    return 0;
}

/*
初始化空闲分区
初始化一个空间，首地址为0
*/
void init()
{
    int size;
    memEmpty = (struct mem_control *)malloc(sizeof(struct mem_control));
    memEmpty->start = 0;
    printf("请输入总内存大小 : ");
    scanf("%d", &size);
    memEmpty->lenght = size;
    memEmpty->next = NULL;
}

/*
输出空闲分区
*/
void printToScreen()
{
    int num = 0;
    struct mem_control *test = memEmpty;
    while (test != NULL)
    {
        num++;
        test = test->next;
    }
    printf("空闲分区个数为%d\n", num);
    test = memEmpty;
    num = 0;
    while (test != NULL)
    {
        num++;
        printf("\n 序号 起始地址 空间大小 \n");
        printf("%5d %10d %10d \n", num, test->start, test->lenght);
        test = test->next;
    }
}

/*
循环运行，进行内存控制
*/
void memController()
{
    char select;
    do
    {
        printf("本函数为进程控制函数\n");
        printf("\t1 - 分配内存\n");
        printf("\t2 - 回收内存\n");
        printf("\t3 - 无操作\n");
        printf("\t0 - 退出\n");
        printf("请输入你的选择(1--4): ");
        select = getchar();
        system("cls");
    } while (select != '1' && select != '2' && select != '3' && select != '4');
    switch (select)
    {
    case '1':
        allocateMem();
        break;
    case '2':
        reclaimMem();
        break;
    case '3':
        break;
    case '0':
        exit(0);
        break;
    default:
        return;
    }
}

/*
分配内存，采用首次适应算法
*/
void allocateMem()
{
    //输入要分配内存的大小
    char *tmpStr;
    printf("请输入要分配的大小 : ");
    scanf("%s", tmpStr);
    do
    {
        printf("请输入要分配的大小 : ");
        scanf("%s", tmpStr);
    } while (isdigit(tmpStr) == 0);

    // 转换为一个整数
    int tmplen = atoi(tmpStr);
    struct mem_control *test = memEmpty;
    if (test == NULL)
    {
        printf("内存不足\n");
    }
    else
    {
        while (test->lenght < tmplen)
        {
            test = test->next;
        }

        //将分配的内存添加到已使用内存队列中
        if (memUsed == NULL) //判断已使用队列是否已有内存块
        {
            memUsed = (struct mem_control *)malloc(sizeof(struct mem_control));
            memUsed->start = test->start;
            memUsed->lenght = tmplen;
            memUsed->next = NULL;
            printf("分配内存开始地址%d，大小%d。\n", test->start, tmplen);
        }
        else
        {
            struct mem_control *test2 = memUsed;
            while (test2->next != NULL)
            {
                test2 = test2->next;
            }
            test2->next = (struct mem_control *)malloc(sizeof(struct mem_control));
            test2->next->start = test->start;
            test2->next->lenght = tmplen;
            test2->next->next = NULL;
        }
        //将已分配内存从空闲内存队列中移除
        if (test->lenght == tmplen) //判断被分离内存块是否有剩余
        {
            if (test == memEmpty) //判断被分离内存块是否在队首
            {
                memEmpty = memEmpty->next;
                free(test);
            }
            else
            {
                struct mem_control *test3 = memEmpty;
                while (test3->next != test)
                {
                    test3 = test3->next;
                }
                test3->next = test->next;
                free(test);
            }
        }
        else
        {
            test->start += tmplen;
            test->lenght -= tmplen;
        }
    }
}

/*
回收内存
*/
void reclaimMem()
{
    char select;
    //循环遍历，选择要回收的内存空间
    struct mem_control *test = memUsed;
    while (test != NULL)
    {
        printf("\n 起始地址 空间大小 \n");
        printf("%10d %10d \n", test->start, test->lenght);
        printf("回收本进程输入Y/y，不回收输入N/n。\n");
        do
        {
            select = getchar();
        } while (select != 'y' && select != 'Y' && select != 'n' && select != 'N');
        if (select == 'y' || select == 'Y') //选择回收该内存
        {
            //将要回收内存从已用内存队列移除
            if (memUsed == test) //判断是否为队首
            {
                memRelease = memUsed;
                memUsed = memUsed->next;
            }
            else //回收内存非队首
            {
                struct mem_control *test2 = memUsed;
                while (test2->next != test)
                {
                    test2 = test2->next;
                }
                test2->next = test->next;
                memRelease = test;
            }
            //将要回收内存添加到未使用内存队列
            if (memEmpty == NULL)
            {
                memEmpty = memRelease;
                memEmpty->next = NULL;
                memRelease = NULL;
            }
            else if (memRelease->start < memEmpty->start) //未使用空间添加到队首
            {
                if (memRelease->start + memRelease->lenght == memEmpty->start) //未使用空间可以向后合并
                {
                    memEmpty->start = memRelease->start;
                    memEmpty->lenght += memRelease->lenght;
                    free(memRelease);
                    memRelease = NULL;
                }
                else //未使用空间不可合并
                {
                    memRelease->next = memEmpty;
                    memEmpty = memRelease;
                    memRelease = NULL;
                }
            }
            else
            {
                struct mem_control *test3 = memEmpty;
                while (test3->next != NULL && test3->next->start < memRelease->start)
                {
                    test3 = test3->next;
                }
                if (test3->next == NULL) //未使用空间添加到队尾
                {
                    if (test3->start + test3->lenght == memRelease->start) //未使用空间可以向前合并
                    {
                        test3->lenght += memRelease->lenght;
                        free(memRelease);
                        memRelease = NULL;
                    }
                    else //未使用空间不可合并
                    {
                        test3->next = memRelease;
                        memRelease->next = NULL;
                        memRelease = NULL;
                    }
                }
                else //添加到队列中间
                {
                    if (test3->start + test3->lenght == memRelease->start) //未使用空间可以向前合并
                    {
                        test3->lenght += memRelease->lenght;
                        free(memRelease);
                        memRelease = NULL;
                        if (test3->start + test3->lenght == test3->next->start) //未使用空间可以向后合并
                        {
                            test3->lenght += test3->next->lenght;
                            test3->next = test3->next->next;
                            free(test3->next);
                        }
                    }
                    else if (memRelease->start + memRelease->lenght == test3->next->start) //未使用空间可以向后合并
                    {
                        test3->next->start = memRelease->start;
                        test3->lenght += memRelease->lenght;
                        memRelease = NULL;
                    }
                    else
                    {
                        memRelease->next = test3->next;
                        test3->next = memRelease;
                        memRelease = NULL;
                    }
                }
            }
            return;
        }
        test = test->next;
    }
}