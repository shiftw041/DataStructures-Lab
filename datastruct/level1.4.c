#include<stdio.h>
#include<stdlib.h>
#include<math.h>
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

char operator;    //运算符
int isend = 0;    //结束判断标志

void IsEnding(char *s)
{   //判断多项式是否输入完毕
    if ((*s == ')') && (*(s + 2) == '('))
    {   //第一个多项式输入完毕
        operator = *(s+1);  //保存运算符
        isend = 2;    //设置结束判断标志值
    }
    else if (*s == ')' && *(s+2) != '(')    //第二个多项式输入完毕，
        isend = 1;
}

char GetSignChar(char **s)
{   //取得系数前的正负号
    char *p = *s;
    if ( *p == '+' || *p == '-' ) 
    {   //读到系数符号
        *s = p + 1;    
        return (*p);
    }
    return '+';
}

void ParseMonomial(char **s, float *c, int *e)
{   //从输入流中读取一个单项式的系数和指数
	float coef = 1;
    float little = 0;
    int expn = 0, n = 1;    //设置单项式初值为x
	if (**s >= '0' && **s <= '9')
    {   //系数不为1，循环读取字符以读取系数
		coef = 0;
		while (**s >= '0' && **s <= '9')
        {
			coef = coef*10 + **s -'0';
			(*s)++;
		}
	}
    if (**s == '.')
    {   //系数为小数，则循环读取字符以读取小数部分
        (*s)++;
        while (**s >= '0' && **s <= '9')
        {
            little = **s - '0';
            for (int i = 0; i < n; i++)
            {
                little /= 10;
            }
			coef += little;
			(*s)++;
            n++;
        }
    }
	if (!(**s == 'x' && *(*s + 1) == '^') && (**s == 'x'))
    {   //修改指数为1
		(*s)++;
		expn = 1;
	}
	else if (**s=='x' && *(*s+1)=='^')
    {   //指数不是1，则循环读取字符以读入指数
		(*s) += 2;
		int tag = 1;
		expn = 0;
		if (**s == '-')
            //指数为负，设置符号标志tag
            tag = -1, (*s)++;     
		while (**s >= '0' && **s <= '9')
        {   //循环读取指数
			expn = expn * 10 + **s - '0';
			(*s)++;
		}
		expn = expn * tag;
	}   //如果都不是以上情况，则说明单项式为x,无需改变初值
    *c = coef;
    *e = expn;
}

void Insert(Polyn *s, float c, int e)
{   //将元素插入链表中合适位置，构建降次幂反指多项式链表
    if (fabs(c) > 1E-6)
    {   //系数不为0则创建结点
        PNode *l = (PNode*)malloc(sizeof(PNode));
        l->coef = c;
        l->expn = e;
        l->next = NULL;
        PNode *p = s->top;
        if (s->count == 0)
        {   //链表为空，直接放入元素
            s->top = l;
        }
        else if ((s->top->expn) < e)
        {   //插入元素次幂最大，放到末尾
            l->next = s->top;
            s->top = l;
        }
        else if ((s->top->expn) == e)
        {   //插入元素次幂和表中最高次数相等，相加后修改结点
            if (!(fabs(s->top->coef + c) <= 1E-6))
                s->top->coef = s->top->coef + c;
            else
            {   //相加系数为零，删除结点
                s->top = s->top->next;
                s->count -= 2;
            }
            free(l);
        } 
        else if (s->count != 0)
        {   //插入元素不放在最后，往前遍历链表直到找到合适位置插入
            while (p->next != NULL && (p->next->expn > e))     
                //不断向前遍历，直到找到合适位置
                p = p->next;
            if (p->next == NULL || p->next->expn < e)
            {   //找到合适位置，插入元素
                l->next = p->next;
                p->next = l;
            }
            else
            {   //找到系数相同项，修改结点
                if (!(fabs(p->next->coef + c) <= 1E-5))
                    p->next->coef = p->next->coef + c;
                else
                {   //相加系数为零，删除结点
                    p->next = p->next->next;
                    s->count -= 2;
                }
                free(l);
            }
        }
        s->count++;
    }
}

void AddPolyn(Polyn *s1, Polyn *s2, Polyn *s3)
{   //多项式s1与s2相加，结果存入s3，创建降幂反指链表
    PNode *p1 = s1->top;
    PNode *p2 = s2->top;    
    PNode *p3 = s3->top;    //链表的遍历指针
    while (p1 != NULL || p2 != NULL)
    {   //从两多项式最大的元素开始降幂相加，直到所有元素都被取完
        if (p1 != NULL && ((p2 == NULL) || (p1->expn > p2->expn)))
        {   //多项式1元素没有取完，且多项式2为空或者多项式1的元素指数更大
            PNode *l = (PNode *)malloc(sizeof(PNode));
            l->expn = p1->expn;
            l->coef = p1->coef;
            if (s3->count == 0)
            {   //多项式3为空，顶指针指向新结点
                s3->top = l;
                p3 = l;
            }
            else
            {   //将新结点插入到链表底端
                p3->next = l;
                p3 = l;
            }
            p1 = p1->next;
            s3->count++;
        }
        else if (p2 != NULL && ((p1 == NULL) || (p1->expn < p2->expn)))
        {   //多项式2元素没有取完，且多项式1为空或者多项式2的元素指数更大
            PNode *l = (PNode *)malloc(sizeof(PNode));
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
        {   //指数相同，系数相加后插入
            if (fabs(p1->coef + p2->coef) <= 1e-6)
                 ;  //相加后系数为0，不操作
            else
            {   //相加后系数不为0，插入和元素
                PNode *l = (PNode *)malloc(sizeof(PNode));
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
    if (s3->count != 0)
        p3->next = NULL;    //将链表底端设置为NULL
    else p3 = NULL;
}

void OutputPolyn(Polyn *s)
{   //打印多项式
    if (s->count != 0)
    {   //元素个数不为0，从第一项开始打印
        PNode *p = s->top; 
        while (p)
        {   //循环打印每一个单项式
            if (p->expn == 0)
            {   //打印常数项         
		        printf("%g", p->coef);
                p = p->next;
                if (p)    
                    //没到末尾，输出下一项的运算符
                    if (p->coef > 0) 
                        printf("+");
                continue;     //继续打印下一项
	        }
            if (p->coef != 1 && p->coef != -1)    
                //系数不为-1、1，正常打印
                printf("%g", p->coef);
            if (p->coef == -1)
                printf("-");
            printf("x");
            if (p->expn == 1)
            {   //指数为1，则不打印指数
                p = p->next;
                if (p) 
                    if (p->coef > 0) 
                        printf("+");
                continue;
            }
            printf("^%d", p->expn);
            p = p->next;     //打印完不为0的指数后打印下一项的运算符
            if (p) 
                if (p->coef > 0) 
                    printf("+");
        }
    }
    else if (s->count == 0)
    {   //元素个数为0，直接打印0
        printf("0");
    }
}

int main()
{
    Polyn s1, s2, s3;    //两个输入多项式和结果多项式
    s1.count = s2.count = s3.count = 0;     //项数计数器初始化
    s1.top = s2.top = s3.top = NULL;
    float c;        //系数
    int e;          //指数
    char sign;      //系数符号标志
    //保存输入流的字符数组，跳过第一个'('
    char linebuffer[1024]={0}, *s = &(linebuffer[1]);
    //读取输入流中的一行存入
    fgets(linebuffer, sizeof(linebuffer), stdin);   
    while (isend != 2)
    {   //结束标志不为2，多项式1没有读完,继续读取
        sign = GetSignChar(&s);         //读取系数前的符号
        ParseMonomial(&s, &c, &e);      //从输入流读入一个系数和指数
        if (sign == '-')    
            //符号为负，系数取相反数
            c = 0.000000 - c;
        Insert(&s1, c, e);      //将单项式插入多项式链表
        IsEnding(s);            //判断多项式是否输入完毕
    }
    s += 3;   //跳过两个多项式中间的符号，准备读入多项式2
    if (operator == '-')
    {   //运算符为负，多项式2的系数设为相反数
        while (isend != 1)
        {   //读入多项式2，流程与1相同
            sign = GetSignChar(&s);
            ParseMonomial(&s, &c, &e);
            if (sign == '-')
                c = 0.000000 - c;
            Insert(&s2, -c, e);
            IsEnding(s);
        }
    }
    else
    {   //运算符为正，正常读入多项式2
        while (isend != 1){
            sign = GetSignChar(&s);
            ParseMonomial(&s, &c, &e);
            if (sign == '-')
                c = 0.000000 - c;
            Insert(&s2, c, e);
            IsEnding(s);
        }
    }
    AddPolyn(&s1, &s2, &s3);    //将两个多项式的和存入s3
    OutputPolyn(&s3);           //打印结果
    return 0;
}