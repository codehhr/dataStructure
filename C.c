#include <stdio.h>
#include <stdlib.h>
#include <malloc.h> //��̬�ڴ����
#include <string.h>
#define SIZE 200                                                                 //�������ȡ200��
int readFile(char *container[], int size, char *fileName);                       //�ļ�����
void freeContainer(char *container[], int *count);                               //�ͷ��ļ�������ڴ�
void showData(char *container[], int *count);                                    //��ʾ����
int findSimple(char *container[], int *count, char *value);                      //��������
void findData(char *container[], int *count);                                    //�������ݷ�װ
int InsertSimple(char *container[], int size, int *count, int pos, char *value); //��������
void InsertData(char *container[], int size, int *count, char *value);           //�������ݷ�װ
int DeleteSimple(char *container[], int *count, int pos);                        //ɾ������
void DeleteData(char *container[], int *count);                                  //ɾ�����ݷ�װ
int upDataSimple(char *container[], int *count, int pos, char *value);           //��������
void UpData(char *container[], int *count, char *value);                         //�������ݷ�װ
void opration(char *container[], int *count);                                    //����

/*
    ʵ���ļ����룬�ļ�����
*/
int main()
{
    int count;
    char *container[SIZE];
    //input your code
    //�������ݲ�������Ч����
    count = readFile(container, SIZE, "C:\\Users\\18719\\Desktop\\DS1\\2016List2.csv");
    if (count != -1) //��ȡ�ɹ�����һ������
    {
        //������������
        _flushall();
        //��������
        opration(container, &count);
        //�ͷ��ڴ�ռ�
        freeContainer(container, &count);
    }
    return 0;
}

/*
    ����:�ļ�����
    �����б�:
            container:�����ļ���������һ�����飬�����б�����ָ��ÿһ�е��ڴ��ַ
            size:container�������������
            fileName:�ļ�·��
    ����ֵ:��ȡ����:��ȡ����Ч�������� ��ȡʧ��:-1

*/
int readFile(char *container[], int size, char *fileName)
{
    int c = 0;
    char buf[1024];
    //���ļ�������
    FILE *fp = fopen(fileName, "r");
    //�ļ���ʧ�ܣ����� 0����ʾ
    if (fp == NULL)
    {
        printf("�ļ� %s ��ʧ�ܣ������ļ�·��\n", fileName);
        return -1;
    }
    //�������ж���buf�У��ٽ�bufд��container������
    while (c < size && !feof(fp))
    {
        //��Ƭ
        fgets(buf, 1024, fp);
        //ȥ�����з�
        if (buf[strlen(buf) - 1] == 0x00000A)
        {
            buf[strlen(buf) - 1] = '\0';
        }
        //��ȡ��Ч��
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
    ����:�ͷ��ļ������ڴ�
    ����:
        container:��Ҫ�ͷ��ڴ������
        c:��Ч����
    ����ֵ:��
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
    ����:��ʾ
    ����:
        container:������ݵ�����
        count: ��Ч����
    ����ֵ:��
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
    ����:ƥ���������
    ����:
        container:������ݵ�����
        count: ��Ч����
        value:Ҫ���ҵ�ֵ
    ����ֵ:0:����ʧ�ܣ�����0���ز��ҵ����ڵڼ���
*/
int findSimple(char *container[], int *count, char *value)
{
    char *p;
    int j = 0;

    while (j < *count)
    {
        p = strstr(container[j], value); //����value���ݵ��������ݽ���������
        if (p != NULL)
        {
            return j + 1;
        }
        j++;
    }
    return 0;
}
/*
    ����:����
    ����:
        container:������ݵ�����
        count:��Ч����
    ����ֵ:��
*/
void findData(char *container[], int *count)
{
    char str[100];
    int j;
    printf("������Ҫ���ҵ�����:");
    scanf("%s", str);
    j = findSimple(container, count, str);
    if (j > 0)
    {
        printf("���ҳɹ�:λ��:%d ����:%s\n", j, container[j - 1]);
    }
    else
    {
        printf("��������ʧ��\n");
    }
}

/*
    ����:����
    ����:
        container:�洢����
        size:������󳤶�
        count:��Ч����
        pos:Ҫ�����λ��(��)
        value:Ҫ���������
    ����ֵ:0:����ʧ�� 1:�ɹ�

*/
int InsertSimple(char *container[], int size, int *count, int pos, char *value)
{
    int c = *count; //����Чֵ����һ��

    while (1)
    {
        //�ж��Ƿ���Լ�������
        if (c >= size)
        {
            printf("�������������ɼ�������\n");
            return 0;
        }
        else
        {
            container[c] = container[c - 1]; //���������һ��Ԫ������ƶ�һλ  ����Ӧ����ֵ�ƶ�����ַ��Ӧ���ƶ�
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
    ����:��һ����װ����
    ����:
        container:�洢����
        size:������󳤶�
        count:��Ч����
    ����ֵ:��

*/
void InsertData(char *container[], int size, int *count, char *value)
{
    char str[100];
    int pos;
    int result;
    printf("������Ҫ���������:");
    scanf("%s", value);
    while (1)
    {
        printf("������Ҫ���������:");
        scanf("%s", str);
        pos = atoi(str);
        if (pos <= 0 || pos > (*count + 1))
        {
            printf("�������벻�Ϸ�(�Ų�����������)\n");
        }
        else
        {
            break;
        }
    }
    result = InsertSimple(container, size, count, pos, value);
    if (result == 1)
    {
        printf("����ɹ�\n");
        *count = *count + 1; //������1
    }
    else
    {
        printf("����ʧ��\n");
    }
}
/*
    ����:ɾ��ĳһ��
    ����:
        container:��������
        count:��Ч����
        value:Ҫ���µ�ֵ
        pos:ɾ����λ��
    ����ֵ:0:ɾ��ʧ�ܣ�1:ɾ���ɹ�

*/
int DeleteSimple(char *container[], int *count, int pos)
{
    while (1)
    {
        if (0 == *count)
        {
            printf("�����ѿգ��޷�ɾ��");
            return 0;
        }
        //��������һ��
        if (pos == *count)
        {
            container[pos - 1] = NULL;
            return 1; //ɾ���ɹ�
        }
        container[pos - 1] = container[pos];
        pos++;
        if (pos == *count)
        {
            return 1; //ɾ���ɹ�
        }
    }
}
/*
    ����:��װɾ��
    ����:
        container:��������
        count:��Ч����
    ����ֵ:��
*/
void DeleteData(char *container[], int *count)
{
    char str[100];
    int pos;    //ɾ��λ�ã��У�
    int result; //ɾ������ֵ���
    while (1)
    {
        printf("������Ҫɾ��������:");
        scanf("%s", str);
        pos = atoi(str);
        if (pos <= 0 || pos > *count)
        {
            printf("�������벻�Ϸ�(�Ų�����������)\n");
        }
        else
        {
            break;
        }
    }
    result = DeleteSimple(container, count, pos);
    if (result == 1)
    {
        printf("ɾ���ɹ�\n");
        *count = *count - 1;
    }
    else
    {
        printf("ɾ��ʧ��\n");
    }
}
/*
    ����:����
    ����:
        container:��������
        count:��Ч����
        pos:Ҫ���µ�����
        value:���µ�ֵ
    ����ֵ:0:����ʧ�� 1:���³ɹ�
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
    ����:��װ����
    ����:
        container:��������
        count:��Ч����
        value:Ҫ���µ�ֵ����ʱ�ռ�
    ����ֵ:��
*/
void UpData(char *container[], int *count, char *value)
{
    char str[100];
    int pos;
    int result;
    printf("������Ҫ���µ�����:");
    scanf("%s", value);
    while (1)
    {
        printf("������Ҫ���µ�����:");
        scanf("%s", str);
        pos = atoi(str);
        if (pos <= 0 || pos > *count)
        {
            printf("�������벻�Ϸ�(�Ų�����������)\n");
        }
        else
        {
            break;
        }
    }
    result = upDataSimple(container, count, pos, value);
    if (result == 1)
    {
        printf("���³ɹ�\n");
    }
    else
    {
        printf("����ʧ��\n");
    }
}
/*
    ����:����
    ����:
        container:��������
        count:��Ч����

    ����ֵ:��
*/
void opration(char *container[], int *count)
{
    _flushall();
    char op;
    char value[200][100]; //������붨����������������������������ַ��ʱ���û��������,д�ɶ�ά���������Ϳ��Զ�β�����
    int insertNum = 0;    //����Ĵ���
    char updata[200][100];
    int updataNum = 0;

    printf("\n");
    printf("����˵��:|| 1:��ʾ���� || 2:�������� || 3:�������� || 4:ɾ������ || 5:�������� || 0:�˳�\n");
    printf("\n");

    while (1)
    {
        printf("������������� : ");
        _flushall();
        op = getchar();
        if (op == '0' || op == '1' || op == '2' || op == '3' || op == '4' || op == '5')
        {
            if (op == '0')
            {
                printf("�˳��ɹ�\n");
                return;
            }
            break;
        }
        else
        {
            printf("������������(�Ų�����������)\n");
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
            InsertData(container, SIZE, count, value[insertNum]); //������ĵڲ����������ַ��������InsertData
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
            printf("������Ϸ��ַ�(�Ų�����������)\n");
            break;
        }

        printf("\n");
        printf("����˵��:|| 1:��ʾ���� || 2:�������� || 3:�������� || 4:ɾ������ || 5:�������� || 0:�˳�\n");
        printf("\n");
        printf("��������Ĳ��� : ");
        _flushall();
        op = getchar();
        if (op == '0')
        {
            printf("�˳��ɹ�\n");
        }
    }
}
