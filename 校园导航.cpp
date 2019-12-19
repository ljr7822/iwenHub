#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <io.h>
#include <windows.h>
#define MAXVEX 50
#define INFINITY 32768
#define MAXL 20
#define MAXC 66

int visited[INFINITY];
int stack[INFINITY];
int count;

struct user //保存管理员帐号密码的结构体
{
    char id[20];  //管理员帐号
    char passwd[20];  //管理员密码
    struct user *next;
};
typedef struct //保存地点信息的结构体
{
    int No;    //校园地点序号
    char name[20];    //校园地点名
    char description[200];  //地点描述
}Vextype;
typedef struct //邻接矩阵
{
    int arcs[MAXVEX][MAXVEX];   //边集
    Vextype vex[MAXVEX];        //顶点集
    int vexnum;                 //顶点数目
    int arcnum;                 //边数目
}AdjMatrix;
typedef struct //坐标矩阵
{
    int point; //该点是否为校园地点
    char name[20]; //该点校园地点名
    int No; //该点校园地点序号
}SchoolMap;

typedef struct Node
{
    int date;  //队列元素的值，存储地点序号
    struct Node *next;
}LinkQueueNode;
typedef struct
{
    LinkQueueNode *front; //头指针
    LinkQueueNode *fear; //尾指针
}LinkQueue;
int IsEmpty(LinkQueue *Q) //队列判空
{
    if(Q->front == Q->fear)
        return 1;
    else
        return 0;
}
int InitQueue(LinkQueue *Q)//队的初始化
{
    Q->front = (LinkQueueNode *)malloc(sizeof(LinkQueueNode));
    if(Q->front != NULL) {
        Q->fear = Q->front;
        Q->front->next = NULL;
        return 1;
    }
    else {
        return 0;
    }
}
int EnterQueue(LinkQueue *Q, int x) //入队
{
    LinkQueueNode *NewNode;
    NewNode = (LinkQueueNode *)malloc(sizeof(LinkQueueNode));
    if(NewNode != NULL) {
        NewNode->date = x;
        NewNode->next = NULL;
        Q->fear->next = NewNode;
        Q->fear = NewNode;
        return 1;
    }
    else {
        return 0;
    }
}
int DeleteQueue(LinkQueue *Q, int *x) //出队
{
    LinkQueueNode *p;
    if(Q->front == Q->fear)
        return 1;
    p = Q->front->next;
    Q->front->next = p->next;
    if(Q->fear == p)
        Q->fear = Q->front;
    *x = p->date;
    free(p);
    return 1;
}

/*###################################################################################################*/
// 用户的说明界面
void Instruction1()
{
    system("cls");
    printf("\n\n");
    printf("\t\t\t\t|================================================|\n");
    printf("\t\t\t\t|*                                              *|\n");
    printf("\t\t\t\t|*      ***欢迎使用西南大学荣昌校区导航***      *|\n");
    printf("\t\t\t\t|*                                              *|\n");
    printf("\t\t\t\t|*----------------------------------------------*|\n");
    printf("\t\t\t\t|*                                              *|\n");
    printf("\t\t\t\t|*         欢迎来到西南大学荣昌校区！我是世界上 *|\n");
    printf("\t\t\t\t|* 最机智的导航 小李 什么？足球场在哪？不知道怎 *|\n");
    printf("\t\t\t\t|* 么走？最近的路线是啥呀？哈哈~~这些问题都难不 *|\n");
    printf("\t\t\t\t|* 倒我哦！在这里你可以查看校园平面图，为你展示 *|\n");
    printf("\t\t\t\t|* 校园地点的总体分布，还可以查询某个地点的相关 *|\n");
    printf("\t\t\t\t|* 信息，以及两地点间的最短路径、最优路径、最佳 *|\n");
    printf("\t\t\t\t|* 布网方案，只要你有问题，我都可以即时为你解答 *|\n");
    printf("\t\t\t\t|* 呢~~有木有崇拜我的感觉>o< 吼吼，赶快来体验吧 *|\n");
    printf("\t\t\t\t|* 一定要记得我呦~~我是 小李，我为西容代言QWQ   *|\n");
    printf("\t\t\t\t|*                                              *|\n");
    printf("\t\t\t\t|*==============================================*|\n");
    printf("\n\n");
    printf("\n\t\t\t\t\t\t按任意键返回...");
    getch();
}
// 管理员的说明界面
void Instruction2()
{
    system("cls");
    printf("\n\n");
    printf("\t\t\t\t|================================================|\n");
    printf("\t\t\t\t|*                                              *|\n");
	printf("\t\t\t\t|*      ***欢迎使用西南大学荣昌校区导航***      *|\n");
    printf("\t\t\t\t|*                                              *|\n");
    printf("\t\t\t\t|*----------------------------------------------*|\n");
    printf("\t\t\t\t|*                                              *|\n");
    printf("\t\t\t\t|*         欢迎来到西南大学荣昌校区！我是世界上 *|\n");
    printf("\t\t\t\t|* 最机智的导航 小李 什么？足球场在哪？不知道怎 *|\n");
    printf("\t\t\t\t|* 么走？最近的路线是啥呀？哈哈~~这些问题都难不 *|\n");
    printf("\t\t\t\t|* 倒我哦！在这里你可以查看校园平面图，为你展示 *|\n");
    printf("\t\t\t\t|* 校园地点的总体分布，还可以查询某个地点的相关 *|\n");
    printf("\t\t\t\t|* 信息，以及两地点间的最短路径、最优路径、最佳 *|\n");
    printf("\t\t\t\t|* 布网方案，只要你有问题，我都可以即时为你解答 *|\n");
    printf("\t\t\t\t|* 呢~~有木有崇拜我的感觉>o< 作为管理员，你还可 *|\n");
    printf("\t\t\t\t|* 以指挥我，比如添加或撤销地点啊、添加路线啊等 *|\n");
    printf("\t\t\t\t|* 等操作，也可以来注册新的管理员帐号，是不是很 *|\n");
    printf("\t\t\t\t|* 腻害！！我是 小李，我为西荣代言QWQ           *|\n");
    printf("\t\t\t\t|*                                              *|\n");
    printf("\t\t\t\t|*==============================================*|\n");
    printf("\n\n");
    printf("\n\t\t\t\t\t\t按任意键返回...");
    getch();
}
// 
void Print(AdjMatrix *G)
{
    int i,count = 0;
    printf("\t\t\t\t|================================================|\n");
    printf("\t\t\t\t|*                                              *|\n");
    printf("\t\t\t\t|*      ***欢迎使用西南大学荣昌校区导航***      *|\n");
    printf("\t\t\t\t|*                                              *|\n");
    printf("\t\t\t\t|*==============================================*|\n\n");
    for(i = 1; i <= G->vexnum; i++) {
        printf("    %d.%-8s\t",i,G->vex[i].name);
        count++;
        if(count == 7) {
            printf("\n");
            count = 0;
        }
    }
    printf("\n\n");
}
// 采用邻接矩阵创建无向图
int Create(AdjMatrix *G,SchoolMap M[MAXL][MAXC])
{
    int i,j,weight,m,n;

    FILE *fp1;
    fp1=fopen("路线信息.txt","r");

    //从"路线信息.txt"文件中读取校园图的景点数目和路线数目
    fscanf(fp1, "%d %d", &G->vexnum, &G->arcnum);

    //初始化邻接矩阵
    for(i = 1; i <= G->vexnum; i++)
        for(j = 1; j<= G->vexnum; j++) {
            G->arcs[i][j] = INFINITY;
    }
    //读取"路线信息.txt"文件中两点序号及距离，并赋值给邻接矩阵
    while(fscanf(fp1,"%d %d %d",&i,&j,&weight) != EOF) {
            G->arcs[i][j] = weight;
            G->arcs[j][i] = weight;
    }
    fclose(fp1);

    FILE *fp2;
    fp2 = fopen("地点介绍.txt", "rt");

    //从"地点介绍.txt"文件中读取校园图中的景点名及描述
    for(i = 1; i <= G->vexnum; i++) { 
        G->vex[i].No = i; // 将顶点集中的地点序号设为i值
		// fscanf()函数是根据数据格式(format)，从输入流(stream)中读入数据，存储到argument中，遇到空格和换行时结束。
		// 数据依次是：地点名称、、、地点简介
        fscanf(fp2, "%s %d %d %s", G->vex[i].name,&m,&n,G->vex[i].description);
        M[m][n].point = 1;
        M[m][n].No = i;
        strcpy(M[m][n].name,G->vex[i].name);
    }
    fclose(fp2);
    return 1;
}
// 显示地图函数
void Display(AdjMatrix *G,SchoolMap M[MAXL][MAXC])
{
    int i, j, m, n;
    system("cls");
    printf("\n\n");
    printf("\t\t\t\t|================================================|\n");
    printf("\t\t\t\t|*                                              *|\n");
	printf("\t\t\t\t|*      ***欢迎使用西南大学荣昌校区导航***      *|\n");
    printf("\t\t\t\t|*                                              *|\n");
    printf("\t\t\t\t|*==============================================*|\n\n");

	// 创建文件指针
    FILE *fp2;
    fp2=fopen("地点介绍.txt","r"); // 以读的方式打开文件

    //从"地点介绍.txt"文件中读取校园图中的景点名及描述
    for(i = 1; i <= G->vexnum; i++) 
	{ // i小于顶点数
        G->vex[i].No = i; // 将顶点集中的地点序号设为i值
		// fscanf()函数是根据数据格式(format)，从输入流(stream)中读入数据，存储到argument中，遇到空格和换行时结束。
		// 数据依次是：地点名称、x坐标、y坐标、地点简介
        fscanf(fp2, "%s %d %d %s", G->vex[i].name,&m,&n,G->vex[i].description);
        M[m][n].point = 1; // 将该位置标记为1，便于下面用来打印
        strcpy(M[m][n].name, G->vex[i].name); // strcpy()是把含有'\0'结束符的字符串复制到另一个地址空间，返回值的类型为char*。
    }
    fclose(fp2); // 关闭文件
    int count = 0;
    for(i = 1; i <= MAXL; i++) {
        printf("\t\t\t");
		// 没有地点信息的坐标位置就用、代替
        for(j = 1; j <= MAXC; j++) {
            if(M[i][j].point == 1) {
                printf("%s",M[i][j].name);
                j += strlen(M[i][j].name);
                j--;
            }
            else{
                printf("`");
            }
        }
        printf("\n");
    }
    printf("\n      ********************************************校园路线情况如下***********************************************\n\n");
    // 遍历矩阵，有边输出 类似于二维数组的遍历
    for(i = 1; i <= G->vexnum; i++) {
        for(j = 1; j <= G->vexnum; j++) {
            if(G->arcs[i][j] != INFINITY && G->arcs[i][j] != 0) {
				// 如果两点有路可通，就将其打印，并且打印其距离
                printf("      %-8s <---> %8s: %3d m  ",G->vex[i].name,G->vex[j].name,G->arcs[i][j]);
                count ++;
                if(count == 3) {
                    printf("\n");
                    count = 0;
                }
            }
        }
    }
    printf("\n\t\t\t\t\t\t按任意键返回...");
    getch();// 吃掉回车符
}
// 查看景点信息
void Description(AdjMatrix *G)
{
    int i,j,choice;
    system("cls");
    printf("\n\n");
    Print(G); // 打印所有地点供用户选择
    do{
        printf("\t\t\t\t\t请输入要查询的地点序号(按'0'返回):");
        scanf("%d",&choice);
        if(choice == 0) break;
        if(choice < 0 || choice > G->vexnum)
            continue;
		// 打印景点名称及简介
        printf("\n\t\t--->%s:%s\n",G->vex[choice].name,G->vex[choice].description);
        printf("\t\t\t\t\t该地点的路线情况为:\n");
        for(i = 1; i <= G->vexnum; i++) {
            if(i == choice)
			{ // 遍历矩阵中该景点序号这一行的各点元素
                for(j = 1; j <= G->vexnum; j++) {
                    if(G->arcs[i][j] != INFINITY) { // 有边输出
                        printf("\t\t\t\t\t%-8s <---> %8s: %3d m\n", G->vex[i].name,G->vex[j].name,G->arcs[i][j]);
                    }
                }
            }
        }
        printf("\n\n");
    }while(1);
    printf("\n\t\t\t\t\t按任意键返回...");
    getch();
}
// 
void DFS(AdjMatrix *G, int m, int i, int end)
{
    int j,k;
    for(j = 1; j <= G->vexnum; j++){
        if(G->arcs[i][j] != INFINITY && visited[j] == 0) {
            visited[j] = 1;
            if(j == end) {
                count++;
                printf("★%d.",count);
                for(k = 1; k < m; k++) {
                    printf("%s->", G->vex[stack[k]].name);
                }
                printf("%s\n", G->vex[end].name);
                visited[j] = 0;
            }
            else {
                stack[m] = j;
                m++;
                DFS(G, m, j, end);
                m--;
                visited[j] = 0;
            }
        }
    }
}
// 查询所有简单路线
void SearchDFS(AdjMatrix *G)
{
    int start, end;
    system("cls");
    printf("\n\n");
    Print(G);
    do{
        printf("\t\t\t\t\t请输入起点序号:");
        scanf("%d",&start);
    }while(start<=0 || start>G->vexnum);
    do{
        printf("\t\t\t\t\t请输入终点序号:");
        scanf("%d",&end);
    }while(start<=0 || start>G->vexnum);
    printf("\n\t\t\t\t\t两地点间的所有路线情况为:\n\n");
    int m = 2;
    memset(stack,0,INFINITY);
    memset(visited,0,MAXVEX);
    stack[1] = start;
    visited[start] = 1;
    DFS(G, m, start, end);
    printf("\n\t\t\t\t\t按任意键返回...");
    memset(stack,0,INFINITY);
    memset(visited,0,MAXVEX);
    count = 0;
    getch();
}
// 
int NextAdjVertex(AdjMatrix *G, int w, int v)
{
    int i, j;
    for(i = w+1;i < G->vexnum; i++) {
        if(G->arcs[v][i] != INFINITY) {
            j = i;
            return j;
        }
    }
    return -1;
}
// 
void BFS(AdjMatrix *G, int start, int end)
{
    int vis[INFINITY];
    int i, num;
    int w, v;
    LinkQueue *Q;

    Q=(LinkQueue*)malloc(sizeof(LinkQueue));
    if(start == end)
        return;
    memset(vis, 0, INFINITY);
    vis[start] = 1;
    InitQueue(Q);
    EnterQueue(Q, start);
    while(Q->front != Q->fear){
        DeleteQueue(Q, &v);
        num = v;
        for(i = 1;i <= G->vexnum; i++){
            if(G->arcs[num][i] != INFINITY) {
                w = i;  //求出当前节点的第一个邻接点（求出序号）
                while(w != -1){
                    if(vis[w] == 0){
                        if(w == end){
                            BFS(G, start, num);
                            printf("%s->",G->vex[num].name);
                            return;
                        }
                        vis[w] = 1;
                        EnterQueue(Q, w);
                        w = NextAdjVertex(G, w, v);
                        //w是求的得第一个邻接点，v是相对w下一个邻接点(求出下一个邻接点的序号)
                    }
                    break;
                }
            }
        }
    }
}
// 查询最短路线（中转最少）
void SearchBFS(AdjMatrix *G)
{
    int start, end;
    system("cls");
    printf("\n\n");
    Print(G);
    do{
        printf("\t\t\t\t\t请输入起点序号:");
        scanf("%d",&start);
    }while(start<=0 || start>G->vexnum);
    do{
        printf("\t\t\t\t\t请输入终点序号:");
        scanf("%d",&end);
    }while(start<=0 || start>G->vexnum);

    if(G->arcs[start][end] != INFINITY)
        printf("\n\t\t\t\t\t从%s到%s不需要中转，可以直接到达!\n",G->vex[start].name,G->vex[end].name);
    if(G->arcs[start][end] == INFINITY) {
        printf("\n\t\t\t\t\t从%s到%s中转次数数最少的路径为:\n\n",G->vex[start].name,G->vex[end].name);
        printf("\t\t\t☆☆☆");
        BFS(G, start, end);
        printf("%s\n",G->vex[end].name);
    }
    printf("\n\t\t\t\t\t按任意键返回...");
    getch();
}
// Dijkstra算法
void Dijkstra(AdjMatrix *G, int start, int end, int dist[], int path[][MAXVEX])
{
    int mindist, i, j, k, t = 1;
    for(i = 1; i <= G->vexnum; i++) {
        dist[i] = G->arcs[start][i];  //对dist数组初始化
        if(G->arcs[start][i] != INFINITY)
            path[i][1] = start;  //如果该弧存在，则path[i][1]为源点
    }
    path[start][0] = 1; //start加入到S中
    for(i = 2; i <= G->vexnum; i++) {  //寻找各条最短路径
        mindist = INFINITY;
        for(j = 1; j <= G->vexnum; j++)
            if(!path[j][0] && dist[j] < mindist) {
                k = j;
                mindist = dist[j];
            }

            if(mindist == INFINITY)
                return ;
            path[k][0] = 1;   //找到最短路径，将该点加入到S集合中
            for(j = 1; j <= G->vexnum; j++) {  //修改路径
                if(!path[j][0] && G->arcs[k][j] < INFINITY && dist[k]+G->arcs[k][j] < dist[j]) {
                    dist[j] = dist[k] + G->arcs[k][j];
                    t = 1;
                    while(path[k][t] != 0) {
                        path[j][t] = path[k][t];
                        t++;
                    }
                    path[j][t] = k;
                    path[j][t+1] = 0;
                }
            }
    }

    for(i = 1; i <= G->vexnum; i++)
        if(i == end)
            break;
    printf("\n    ★★★%s--->%s的最短路线为: 从%s",G->vex[start].name,G->vex[end].name,G->vex[start].name);
    for(j = 2; path[i][j] != 0; j++) {
        printf("->%s",G->vex[path[i][j]].name);
    }
    printf("->%s, 距离为%d m\n",G->vex[end].name,dist[i]);
    printf("\n\t\t\t\t\t按任意键返回...");
    getch();
}
// Dijkstra算法求：从起点建筑到各终点建筑的最短路线
void Shortcut(AdjMatrix *G)
{
    int start, end;
    int dist[MAXVEX];
    int path[MAXVEX][MAXVEX] = {{0}};
    system("cls");
    printf("\n\n");
    Print(G);
    do{
        printf("\t\t\t\t\t请输入起点:");
        scanf("%d",&start);
    }while(start<=0 || start>G->vexnum);
    do{
        printf("\t\t\t\t\t请输入终点:");
        scanf("%d",&end);
    }while(end<=0 || end>G->vexnum);
    Dijkstra(G,start,end,dist,path);
}
// prim算法构建最小生成树
void Prim(AdjMatrix *G, int start)
{
    struct
    {
        int adjvex;
        int lowcost;
    }closedge[MAXVEX];
    int i, e, k, m, min;
    closedge[start].lowcost = 0;

    // 对除了出发点以外deep所有顶点初始化对应的closedge数组
    for(i = 1; i <= G->vexnum; i++) {
        if(i != start) {
            closedge[i].adjvex = start;
            closedge[i].lowcost = G->arcs[start][i];
        }
    }

    for(e = 1; e <= G->vexnum-1; e++)  //控制选中的n-1条符合条件的边
    {
        //选择权值最小的边
        min = INFINITY;
        for(k = 1; k <= G->vexnum; k++) {
            if(closedge[k].lowcost != 0 && closedge[k].lowcost < min) {
                m = k;
                min = closedge[k].lowcost;
            }
        }
        printf("\t\t\t\t\t从%s---%s:%d m\n", G->vex[closedge[m].adjvex].name,G->vex[m].name,closedge[m].lowcost);
        closedge[m].lowcost = 0;

        for(i = 1; i <= G->vexnum; i++) {
            if(i != m && G->arcs[m][i] < closedge[i].lowcost) {
                closedge[i].lowcost = G->arcs[m][i];
                closedge[i].adjvex = m;
            }
        }
    }
    printf("\n\t\t\t\t\t按任意键返回...");
    getch();
}
// 查询最小生成树
void MiniSpanTree(AdjMatrix *G)
{
    int i,count=0;
    int start;
    system("cls");
    printf("\n\n");
    Print(G);
    do{
        printf("\t\t\t\t\t请输入起点序号:");
        scanf("%d",&start);
    }while(start<=0 || start>G->vexnum);
    for(i = 1; i <= G->vexnum; i++){
        if(G->arcs[start][i] != INFINITY)
            count++;
    }
    if(count == 0) {
        printf("\n\t\t\t\t\t按任意键返回...");
        getch();
    }
    else {
        printf("\n");
        Prim(G, start);
    }
}
// 增加新地点
void AddPoint(AdjMatrix *G,SchoolMap M[MAXL][MAXC])
{
    int i,j;
    int x,y;
    system("cls");
    printf("\n\n");
    Print(G);
    printf("\t\t\t\t\t请输入增加的地点名称:");
    scanf("%s",G->vex[++G->vexnum].name);
    G->vex[G->vexnum].No = G->vexnum;
    printf("\t\t\t\t\t请输入该地点的简介:");
    scanf("%s",G->vex[G->vexnum].description);
    do{
        printf("\t\t\t\t\t请输入该地点的坐标(x,y):");
        scanf("%d,%d",&x,&y);
    }while((x<=0 || x>=20) || (y<=0 || y>=63));
    M[x][y].point = 1;
    M[x][y].No = G->vexnum;
    strcpy(M[x][y].name,G->vex[G->vexnum].name);
    for(i = 1; i <= G->vexnum; i++) {
        G->arcs[G->vexnum][i] = INFINITY;
        G->arcs[i][G->vexnum] = INFINITY;
    }

    FILE *fp;
    fp=fopen("路线信息.txt","wt");
    fprintf(fp,"%d %d\n",G->vexnum,G->arcnum);
    for(i = 1; i <= G->vexnum; i++)
        for(j = i; j <= G->vexnum; j++) {
            if(G->arcs[i][j] != INFINITY) {
                fprintf(fp,"%d %d %d\n",i,j,G->arcs[i][j]);
            }
        }
    fclose(fp);

    FILE *fp2;
    fp2=fopen("地点介绍.txt","at");
    fprintf(fp2,"\n%s %d %d %s", G->vex[G->vexnum].name, x, y, G->vex[G->vexnum].description);
    fclose(fp2);
    printf("\n\t\t\t\t\t该地点添加成功~快去地图信息里查看吧!\n");
    printf("\n\t\t\t\t\t按任意键进入下一步...");
    getch();
}
// 添加新路线
void AddEdge(AdjMatrix *G)
{
    int i,j;
    int start, end, weight;
    system("cls");
    printf("\n\n");
    Print(G);
    do{
        printf("\t\t\t\t\t请输入增加路线的起点建筑:");
        scanf("%d",&start);
    }while(start<=0 || start>G->vexnum);
    do{
        printf("\t\t\t\t\t请输入增加路线的终点建筑:");
        scanf("%d",&end);
    }while(end<=0 || end>G->vexnum);
    printf("\t\t\t\t\t请输入增加路线的距离:");
    scanf("%d",&weight);
    if(G->arcs[start][end]== INFINITY ) { //如果以前没有这条弧，则添加
        G->arcs[start][end] = weight;
        G->arcs[end][start] = weight;   //并将矩阵元素赋值，图边数+1
        G->arcnum ++;

        //重新将添加新路径之后的图的信息写入"景点信息.txt"文件中
        FILE *fp;
        fp=fopen("路线信息.txt","wt");
        fprintf(fp,"%d %d\n",G->vexnum,G->arcnum);
        for(i = 1; i <= G->vexnum; i++)
            for(j = i; j <= G->vexnum; j++) {
                if(G->arcs[i][j] != INFINITY) {
                    fprintf(fp,"%d %d %d\n",i,j,G->arcs[i][j]);
                }
            }
        fclose(fp);
        printf("\n\t\t\t\t\t这条路线添加成功~快去地图信息里查看吧!\n");
    }

    else {
        printf("\n\t\t\t\t\t这条路线已经存在了喔~请勿重复添加呢!\n");
    }
    printf("\n\t\t\t\t\t按任意键返回...");
    getch();
}
// 撤销地点函数
/*void DelPoint(AdjMatrix *G,SchoolMap M[MAXL][MAXC])
{
    int i,j,k;
    int x,y;
    int No;
    system("cls");
    printf("\n\n");
    Print(G);
    do{
        printf("\t\t\t\t\t请输入撤销的地点序号:");
        scanf("%d",&No);
    }while(No<=0 || No>G->vexnum);
    G->vexnum--;

    FILE *fp;
    fp=fopen("地点介绍.txt","wt");
    fprintf(fp,"%d %d\n",G->vexnum,G->arcnum);
    for(i = 1; i <= G->vexnum; i++)
        for(j = i; j <= G->vexnum; j++) {
            if(G->arcs[i][j] != INFINITY) {
                fprintf(fp,"%d %d %d\n",i,j,G->arcs[i][j]);
            }
        }
    fclose(fp);

    FILE *fp2;
    fp2=fopen("地点介绍.txt","wt");
    for(i = 1; i <= G->vexnum; i++) {
        for(j = 1; j <= G->vexnum; j++) {
            for(k = 1; k <= G->vexnum; k++) {
                if(i == M[j][k].No) {
                    x = j;
                    y = k;
                }
            }
        }
        fprintf(fp2,"%s %d %d %s\n", G->vex[i].name, x, y, G->vex[i].description);
    }
    fclose(fp2);
    printf("\n\t\t\t\t\t该地点撤销成功~快去地图信息里查看吧!\n");
    printf("\n\t\t\t\t\t按任意键进入下一步...");
    getch();
}
*/
// 撤销旧路线
void DelEdge(AdjMatrix *G)
{
    int i,j;
    int start, end;
    system("cls");
    printf("\n\n");
    Print(G);
    do{
        printf("\t\t\t\t\t请输入撤销路线的起点建筑:");
        scanf("%d",&start);
    }while(start<=0 || start>G->vexnum);
    do{
        printf("\t\t\t\t\t请输入撤销路线的终点建筑:");
        scanf("%d",&end);
    }while(end<=0 || end>G->vexnum);


    if(G->arcs[start][end] != INFINITY ) {  //如果以前有这条弧
        G->arcs[start][end] = INFINITY;
        G->arcs[end][start] = INFINITY;  //则将矩阵元素赋值为无穷，表示没有这条弧了
        G->arcnum --;     //并将图的边数-1

        ////重新将撤销弧之后的图的信息写入"景点信息.txt"文件中
        FILE *fp;
        fp=fopen("路线信息.txt","wt");
        fprintf(fp,"%d %d",G->vexnum,G->arcnum);
        fputc('\n',fp);
        for(i = 1; i <= G->vexnum; i++)
            for(j = i; j <= G->vexnum; j++) {
                if(G->arcs[i][j] != INFINITY) {
                    fprintf(fp,"%d %d %d",i,j,G->arcs[i][j]);
                    fputc('\n',fp);
                }
            }
        fclose(fp);
        printf("\n\t\t\t\t\t这条路线撤销成功~快去地图信息里查看吧!\n");
    }
    else {
        printf("\n\t\t\t\t\t这条路线不存在喔~\n");
    }
    printf("\n\t\t\t\t\t按任意键返回...");
    getch();
}
// 游客菜单
int User()
{
    AdjMatrix G;
    SchoolMap M[MAXL][MAXC];
    int choice;
    Create(&G,M);
    do{
        system("cls"); //清屏
        printf("\n\n");
	    printf("\t\t\t\t|*==============================================*|\n");
	    printf("\t\t\t\t|*                                              *|\n");
		printf("\t\t\t\t|*      ***欢迎使用西南大学荣昌校区导航***      *|\n");
	    printf("\t\t\t\t|*                                              *|\n");
	    printf("\t\t\t\t|*----------------------------------------------*|\n");
	    printf("\t\t\t\t|*                                              *|\n");
	    printf("\t\t\t\t|*               1. 导航使用说明                *|\n");
	    printf("\t\t\t\t|*               2. 校园平面简图                *|\n");
	    printf("\t\t\t\t|*               3. 查看景点信息                *|\n");
        printf("\t\t\t\t|*               4. 查询简单路线                *|\n");
	    printf("\t\t\t\t|*               5. 查询最短路线                *|\n");
	    printf("\t\t\t\t|*               6. 查询最优路线                *|\n");
	    printf("\t\t\t\t|*               7. 最佳布网方案                *|\n");
	    printf("\t\t\t\t|*               0. 退出                        *|\n");
	    printf("\t\t\t\t|*                                              *|\n");
	    printf("\t\t\t\t|*==============================================*|\n");
        printf("\n\n");
        do{
            printf("\t\t\t\t请选择您要进行的操作:");
            scanf("%d",&choice);
        }while(choice<0 || choice>7);
        getchar();
        printf("\t\t\t\t按任意键进入下一步...");
        getch();
        printf("\n");
        switch(choice)
        {
            case 1:Instruction1();break;
            case 2:Display(&G,M);break;
            case 3:Description(&G);break;
            case 4:SearchDFS(&G);break;
            case 5:SearchBFS(&G);break;
            case 6:Shortcut(&G);break;
            case 7:MiniSpanTree(&G);break;
            case 0:return 0;
        }
    }while(1);
}
// 读取admin.txt文件
struct user *Read()
{
    struct user *phead,*pnew,*pend;  //创建链表
    FILE *fp;
    fp=fopen("admin.txt","rt");  //以只读方式打开admin.txt文本文件
    phead=NULL; //头指针指向空
    if(fp!=NULL){
        while(!feof(fp)) {
            pnew=(struct user *)malloc(sizeof(struct user));  //给新结点开辟空间
            pnew->next=NULL;
            if((fscanf(fp,"\n%s %s",pnew->id,pnew->passwd))!=2)  //如果一行不是两个数据,则终止循环
               break;
            if(phead==NULL) {
                phead=pnew;
            }
            else {
                pend->next=pnew;
            }
            pend=pnew;
        }
    }
    fclose(fp); //关闭文件
    return phead; //返回头指针
}
// 管理员注册
void Register()
{
    FILE *fp;
    char id[20];
    char passwd1[20];
    char passwd2[20];
    struct user *t;
    t=Read();  //把文件数据里的头指针地址赋给指针t
    system("cls");
    printf("\n\n");
    printf("\t\t\t\t|================================================|\n");
    printf("\t\t\t\t|*                                              *|\n");
	printf("\t\t\t\t|*      ***欢迎使用西南大学荣昌校区导航***      *|\n");
    printf("\t\t\t\t|*                                              *|\n");
    printf("\t\t\t\t|*----------------------------------------------*|\n");
    printf("\t\t\t\t|*                                              *|\n");
    printf("\t\t\t\t|*                  管理员注册                  *|\n");
    printf("\t\t\t\t|*                                              *|\n");
    printf("\t\t\t\t|*==============================================*|\n");
    printf("\n\n");
    printf("\t\t\t\t\t请输入帐号:");
    scanf("%s",id);
    getchar(); //吸收回车键
    while(t!=NULL) {
        if(strcmp(id,t->id)==0) {
            printf("\n\t\t\t\t\t该帐号已存在!按任意键返回.");
            getch();
            return;
        }
        t=t->next;
    }
    fp=fopen("admin.txt","at");  //以追加方式打开文件
    printf("\n\t\t\t\t\t请输入密码:");
    scanf("%s",passwd1);
    printf("\n\t\t\t\t\t请再次输入密码:");
    scanf("%s",passwd2);
    if(strcmp(passwd1,passwd2)==0){
        fprintf(fp,"%s %s\n",id,passwd1);
        fclose(fp);
        printf("\n\t\t\t\t\t帐号注册成功!按任意键返回.");
    }
    else {
        fclose(fp);
        printf("\n\t\t\t\t\t输入密码有误,帐号注册失败!\n按任意键返回.");
    }
    getch(); //吸收任意键
}
// 管理员菜单
int Admin()
{
    AdjMatrix G;
    SchoolMap M[MAXL][MAXC];
    int choice;
    Create(&G,M);
    do{
        system("cls");
        printf("\n\n");
	    printf("\t\t\t\t|================================================|\n");
	    printf("\t\t\t\t|*                                              *|\n");
		printf("\t\t\t\t|*      ***欢迎使用西南大学荣昌校区导航***      *|\n");
	    printf("\t\t\t\t|*                                              *|\n");
	    printf("\t\t\t\t|*----------------------------------------------*|\n");
	    printf("\t\t\t\t|*                                              *|\n");
	    printf("\t\t\t\t|*               1. 导航使用说明                *|\n");
	    printf("\t\t\t\t|*               2. 校园平面简图                *|\n");
	    printf("\t\t\t\t|*               3. 查看地点信息                *|\n");
        printf("\t\t\t\t|*               4. 查询简单路线                *|\n");
	    printf("\t\t\t\t|*               5. 查询最短路线                *|\n");
	    printf("\t\t\t\t|*               6. 查询最优路线                *|\n");
	    printf("\t\t\t\t|*               7. 最佳布网方案                *|\n");
	    printf("\t\t\t\t|*               8. 添加新地点                  *|\n");
	    printf("\t\t\t\t|*               9. 添加新路线                  *|\n");
	    printf("\t\t\t\t|*               10.撤销旧路线                  *|\n");
	    printf("\t\t\t\t|*               11.注册管理员                  *|\n");
	    printf("\t\t\t\t|*               0. 退出                        *|\n");
	    printf("\t\t\t\t|*                                              *|\n");
	    printf("\t\t\t\t|*==============================================*|\n");
        printf("\n\n");
        do{
            printf("\t\t\t\t请选择您要进行的操作:");
            scanf("%d",&choice);
        }while(choice<0 || choice>11);
        getchar();
        printf("\t\t\t\t按任意键进入下一步...");
        getch();
        printf("\n");
        switch(choice)
        {
            case 1:Instruction2();break; // 管理员说明界面
            case 2:Display(&G,M);break;  // 显示地图
            case 3:Description(&G);break; // 查看景点信息
            case 4:SearchDFS(&G);break; // 查询所有简单路线
            case 5:SearchBFS(&G);break; // 查询最短路径
            case 6:Shortcut(&G);break; // 查询从起点建筑到各终点建筑的最短路线
            case 7:MiniSpanTree(&G);break; // 查询最小生成树
            case 8:AddPoint(&G,M);break; // 增加新地点
            case 9:AddEdge(&G);break; // 添加新线路
            case 10:DelEdge(&G);break; // 撤销旧线路
            case 11:Register();break; // 管理员注册
			// case 12:DelPoint(&G,M); break; // 撤销地点函数
            case 0 :exit(0);
        }
    }while(1);
}
// 管理员登录
void AdminLogin()
{
    system("cls");
    printf("\n\n");
    printf("\t\t\t\t|================================================|\n");
    printf("\t\t\t\t|*                                              *|\n");
	printf("\t\t\t\t|*      ***欢迎使用西南大学荣昌校区导航***      *|\n");
    printf("\t\t\t\t|*                                              *|\n");
    printf("\t\t\t\t|*----------------------------------------------*|\n");
    printf("\t\t\t\t|*                                              *|\n");
    printf("\t\t\t\t|*                  管理员登录                  *|\n");
    printf("\t\t\t\t|*                                              *|\n");
    printf("\t\t\t\t|*==============================================*|\n");
    printf("\n\n");
    int i,n,count=0;
    char ch;
    for(i=0;i<3;i++) {
        struct user *t;
        t=Read();
        char id[20]={0}; // 创建一个字符串数组保存用户ID
        char passwd[20]={0}; // 创建一个字符串数组保存密码
        printf("\n\t\t\t\t\t请输入帐号:");
        scanf("%s",id);
        getchar();  //吸收回车
        printf("\n\t\t\t\t\t请问您输入时是否需要隐藏密码:(Y/N):");
        scanf("%c",&ch);
        printf("\n");
        if(ch=='Y'||ch=='y') {
            printf("\t\t\t\t\t请输入密码:");
            for(n=0;;n++) {
                passwd[n]=getch();
                switch(passwd[n])
                {
                    case '\b':
                            passwd[--n]='\0';
                            n--;
                            printf("\b \b");
                            break;
                    case '\r':
                            passwd[n]='\0';
                            putchar('\n');
                            break;
                    default :
                            putchar(' ');
                            break;
                }
                if(passwd[n]=='\0')
                    break;
            }
        }
        if(ch=='N'||ch=='n') {
            getchar();
            printf("\n\t\t\t\t\t请输入密码:");
            scanf("%s",passwd);
        }
        while(t!=NULL) {
            if(strcmp(id,t->id)==0&&strcmp(passwd,t->passwd)==0) {
                printf("\n\t\t\t\t\t登录成功!按任意键进入主菜单.");
                getch();
                Admin(); // 调用个管理员菜单
                break;
            }
            t=t->next;
        }
        if(t==NULL) {
            printf("\n\t\t\t\t\t用户名或密码有误,请重新输入!\n\n");
            count++; // 错误次数加1
        }
    }
    if(count==3) {
        printf("\n\t\t\t\t\t您已连续3次用户名或密码输入有误,登录失败!系统将退出.\n");
        Sleep(5000);  // 休眠函数，短时间暂停，时间到后会自动退出
        exit(0);
    }
}
// 程序主函数
int main()
{
    int choice;
    do{
        system("cls");
        printf("\n\n");
	    printf("\t\t\t\t|*==============================================*|\n");
	    printf("\t\t\t\t|*                                              *|\n");
		printf("\t\t\t\t|*      ***欢迎使用西南大学荣昌校区导航***      *|\n");
	    printf("\t\t\t\t|*                                              *|\n");
	    printf("\t\t\t\t|*----------------------------------------------*|\n");
	    printf("\t\t\t\t|*                                              *|\n");
	    printf("\t\t\t\t|*    欢迎您，请选择用户类型：                  *|\n");
        printf("\t\t\t\t|*                                              *|\n");
	    printf("\t\t\t\t|*               1. 我是游客                    *|\n");
	    printf("\t\t\t\t|*               2. 我是管理员                  *|\n");
	    printf("\t\t\t\t|*               0. 退出                        *|\n");
	    printf("\t\t\t\t|*                                              *|\n");
	    printf("\t\t\t\t|*==============================================*|\n");
        printf("\n\n");
        do{
            printf("\t\t\t\t请选择您要进行的操作:");
            scanf("%d",&choice);
        }while(choice<0 || choice>2);
        getchar(); //吃掉回车符
        printf("\t\t\t\t按任意键进入下一步...");
        getch();
        printf("\n");
        switch(choice)
        {
            case 1:User();break;
            case 2:AdminLogin();break;
            case 0:return 0;
        }
    }while(1);
    return 0;
}