#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_NUM 20
typedef struct DuLNode
{   //站点结点结构
    char stopname[MAX_NUM];         //存储站名
    float distance;                 //存储到下一站的距离，尾站则设置为0
    struct DuLNode *pre, *next;     //指向前一站和下一站
} DuLNode;

typedef struct DuLinkList
{   //地铁线路结构
    struct DuLNode *head, *tail;    //指向线路首尾站
    int line, stopnum;              //线路号和站点数
} DuLinkList;

void CreateDuL(DuLinkList *metro)
{   //构建地铁线路双向链表
    float d = 0;
    char name[MAX_NUM] = {'\0'};
    metro->head = metro->tail = NULL;
    metro->stopnum = 0;             //初始化地铁线路结构的各项
    DuLNode *p = NULL;              //定义遍历站点指针
    scanf("%d", &metro->line);      //录入线路号
    do
    {   //循环插入新站点
        scanf("%s %f", name, &d);
        DuLNode *l = (DuLNode *)malloc(sizeof(DuLNode));
        l->pre = l->next = NULL;
        strcpy(l->stopname, name), l->distance = d;
        if (metro->stopnum == 0)    //已输入站点数为0，将首尾站指针指向新站
            p = metro->head = metro->tail = l;
        else
        {   //新站不为首站，将其插入末端
            p->next = l;
            l->pre = p;
            p = l;
            metro->tail = l;
        }
        metro->stopnum++;           
    } while (fabs(d) > 1E-6);       // d = 0即尾站录入完成时结束循环
}

void OutputDuL(DuLinkList *metro)
{   //输出地铁线路
    DuLNode *p = metro->head;
    printf("%d ", metro->line);     //输出线路号
    while (p->next) 
    {   //循环输出非尾站及其到下一站距离
        printf("%s %.2f ", p->stopname, p->distance);
        p = p->next;
    }
    printf("%s", p->stopname);      //输出尾站名
}

int main()
{   //主函数实现地铁线双向链表的建立和输出
    int linenum, i = 0;
    scanf("%d", &linenum);
    //开辟空间存储各个地铁线路
    DuLinkList *const Metro = (DuLinkList *)malloc(sizeof(DuLinkList) * linenum); 
    while (i < linenum)             //循环构建地铁各线路
        CreateDuL(&(Metro[i++]));
    i = 0;
    while (i < linenum)
    {   //循环输出地铁各线路
        OutputDuL(&(Metro[i++]));
        if (i != linenum)           //没打印完，输出换行符
            printf("\n");
    }
    return 0;
}