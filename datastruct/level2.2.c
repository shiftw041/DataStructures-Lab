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
        if (metro->stopnum == 0)    //已输入站点数为0，首尾站指针指向新站
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

void AddDuLNode(int linen, DuLinkList *metro)
{   //在linen号线路中增加新站点
    float pred = 0, nextd = 0;      //分别存储当前站到前后站的距离
    char prestop[MAX_NUM], stop[MAX_NUM];   
    while (metro->stopnum != 0) 
        if (metro->line == linen)   //找到对应线路
            break;
        else 
            metro++;
    scanf("%f %f", &pred, &nextd);  //读取间距
    DuLNode *p = metro->head, *q = NULL;
    if (fabs(pred) < 1E-6)
    {   //增加站点与上一站的距离为0，为首站，直接插入并修改原首站点数据
        while (p != NULL)
        {   //遍历链表判断是否有与新站同名的站点
            if ((!strcmp(p->stopname, stop)))
            {
                printf("增加失败，已有同名站点");
                return;
            }
            p = p->next;
        }
        scanf("%s", stop);
        DuLNode *l = (DuLNode *)malloc(sizeof(DuLNode));
        l->pre = l->next = NULL;
        l->distance = nextd, strcpy(l->stopname, stop);
        metro->head->pre = l;
        l->next = metro->head;
        metro->head = l;
    }
    else if (fabs(nextd) < 1E-6)
    {   //增加站点与下一站的距离为0，为尾站，直接插入并修改原尾站点数据
        while (p != NULL)
        {   //遍历链表判断是否有与新站同名的站点
            if ((!strcmp(p->stopname, stop)))
            {
                printf("增加失败，已有同名站点");
                return;
            }
            p = p->next;
        }
        scanf("%s", stop);
        DuLNode *l = (DuLNode *)malloc(sizeof(DuLNode));
        l->pre = l->next = NULL;
        l->distance = 0, strcpy(l->stopname, stop);
        metro->tail->next = l, metro->tail->distance = pred;
        l->pre = metro->tail;
        metro->tail = l;
    }
    else 
    {   //增加站点不是首尾站，查找前一站
        scanf("%s %s", prestop, stop);             //读取前一站名和新站名
        while (p != NULL)
        {   //遍历链表查找新站的前一站，同时判断是否有与新站同名的站点
            if ((!strcmp(p->stopname, prestop)))
                q = p;                             //找到前一站，记录
            if ((!strcmp(p->stopname, stop))){
                printf("增加失败，已有同名站点");   
                return ;
            }
            p = p->next;
        }
        if (q == NULL)
        {   //没有找到前一站
            printf("增加失败，没有与输入的增加位置前一站点同名的站点");
            return ;
        }
        else 
        {   //找到前一站且新站不在原地铁线路中，插入新站并修改前后站的数据
            DuLNode *l = (DuLNode *)malloc(sizeof(DuLNode));
            l->pre = l->next = NULL;
            l->distance = nextd, strcpy(l->stopname, stop);
            l->next = q->next;
            q->next->pre = l,q->next = l, q->distance = pred;
            l->pre = q, l->distance = nextd;
        }
    }
    metro->stopnum++;
    OutputDuL(metro);   //增加站点成功，打印该条线路
    return ;
}

void DeleteDuLNode(int linen, DuLinkList *metro)
{   //在linen号线路中删除站点
    char stop[MAX_NUM];
    while (metro->stopnum != 0)
        if (metro->line == linen)   //找到对应线路
            break;
        else
            metro++;
    DuLNode *p = metro->head, *q = NULL;
    scanf("%s", stop);   //读取要删除的站名
    while (p != NULL)    //遍历链表寻找要删除的站点
        if ((!strcmp(p->stopname, stop)))
            break;       //找到要删除的站点
        else
            p = p->next;
    
    if (p == NULL) 
    {   //没找到同名站点
        printf("删除失败，没有同名站点");
        return ;
    }
    else 
    {   //找到同名站点，判断站点位置后删除
        if (p == metro->head)        //删除首站
            p->next->pre = NULL, free(p);
        else if (p == metro->tail) 
        {   //删除尾站
            metro->tail = p->pre;
            metro->tail->distance = 0;
            free(p);  
        }
        else 
        {   //删除非首尾站点
            p->pre->next = p->next;
            p->next->pre = p->pre;
            //修改删除站点前后两站之间的距离
            p->pre->distance += p->distance;
            free(p);
        }
    }
    metro->stopnum--;
    OutputDuL(metro);
}

int main()
{   //主函数实现地铁线双向链表的建立和输出，并实现增加或删除站点
    int linenum, i = 0;             
    char op[MAX_NUM];               //操作名         
    scanf("%d", &linenum);          //录入线路总数
    //开辟空间存储各个地铁线路
    DuLinkList *const Metro = (DuLinkList *)malloc(sizeof(DuLinkList) * linenum);
    while (i < linenum)             //循环构建地铁各线路
        CreateDuL(&(Metro[i++]));
    i = 0;
    while (i < linenum) 
    {   //循环输出地铁各线路
        OutputDuL(&(Metro[i++]));
        printf("\n");
    }
    scanf("%s%d", op, &i);          //读入操作和线路号
    if (!strcmp(op, "add"))
        AddDuLNode(i, Metro);       //增加站点
    else if (!strcmp(op, "delete"))
        DeleteDuLNode(i, Metro);    //删除站点
    return 0;
}