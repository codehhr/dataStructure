#include <stdio.h>
#include <stdlib.h>
#include <malloc.h> //动态内存管理
#include <string.h>
#define SIZE 200                                                                 //定义最长读取200行
int readFile(char *container[], int size, char *fileName);                       //文件读入
void freeContainer(char *container[], int *count);                               //释放文件读入的内存
void showData(char *container[], int *count);                                    //显示数据
int findSimple(char *container[], int *count, char *value);                      //查找数据
void findData(char *container[], int *count);                                    //查找数据封装
int InsertSimple(char *container[], int size, int *count, int pos, char *value); //插入数据
void InsertData(char *container[], int size, int *count, char *value);           //插入数据封装
int DeleteSimple(char *container[], int *count, int pos);                        //删除数据
void DeleteData(char *container[], int *count);                                  //删除数据封装
int upDataSimple(char *container[], int *count, int pos, char *value);           //更新数据
void UpData(char *container[], int *count, char *value);                         //更新数据封装
void opration(char *container[], int *count);                                    //操作

/*
    实现文件读入，文件搜索
*/
int main()
{
    int count;
    char *container[SIZE];
    //input your code
    //加载数据并返回有效行数
    count = readFile(container, SIZE, "C:\\Users\\18719\\Desktop\\DS1\\2016List2.csv");
    if (count != -1) //读取成功，进一步操作
    {
        //清除输入输出流
        _flushall();
        //操作数据
        opration(container, &count);
        //释放内存空间
        freeContainer(container, &count);
    }
    return 0;
}

/*
    功能:文件读入
    参数列表:
            container:读入文件的容器，一个数组，数组中保存着指向每一行的内存地址
            size:container容器的最大容量
            fileName:文件路径
    返回值:读取正常:读取的有效行数数量 读取失败:-1

*/
int readFile(char *container[], int size, char *fileName)
{
    int c = 0;
    char buf[1024];
    //打开文件输入流
    FILE *fp = fopen(fileName, "r");
    //文件打开失败，返回 0并提示
    if (fp == NULL)
    {
        printf("文件 %s 打开失败，请检查文件路径\n", fileName);
        return -1;
    }
    //将流按行读入buf中，再将buf写入container容器中
    while (c < size && !feof(fp))
    {
        //切片
        fgets(buf, 1024, fp);
        //去掉换行符
        if (buf[strlen(buf) - 1] == 0x00000A)
        {
            buf[strlen(buf) - 1] = '\0';
        }
        //读取有效行
        if (buf[0] != '\0')
        {
            container[c] = (char *)malloc(strlen(buf) + 1);
            if (container[c] != NULL)
            {
                strcpy(container[c], buf);
                c++;
            }
        }
    }
    fclose(fp);
    return c;
}
/*
    功能:释放文件读入内存
    参数:
        container:需要释放内存的数组
        c:有效行数
    返回值:无
*/
void freeContainer(char *container[], int *count)
{
    int j = 0;
    while (j < *count)
    {
        if (container[j] != NULL)
            free(container[j]);
        j++;
    }
}

/*
    功能:显示
    参数:
        container:存放数据的数组
        count: 有效行数
    返回值:无
*/
void showData(char *container[], int *count)
{
    int j = 0;
    while (j < *count)
    {
        printf("%d %s\n", (j + 1), container[j]);
        j++;
    }
}
/*
    功能:匹配查找内容
    参数:
        container:存放数据的数组
        count: 有效行数
        value:要查找的值
    返回值:0:查找失败，大于0返回查找到的在第几行
*/
int findSimple(char *container[], int *count, char *value)
{
    char *p;
    int j = 0;

    while (j < *count)
    {
        p = strstr(container[j], value); //返回value内容到整个内容结束的内容
        if (p != NULL)
        {
            return j + 1;
        }
        j++;
    }
    return 0;
}
/*
    功能:查找
    参数:
        container:存放数据的数组
        count:有效行数
    返回值:无
*/
void findData(char *container[], int *count)
{
    char str[100];
    int j;
    printf("请输入要查找的内容:");
    scanf("%s", str);
    j = findSimple(container, count, str);
    if (j > 0)
    {
        printf("查找成功:位置:%d 内容:%s\n", j, container[j - 1]);
    }
    else
    {
        printf("查找数据失败\n");
    }
}

/*
    功能:插入
    参数:
        container:存储数组
        size:数组最大长度
        count:有效行数
        pos:要插入的位置(行)
        value:要插入的内容
    返回值:0:插入失败 1:成功

*/
int InsertSimple(char *container[], int size, int *count, int pos, char *value)
{
    int c = *count; //将有效值复制一份

    while (1)
    {
        //判断是否可以继续插入
        if (c >= size)
        {
            printf("容器已满，不可继续插入\n");
            return 0;
        }
        else
        {
            container[c] = container[c - 1]; //将数组最后一个元素向后移动一位  这里应该让值移动，地址不应该移动
            if (c != pos - 1)
            {
                c--;
            }
            if (c == (pos - 1))
            {
                container[c] = value;
                return 1;
            }
        }
    }
}
/*
    功能:进一步封装插入
    参数:
        container:存储数组
        size:数组最大长度
        count:有效行数
    返回值:无

*/
void InsertData(char *container[], int size, int *count, char *value)
{
    char str[100];
    int pos;
    int result;
    printf("请输入要插入的内容:");
    scanf("%s", value);
    while (1)
    {
        printf("请输入要插入的行数:");
        scanf("%s", str);
        pos = atoi(str);
        if (pos <= 0 || pos > (*count + 1))
        {
            printf("您的输入不合法(信不信我批斗你)\n");
        }
        else
        {
            break;
        }
    }
    result = InsertSimple(container, size, count, pos, value);
    if (result == 1)
    {
        printf("插入成功\n");
        *count = *count + 1; //行数加1
    }
    else
    {
        printf("插入失败\n");
    }
}
/*
    功能:删除某一行
    参数:
        container:数据容器
        count:有效行数
        value:要更新的值
        pos:删除的位置
    返回值:0:删除失败，1:删除成功

*/
int DeleteSimple(char *container[], int *count, int pos)
{
    while (1)
    {
        if (0 == *count)
        {
            printf("数据已空，无法删除");
            return 0;
        }
        //如果是最后一行
        if (pos == *count)
        {
            container[pos - 1] = NULL;
            return 1; //删除成功
        }
        container[pos - 1] = container[pos];
        pos++;
        if (pos == *count)
        {
            return 1; //删除成功
        }
    }
}
/*
    功能:封装删除
    参数:
        container:数据容器
        count:有效行数
    返回值:无
*/
void DeleteData(char *container[], int *count)
{
    char str[100];
    int pos;    //删除位置（行）
    int result; //删除返回值结果
    while (1)
    {
        printf("请输入要删除的行数:");
        scanf("%s", str);
        pos = atoi(str);
        if (pos <= 0 || pos > *count)
        {
            printf("您的输入不合法(信不信我批斗你)\n");
        }
        else
        {
            break;
        }
    }
    result = DeleteSimple(container, count, pos);
    if (result == 1)
    {
        printf("删除成功\n");
        *count = *count - 1;
    }
    else
    {
        printf("删除失败\n");
    }
}
/*
    功能:更新
    参数:
        container:数据容器
        count:有效行数
        pos:要跟新的行数
        value:更新的值
    返回值:0:更新失败 1:更新成功
*/
int upDataSimple(char *container[], int *count, int pos, char *value)
{
    if (pos <= 0 || pos > *count)
    {
        return 0;
    }
    else
    {
        container[pos - 1] = value;
        return 1;
    }
}
/*
    功能:封装更新
    参数:
        container:数据容器
        count:有效行数
        value:要更新的值的暂时空间
    返回值:无
*/
void UpData(char *container[], int *count, char *value)
{
    char str[100];
    int pos;
    int result;
    printf("请输入要更新的内容:");
    scanf("%s", value);
    while (1)
    {
        printf("请输入要更新的行数:");
        scanf("%s", str);
        pos = atoi(str);
        if (pos <= 0 || pos > *count)
        {
            printf("您的输入不合法(信不信我批斗你)\n");
        }
        else
        {
            break;
        }
    }
    result = upDataSimple(container, count, pos, value);
    if (result == 1)
    {
        printf("更新成功\n");
    }
    else
    {
        printf("更新失败\n");
    }
}
/*
    功能:操作
    参数:
        container:数据容器
        count:有效行数

    返回值:无
*/
void opration(char *container[], int *count)
{
    _flushall();
    char op;
    char value[200][100]; //这个必须定义在这里，否则其他函数访问这个地址的时候就没法访问了,写成二维数组这样就可以多次插入了
    int insertNum = 0;    //插入的次数
    char updata[200][100];
    int updataNum = 0;

    printf("\n");
    printf("操作说明:|| 1:显示数据 || 2:查找数据 || 3:插入数据 || 4:删除数据 || 5:更新数据 || 0:退出\n");
    printf("\n");

    while (1)
    {
        printf("请输入操作代号 : ");
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
            printf("您的输入有误(信不信我批斗你)\n");
        }
    }

    while (op != '0')
    {
        switch (op)
        {
        case '1':
            showData(container, count);
            break;
        case '2':
            findData(container, count);
            break;
        case '3':
            InsertData(container, SIZE, count, value[insertNum]); //将数组的第插入次数个地址传给函数InsertData
            insertNum++;
            break;
        case '4':
            DeleteData(container, count);
            break;
        case '5':
            UpData(container, count, updata[updataNum]);
            updataNum++;
            break;
        default:
            printf("请输入合法字符(信不信我批斗你)\n");
            break;
        }

        printf("\n");
        printf("操作说明:|| 1:显示数据 || 2:查找数据 || 3:插入数据 || 4:删除数据 || 5:更新数据 || 0:退出\n");
        printf("\n");
        printf("请继续您的操作 : ");
        _flushall();
        op = getchar();
        if (op == '0')
        {
            printf("退出成功\n");
        }
    }
}
