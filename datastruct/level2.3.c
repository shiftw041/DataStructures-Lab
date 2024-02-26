#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>            
#define MAX_NUM 50
#define INFINITY 1024
typedef struct ArcNode
{   //弧结构
    int tailvex, headvex;       //弧尾弧头
    struct ArcNode *nextarc;    //弧尾相同的下一条弧
    float distance;             //两站距离
} ArcNode;

typedef struct VNode
{   //站点表头结构
    char name[MAX_NUM];         //站点名
    struct ArcNode *firstarc;   //指向第一条弧的指针
} VNode;

typedef struct ALGragh
{   //邻接表存储图结构
    struct VNode stops[MAX_NUM];//头结点数组
    int stopnum, linenum;       //站点数，线路数
} ALGragh;

int Searchstop(ALGragh *metro, char *aimstop)
{   //查找目标站点是否在图中，在则返回下标数，不在则返回站点总数stopnum
    int i = 0;
    while (i < metro->stopnum)
        if (!(strcmp(metro->stops[i].name, aimstop)))
            break;
        else i++;
    return i;
}

void CreateGragh(ALGragh *metro)
{   //输入若干线路构建地铁线路图，用邻接表存储
    int i = 0, j = 0, linen;
    char name[MAX_NUM] = {'\0'};
    while (i++ < metro->linenum)
    {   //循环读入各条线路
        scanf("%d", &linen);    //输入线路号
        float nextd = 0, pred = 0;
        int this = 0, pre = 0;
        ArcNode *p = (ArcNode *)calloc(1, sizeof(ArcNode));
        do
        {   //循环读入一条线路中的各个站点
            scanf("%s %f", name, &nextd);   //读入新站名和距离
            this = Searchstop(metro, name);
            if (this == metro->stopnum)
            {   //查找新站点是否在图中，在则返回下标，不在返回stopnum
                //将新站点下标设为最末，加入站点数组
                this = metro->stopnum;      
                strcpy(metro->stops[this].name, name);
                metro->stopnum++;
                metro->stops[this].firstarc = NULL;    //初始化站点表头
            }
            if (fabs(pred) > 1E-6)
            {   //和上一站的距离不为零，不是第一站，添加当前站到上一站的弧
                ArcNode *m = (ArcNode *)calloc(1, sizeof(ArcNode));
                m->distance = pred;
                m->tailvex = this, m->headvex = pre;
                m->nextarc = metro->stops[this].firstarc;
                metro->stops[this].firstarc = m;
                p->headvex = this;      //修改上一站到当前站的弧的弧头
            }
            if (fabs(nextd) > 1E-6)
            {   //到下一站的距离不为零，不是最后一站
                //分配当前站到下一站的弧的空间
                ArcNode *l = (ArcNode *)calloc(1, sizeof(ArcNode));
                l->tailvex = this;
                l->distance = nextd;
                pred = nextd;
                pre = this;
                l->nextarc = metro->stops[this].firstarc;
                //该站点指向的第一条弧设置为新的弧
                metro->stops[this].firstarc = l;    
                p = l;     //设置前指针指向当前弧，方便下一次循环访问
            }
        } while (fabs(nextd) > 1E-6);    //没读到尾站则循环继续
    }
}

void Shortpath(ALGragh *metro, char *first, char *last)
{   //迪杰斯特拉算法寻找两个站点之间的最短路径并输出
    //查找并存储两个目标站点的下标
    int i = Searchstop(metro, first), j = Searchstop(metro, last);
    int v, w, k, Path[MAX_NUM];
    float min, Dist[MAX_NUM], thisdist[MAX_NUM];
    //初始化标志数组，final[i] = 0说明j到i的最短路径没找到
    int final[MAX_NUM] = {0};
    ArcNode *p = metro->stops[j].firstarc;
    for (v = 0; v < metro->stopnum; v++)
    {   //初始化路径数组
        Dist[v] = INFINITY;         //存储j到v的目前最短路径长度
        Path[v] = v;                //存储j到v的目前最短路径的前驱
    }
    final[j] = 1, Dist[j] = 0, Path[j] = j;   //设置j到j已找到最短路
    for ( ; p != NULL; p = p->nextarc)
    {   //将j的邻边存入最短路径数组
        if (p->distance < Dist[p->headvex]){
            Dist[p->headvex] = p->distance;
            Path[p->headvex] = j;
        }
    }
    for (v = 0; final[i] == 0; v++)
    {   //每次循环计算得到j到某点的最短路径，
        //从最近的点算到最远的，直到j到i的最短路径已求得
        min = INFINITY;
        for (w = 0; w < metro->stopnum; w++)
        {   //找到目前离j最近的且未求得最短路径的站点k
            if (!final[w] && Dist[w] < min)
            {   
                k = w;
                min = Dist[w];
            }
        }
        final[k] = 1;    //标志已求得j到k的最短路径
        for (v = 0; v < metro->stopnum; v++)
        {   //初始化邻边数组
            thisdist[v] = INFINITY;     //存储当前站点到各站点的直接距离
        }
        for (p = metro->stops[k].firstarc; p != NULL; p = p->nextarc)
        {   //将k的邻边存入邻边数组
            thisdist[p->headvex] = p->distance;
        }
        for (w = 0; w < metro->stopnum; w++)
        {   //若经过k再到w的路径比当前j到w的路径更短，修正最短路径数组
            if (!final[w] && (min + thisdist[w]) < Dist[w])
            {
                Dist[w] = min + thisdist[w];
                Path[w] = k;
            }
        }
    }
    printf("%.2f ", Dist[i]);               //打印i到j的最短路径长度
    for (w = i; w != j; w = Path[w])        //打印首站及途经站点和间距
        printf("%s %.2f ", metro->stops[w].name, Dist[w] - Dist[Path[w]]); 
    printf("%s", metro->stops[j].name);     //打印终点站
}

int main()
{   //主函数实现邻接表构建地铁线路图，迪杰斯特拉算法计算两站点的最短路径
    char first[MAX_NUM], last[MAX_NUM];
    ALGragh Metro;                  //定义地铁路线图
    ArcNode *p = NULL;
    Metro.stopnum = 0;
    scanf("%d", &Metro.linenum);    //读入线路数
    CreateGragh(&Metro);            //建地铁站图
    scanf("%s %s", first, last);    //输入要查询的两个站点
    Shortpath(&Metro, first, last); //查找最短路径
    return 0;
}