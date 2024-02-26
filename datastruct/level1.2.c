#include <stdio.h>
#include <stdlib.h>
typedef struct StackNode
{   //栈结点结构
    int coef;
    int expn;
    struct StarkNode *next;
} StarkNode;

typedef struct LinkStark
{   //栈结构
    StarkNode *top;
    int count;
} LinkStark;

void push(LinkStark *S, int c, int e)
{   //入栈函数，指针次序翻转，新结点指向旧结点
    StarkNode *s = (StarkNode *)malloc(sizeof(StarkNode));
    s->coef = c;
    s->expn = e;
    if ( S->count == 0)
    {   //如果栈空，栈顶指针指向新分配的结点空间
        S->top = s;
        s->next = NULL;
    }
    else
    {   //栈非空
        s->next = S->top;
        S->top = s;
    }
    S->count++;
}

void OutputStark(LinkStark *s1, LinkStark *s2)
{   //降序输出多项式序列，逐项比较次幂大小输出
    int i, j;
    StarkNode *m, *n;
    m = s1->top;
    n = s2->top;
    i = s1->count + s2->count;  //需要输出的总项数
    for (j = 0; j < i; j++)
    {   //循环输出各项
        if ((m != NULL) && ((n == NULL) || (m->expn > n->expn)))
        {   //若栈1没有输出完，且栈2已全部输出或栈1的栈顶元素次幂比栈2的高
            //则输出栈1的最高项
            printf("%d %d", m->coef, m->expn);
            m = m->next;    //遍历指针指向前一个更低次幂项
        }
        else 
        {   //反之输出栈2元素
            printf("%d %d", n->coef, n->expn);
            n = n->next;
        }
        if (j != i)    //没到最后一项，输出间隔空格
            printf(" ");
    }
}
int main()
{
    LinkStark s1, s2;
    s1.count = s2.count = 0;//构建两个多项式栈，栈元素计数器设为0
    int c, e;
    while ((scanf("%d %d", &c, &e) != EOF))
    {   //将第一个多项式入栈
        if (getchar() == ';')
            break;
        if (s1.count == 5)
        {   //栈满，退出程序
            printf("Full!");
            system("pause");
            return 0;
        }
        push(&s1, c, e);
    }
    while(scanf("%d %d", &c, &e) != EOF )
    {   //将第二个多项式入栈
        if (s2.count == 5)
        {   //栈满，退出程序
            printf("Full!");
            system("pause");
            return 0;
        }
        push(&s2, c, e);
        if (getchar() == '\n')
            break;
    }
    OutputStark(&s2, &s1);  //降序输出两栈元素
    return 0;
}