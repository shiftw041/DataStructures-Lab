#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct PNode
{   //多项式结点结构
    float coef;
    int expn;
    struct PNode *next;
} PNode;

typedef struct Polyn
{   //多项式链表结构
    PNode *top;
    int count;
} Polyn;

void Insert(Polyn *S, float c, int e)
{   //单项式入栈函数，指针次序翻转，新结点指向旧结点
    PNode *s = (PNode *)calloc(1, sizeof(PNode));
    s->coef = c;
    s->expn = e;
    if ( S->count == 0)
    {   //若栈空，栈顶指针指向新分配的结点空间
        S->top = s;
    }
    else
    {   //栈非空
        s->next = S->top;
        S->top = s;
    }
    S->count++;
}

void AddPolyn(Polyn *s1, Polyn *s2, Polyn *s3)
{   //多项式s1与s2相加，结果降幂存入s3
    PNode *p1 = s1->top;
    PNode *p2 = s2->top;    //链表的遍历指针
    PNode *p3;
    PNode *l;
    while (p1 != NULL || p2 != NULL)
    {   //循环遍历比较两多项式栈顶项，降幂存入栈s3
        if (p1 != NULL && ((p2 == NULL) || (p1->expn > p2->expn)))
        {   //若栈1没有输出完，且栈2已全部输出或栈1的栈顶元素次幂比栈2的高
            //将栈1的栈顶元素存入栈3
            l = (PNode *)calloc(1, sizeof(PNode));
            l->expn = p1->expn;
            l->coef = p1->coef;
            l->next = NULL;
            if (s3->count == 0)
            {   //栈3为空
                s3->top = l;
                p3 = l;
            }
            else
            {   //栈3不为空
                p3->next = l;
                p3 = l;
            }
            p1 = p1->next;
            s3->count++;
        }
        else if (p2 != NULL && ((p1 == NULL) || (p1->expn < p2->expn)))
        {   //若栈2没有输出完，且栈1已全部输出或栈2的栈顶元素次幂比栈1的高
            //将栈2的栈顶元素存入栈3
            l = (PNode *)calloc(1, sizeof(PNode));
            l->expn = p2->expn;
            l->coef = p2->coef;
            if (s3->count == 0)
            {
                s3->top = l;
                p3 = l;
            }
            else
            {
                p3->next = l;
                p3 = l;
            }
            p2 = p2->next;
            s3->count++;
        }
        else if (p1->expn == p2->expn)
        {   //栈1和栈2的栈顶元素次幂相同
            if (fabs(p1->coef + p2->coef) <= 1e-6)
                 ;   //两项系数相加为0，无需在栈3增加新元素
            else
            {   //两项之和不为0，将和元素存入栈3
                l = (PNode *)calloc(1, sizeof(PNode));
                l->expn = p1->expn;
                l->coef = p1->coef + p2->coef;
                if (s3->count == 0)
                {
                    s3->top = l;
                    p3 = l;
                }
                else
                {
                    p3->next = l;
                    p3 = l;
                }
                s3->count++;
            }
            p1 = p1->next;
            p2 = p2->next;
        }
    }
}

void OutputPolyn(Polyn *s)
{   //输出多项式
    PNode *l = s->top;
    int a = 1;
    if(s->count == 0)   //栈为空
        printf("0 0");
    else 
    {   //栈不为空，循环输出各项
        while (l != NULL)
        {
            if (l->coef != 0)
            {
                printf("%g %d", l->coef, l->expn);
                if (a != s->count)  //没到最后一项，输出间隔空格
                    printf(" ");
            }
            l = l->next;
            a++;
        }
    }
}

int main()
{
    Polyn s1, s2, s3;    //两个输入多项式和结果多项式
    s1.count = s2.count = s3.count = 0;   //计数器初始化
    char d, f;
    float c;
    int e;
    while ((scanf("%f %d", &c, &e) != EOF))
    {   //第一个多项式的元素降幂插入链表
        Insert(&s1, c, e);
        getchar();
        if ((d = getchar()) == '+' || d == '-' )
        {   //读取系数前的符号
            if ((f = getchar()) == ' ')
                break;  //读到的符号d是运算符，栈1输入完毕，退出
            else
            {   //读到的符号d是系数的符号，将字符f和d退回输入流
                ungetc(f, stdin);
                ungetc(d, stdin);
            }
        }
        if (d != '+' && d != '-')   
            //读到的字符不是正负号，将其退回输入流
            ungetc(d, stdin);
    }
    f = d;
    if (f == '+')
    {   //执行加运算
        while (scanf("%f %d", &c, &e) != EOF)
        {   //第二个多项式的元素降幂次插入链表
            Insert(&s2, c, e);
            if (getchar() == '\n')
                break;
        }
    }
    else
    {   //执行减运算，多项式2的系数取反
        while (scanf("%f %d", &c, &e) != EOF)
        {   //第二个多项式的元素取反后降幂次插入链表
            Insert(&s2, -c, e);
            if (getchar() == '\n')
                break;
        }
    }
    AddPolyn(&s1, &s2, &s3);    //将两个多项式的和存入s3
    OutputPolyn(&s3);           //打印结果
    return 0;
}