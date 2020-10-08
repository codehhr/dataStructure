#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/***************************************************常量*******************************************************************/

#define SIZE 200

/***************************************************函数清单***************************************************************/

int readFile(int nums[], int size, char *filePath); //读取文件
void opration(int nums[], int *count);              //操作
void showData(int nums[], int *count);              //显示数据
void findData(int nums[], int *count);              //查找数据
void insertData(int nums[], int *count);            //插入数据
void delData(int nums[], int *count);               //删除数据
void getData(int nums[], int *count);               //获取元素（封装后的）
int get(int nums[], int pos);                       //获取元素

/***************************************************具体函数***************************************************************/
/*
    功能:读取文件的中的数字然后进行CRUD操作
    参数：无
    返回值：无
*/
int main()
{
    int count;      //读取的有效长度
    int nums[SIZE]; //容器数组
    //读取文件，成功 返回值为有效长度count，失败  返回值为-1并打印“文件打开失败”
    count = readFile(nums, SIZE, "test02.txt");
    //判断：如果成功，那么开始进一不操作，失败则退出程序
    if (count != -1)
    {
        //清空缓存区域
        _flushall();
        //进行操作
        opration(nums, &count);
    }
    // 执行结束
    return 0;
}

/*
    功能：从一个文件中读取一行一行的整数，每一行只能有一个整数
    参数：
        nums:容器数组
        size：数组最大容量
        filePath：文件路径
    返回值：
        int类型，读取成功返回读取的有效行数，失败返回-1

*/
int readFile(int nums[], int size, char *filePath)
{
    int count = 0;  //有效行数
    char buf[1024]; //读取一行时的容器

    //打开文件输入流
    FILE *fp = fopen(filePath, "r");

    //校验文件是否打开成功
    if (fp == NULL)
    {
        printf("文件打开失败");
        return -1;
    }

    //将流写入数组中
    while (!feof(fp) && count <= size) //如果文件没有结束并且数组没有满，那么就持续循环
    {
        //读取一行到buf中
        fgets(buf, 1024, fp);
        //将buf转换成整型数字，然后赋值给nums[count]
        nums[count] = atoi(buf);
        //count++
        count++;
    }

    //校验文本是否全部读完
    if (count == size && !feof(fp)) //有效行数达到了数组的最大容量且流还未结束)
    {
        printf("数组容器最大承载 %d 个数字", size);
        return -1;
    }
    //关闭文件输入流
    fclose(fp);
    //以上代码执行成功，返回有效行数
    return count;
}
/*
    功能：操作
    参数:
        nums:容器数组
        count：有效行数
    返回值：无

*/
void opration(int nums[], int *count)
{
    _flushall(); //以防万一，清除一下流
    char op;     //输入的操作
    //输出一些操作前的提示
    printf("操作说明：\n1:显示数据\n2:查找数据\n3:插入数据\n4:删除数据\n5:获取数据\n0:退出\n");
    //循环操作，输入操作类型
    while (1)
    {
        printf("请输入您的操作:");
        _flushall();
        op = getchar();
        if (op == '0' || op == '1' || op == '2' || op == '3' || op == '4' || op == '5')
        {
            if (op == '0')
            {
                printf("退出成功\n");
                return;
            }
            break;
        }
        else
        {
            printf("您的输入有误\n");
        }
    }
    //循环操作，执行操作的函数
    while (op != '0')
    {
        switch (op)
        {
        case '1':
            showData(nums, count);
            break;
        case '2':
            findData(nums, count);
            break;
        case '3':
            insertData(nums, count);
            break;
        case '4':
            delData(nums, count);
            break;
        case '5':
            getData(nums, count);
            break;
        default:
            printf("请输入合法字符\n");
            break;
        }

        printf("请继续您的操作:");
        _flushall();
        op = getchar();
        if (op == '0')
        {
            printf("退出成功\n");
        }
    }
}

/*
    功能：显示数据
    参数：
        nums：数组容器
        count：有效行数
    返回值：无
*/
void showData(int nums[], int *count)
{
    int j = 0;
    while (j < *count)
    {
        printf("%d : %d\n", j + 1, nums[j]);
        j++;
    }
}

/*
    功能：查找数据并打印出来
    参数：
        nums:数组容器
        count:有效行数
    返回值：无
*/
void findData(int nums[], int *count)
{
    int value; //需要查找的内容
    int i = 0;
    //输入查找的内容
    printf("请输入您要查找的内容:");
    scanf("%d", &value);
    //查找
    while (i < *count)
    {
        if (value == nums[i])
        {
            printf("查找成功，位置:%d  内容：%d\n", i + 1, nums[i]);
            return;
        }
        i++;
    }
    printf("查找失败：没有此内容\n");
}

/*
    功能：插入数据
    参数：
        nums：容器数组
        count：有效行数
    返回值：无
*/
void insertData(int nums[], int *count)
{
    int pos;   //插入的位置
    int value; //插入的数据
    int j;     //插入时候使用的游标
    printf("请输入您将要插入的数据：");
    scanf("%d", &value);
    printf("请输入您要插入的位置：");
    scanf("%d", &pos);
    //插入之前判断一下是否可以插入
    while (1)
    {
        if (*count == SIZE)
        {
            printf("容器已满，无法插入\n");
            return;
        }
        else if (pos < 1 || pos >= *count + 2)
        {
            printf("插入的位置不合理,请重新输入位置:");
            scanf("%d", &pos);
        }
        else
        {
            break;
        }
    }
    //开始插入
    j = *count;
    while (j >= pos)
    {
        nums[j] = nums[j - 1];
        j--;
    }
    //新元素保存到pos位置
    nums[pos - 1] = value;
    //有效行数加1
    (*count)++;
    printf("插入成功\n");
}
/*
    功能：删除某一行
    参数：
        nums:数组容器
        count:有效行数
    返回值：无
*/
void delData(int nums[], int *count)
{
    int pos; //删除的位置
    int j;   //删除时候用的游标
    //输入删除的位置
    printf("请输入删除的位置:");
    scanf("%d", &pos);
    //判断删除的位置是否合理
    while (1)
    {
        if ((pos < 1 || pos > *count))
        {
            printf("删除的位置不存在，请重新输入:");
            scanf("%d", &pos);
        }
        else
        {
            break;
        }
    }
    //删除
    j = pos;
    while (j <= *count)
    {
        nums[j - 1] = nums[j];
        j++;
    }
    printf("删除成功\n");
    (*count)--;
}

/*
    功能：获取某一位置的数据(封装Get)
    参数：
        nums:数组容器
        count:有效行数
    返回值:无
*/
void getData(int nums[], int *count)
{
    int value; //获取到的值
    int pos;   //位置
    printf("请输入要获取元素的位置:");
    scanf("%d", &pos);
    while (1)
    {
        if (pos < 1 || pos > *count)
        {
            printf("输入不合理，请重新输入:");
            scanf("%d", &pos);
        }
        else
        {
            break;
        }
    }
    value = get(nums, pos);
    printf("查找成功，值为：%d\n", value);
}

/*
    功能：获取pos位置的值
    参数：
        pos：要获取值的位置
    返回值：
        pos位置的值
*/
int get(int nums[], int pos)
{
    return nums[pos - 1];
}
