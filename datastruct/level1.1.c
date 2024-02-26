#include <stdio.h>
#include <stdlib.h>
typedef struct QNode
{   //循环队列结点结构
	int data;
	struct QNode *next;
} QNode,*Qptr;

typedef struct
{   //循环队列链表结构
	Qptr front,rear;
} Queue;
 
void InitQueue(Queue *Q)
{   //建立空队列
	Q->front = Q->rear = (Qptr)malloc(sizeof(QNode));
	Q->rear->next = Q->front;
}
 
void EnQueue(Queue *Q, int e)
{   //元素入队
	Qptr s = (Qptr)malloc(sizeof(QNode));
	Q->rear->data = e;
	s->next = Q->rear->next;
	Q->rear->next = s;
	Q->rear = s;
}

int main()
{  
    int n, i, k, e;
    scanf("%d %d %d", &n, &i, &k);
    if (k > 8)
    {   //输出个数大于存储最大容量
        printf("Error");
        system("pause");
		return 0;
    }
    else if (n < k)
    {   //输出个数大于存储个数
        printf("Fault");
		system("pause");
        return 0;
    }
    if (n >= 8)  
        printf("Full ");
    Queue Q;
	Qptr p;
	InitQueue(&Q);
	for (i = 0; i < n && i < 8; i++)
    {   //循环输入数据,最多读入8个
		scanf("%d", &e);
        EnQueue(&Q, e);
    }
	p = Q.front;
    for (i = 0; i < k; i++)
    {   //打印k个数据
        printf("%d", p->data);
        if (i != k-1)
            printf(" ");
		p = p->next;
    }
    return 0;
}