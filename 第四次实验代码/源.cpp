#include<iostream>
#include<cstdlib>
#include<ctime>
#include<algorithm>
#include <iomanip>
using namespace std;
//-----------------数据定义------------
#define MAX 1000000
int pageNum;   //页面个数
int missCount;   //缺页次数
double missRag;   //缺页率
int memNum;    //内存物理块数
int memcount;   
int ss;        //工作集起始页号；
int NUM;       //逻辑地址页面总数
int speed;     //工作集移动速率
double t = 0.5;
double r;
int N;            //虚拟内存尺寸
int list[MAX];    //页面访问随机序列
int wrList[MAX];  //页面读写访问方式
int mem[MAX];      //内存初始化为-1
int id;          //当前页面访问位置
int states[MAX];  //内存页面状态数组
clock_t t1, t2;     //开始时间以及结束时间
int index = 0;//指示当前下标

int lost = 0;//没找到的页面数

bool  isInNodes(int n); //
void addToLink(int data, int type);
void emptyIdle();
void emptyModi();
void PBA(int n);
void usePBA();
struct LNode
{
	int data;
	int flag;//访问位
	int modify;//修改位
	LNode* next;
};
struct Link
{
	int num;//当前链表上的结点数
	LNode* next;
};
LNode* nodes;
Link idle;
Link modified;

double a1, a2, a3, a4, a5, b1, b2, b3, b4, b5;   //用于多种页面置换算法比较
								
												 //——————页面访问序列随机发生————————————————————
#define randomNum(a,b) rand() % (b - a + 1) + a
#define random(x) rand()%x
void listPrint();
void randOccur() {
	cout << "请设置生成的页面访问序列的个数:";
	cin >> NUM;
	cout << "请设置虚拟内存的尺寸:";
	cin >> N;
	cout << "请设置起始页号：";
	cin >> ss;
	cout << "请设置的工作集页面总数：";
	cin >> pageNum;
	cout << "请设置工作集移动速率：";
	cin >> speed;
	cout << "请设置实际内存数：";
	cin >> memNum;
	cout << "---------------------------------" << endl;
	srand((int)time(0));
	int j = 0;
	for (int i = 0; i<NUM; i++) {
		if (j < speed) {
			int x = ss + pageNum, y = N - 1;
			list[i] = randomNum(ss, min(x, y));
			double sn = random(10)*0.1;
			if (sn > 0.7) {
				wrList[i] = 1;//以写方式访问
			}
			else {
				wrList[i] = 0;//以读方式访问
			}
			j++;
		}
		else {
			r = random(10)*0.1;
			j = 0;
			if (r < t) {
				ss = random(N);
			}
			else {
				ss = (ss + 1) % N;
			}
		}
	}
}
//内存数组初始化
void pre() {
	//------------初始化------------
	for (int i = 0; i < memNum; i++) {
		mem[i] = -1;
	}
	memcount = 0;
}
void start() {
	cout << "******************页面置换算法*****************" << endl;
	cout << "1.最佳置换算法             2.先进先出置换算法  " << endl;
	cout << "3.最久未使用置换算法       4.页面缓冲算法" << endl;
	cout << "5.改进型 Clock 置换算法    6.比较各算法效率 " << endl;
	cout << "0.退出 " << endl;
	cout << "***********************************************" << endl;
}
//输出随机序列
void listPrint() {
	cout << "--------------随机序列如下---------------" << endl;
	for (int i = 0; i < NUM; i++) {
		cout << list[i] << "  ";
	}
	cout << endl;
	cout << "----------------------------------------" << endl;
}
//查找是否在内存中
int judge(int n) {
	int a;
	for (int i = 0; i < memNum; i++) {
		if (mem[i] == list[n]) {
			a = 1;
			if (n >= memNum)
				id = i;
			states[i] = 1;
			break;
		}
		if (i == memNum - 1 && mem[i] != list[n])
			a = 0;
	}
	return a;
}
//查找list中最后被访问的元素
void search(int wei) {
	int cur = 0;
	int last[MAX];
	int min = 0;
	for (int i = 0; i < memNum; i++) {
		last[i] = MAX;
	}
	for (int j = 0; j < memNum; j++) {
		for (int i = wei + 1; i < NUM; i++) {
			if (mem[j] == list[i]) {
				last[j] = i;
				break;
			}
		}
	}
	for (int i = 0; i < memNum; i++) {
		if (last[i] > min) {
			cur = i;
			min = last[i];
		}
	}
	mem[cur] = list[wei];
}
void memPrint() {
	cout << "<";
	for (int i = 0; i < memNum; i++) {
		if (i == memNum - 1) {
			cout << mem[i];
			break;
		}
		cout << mem[i] << ",";
	}
	cout << ">" << endl;
}
//最佳置换算法
void OPT() {
	pre();
	t1 = clock();
	missCount = 0;
	int flag = -1;
	for (int i = 0; i <NUM; i++) {
		if (memcount < 3) {
			mem[memcount] = list[i];
			memcount++;
		}
		else {
			flag = judge(i);
			if (flag == 0) {
				if (memcount < memNum) {
					mem[memcount] = list[i];
					memcount++;
				}
				else {
					search(i);
					missCount++;
					//cout << "发生缺页中断" << endl;
				}
			}
		}
		//memPrint();
	}
	t2 = clock();
	missRag = missCount * 1.0 / NUM;
	cout << "缺页率:" << missRag << "\t" << "时间开销:" << double(t2 - t1) / CLOCKS_PER_SEC << endl;
	a1 = missRag;
	b1 = double(t2 - t1) / CLOCKS_PER_SEC;
}
//预先处理FIFO计数数组
int first[MAX];
void prd() {
	for (int i = 0; i < memNum; i++) {
		first[i] = 0;
	}
}
void add(int n) {
	for (int i = 0; i <= n; i++) {
		first[i]++;
	}
}
//先进先出淘汰算法
void FIFO() {
	pre();
	prd();
	t1 = clock();
	missCount = 0;
	int flag = -1;
	for (int i = 0; i < NUM; i++) {
		if (memcount < 3) {
			mem[memcount] = list[i];
			add(memcount);
			memcount++;
		}
		else {
			flag = judge(i);
			if (flag == 0) {
				if (memcount < memNum) {
					mem[memcount] = list[i];
					add(memcount);
					memcount++;
				}
				else {
					int min = 0, cur = 0;
					for (int k = 0; k < memNum; k++) {
						if (first[k] > min) {
							cur = k;
							min = first[k];
						}
					}
					first[cur] = 0;
					add(memNum);
					mem[cur] = list[i];
					missCount++;
					//cout << "发生缺页中断" << endl;
				}
			}
		}
		//memPrint();
	}
	t2 = clock();
	missRag = missCount * 1.0 / NUM;
	cout << "缺页率:" << missRag << "\t" << "时间开销:" << double(t2 - t1) / CLOCKS_PER_SEC << endl;
	a2 = missRag;
	b2 = double(t2 - t1) / CLOCKS_PER_SEC;
}
//查找最久没有被访问的元素
void searchL(int wei) {
	int cur = 0;
	int endf[MAX];
	int min = MAX;
	for (int i = 0; i < memNum; i++) {
		endf[i] = -1;
	}
	for (int j = 0; j < memNum; j++) {
		for (int i = wei + 1; i > -1; i--) {
			if (mem[j] == list[i]) {
				endf[j] = i;
				break;
			}
		}
	}
	for (int i = 0; i < memNum; i++) {
		if (endf[i] < min) {
			cur = i;
			min = endf[i];
		}
	}
	mem[cur] = list[wei];
}
//最久未使用
void LRU() {
	pre();
	t1 = clock();
	missCount = 0;
	int flag = -1;
	for (int i = 0; i < NUM; i++) {
		if (memcount < 3) {
			mem[memcount] = list[i];
			memcount++;
		}
		else {
			flag = judge(i);
			if (flag == 0) {
				if (memcount < memNum) {
					mem[memcount] = list[i];
					memcount++;
				}
				else {
					searchL(i);
					missCount++;
					//cout << "发生缺页中断" << endl;
				}
			}
		}
		//memPrint();
	}
	t2 = clock();
	missRag = missCount * 1.0 / NUM;
	cout << "缺页率:" << missRag << "\t" << "时间开销:" << double(t2 - t1) / CLOCKS_PER_SEC << endl;
	a3 = missRag;
	b3 = double(t2 - t1) / CLOCKS_PER_SEC;
}
//循环扫描
void Cloop(int n, int wei) {
	int temp;
	for (int i = n + 1;; i++) {
		temp = i % memNum;
		if (states[temp] == 0) {
			mem[temp] = list[wei];
			states[temp] = 1;
			id = temp;
			break;
		}
		else {
			states[temp] = 0;
		}
	}
}
//---访问位输出函数
void weiPrint() {
	cout << "(";
	for (int i = 0; i < memNum; i++) {
		if (i == memNum - 1) {
			cout << states[i];
			break;
		}
		cout << states[i] << ",";
	}
	cout << ")" << endl;
}

//查找改进时钟算法
void Gloop(int n, int wei) {
	int temp;
	for (int i = n + 1;; i++) {
		temp = i % memNum;
		if (states[temp] == 0 && wrList[temp] == 0) {
			mem[temp] = list[wei];
			states[temp] = 1;
			id = temp;
			break;
		}
		if (i>n + memNum - 1) {
			if (states[temp] == 0 && wrList[temp] == 1) {
				mem[temp] = list[wei];
				states[temp] = 1;
				id = temp;
				break;
			}
			states[temp] = 0;
		}
		if (i == n + 2 * memNum - 1)
			i = n;
	}
}

//---修改位输出函数
void rPrint() {
	cout << "【";
	for (int i = 0; i < memNum; i++) {
		if (i == memNum - 1) {
			cout << wrList[i];
			break;
		}
		cout << wrList[i] << ",";
	}
	cout << "】" << endl;
}
//改进时钟
void GCLOCK() {
	pre();
	id = -1;
	//初始化将所有页的访问位置0；
	for (int i = 0; i < memNum; i++) {
		states[i] = 0;
	}
	t1 = clock();
	missCount = 0;
	int flag = -1;
	for (int i = 0; i < NUM; i++) {
		if (memcount < 3) {
			mem[memcount] = list[i];
			states[memcount] = 1;
			memcount++;
		}
		else {
			flag = judge(i);
			if (flag == 0) {
				if (memcount < memNum) {
					mem[memcount] = list[i];
					states[memcount] = 1;
					memcount++;
				}
				else {
					Gloop(id, i);
					missCount++;
					//cout << "发生缺页中断" << endl;
				}
			}
		}
		//memPrint();
		//weiPrint();
		//rPrint();
	}
	t2 = clock();
	missRag = missCount * 1.0 / NUM;
	cout << "缺页率:" << missRag << "\t" << "时间开销:" << double(t2 - t1) / CLOCKS_PER_SEC << endl;
	a5 = missRag;
	b5 = double(t2 - t1) / CLOCKS_PER_SEC;
}
void compare() {
	double a = a1 * 100;
	cout << setiosflags(ios::fixed);
	cout << "名称    " << "\t" << "OPT" << "\t" << "FIFO" << "\t" << "LRU" << "\t" << "PBA" << "\t" << "Gclock" << endl;
	cout << "缺页率  " << "\t" << setprecision(2) << a << "%\t" << a2 * 100 << "%\t" << a3 * 100 << "%\t" << a4 * 100 << "%\t" << a5 * 100 << "%" << endl;
	cout << "时间开销" << "\t" << setprecision(4) << b1 << "\t" << b2 << "\t" << b3 << "\t" << b4 << "\t" << b5 << endl;

}
int main() {
	int select;
	cout << "---------------页面置换初始化设置-------------" << endl;
	randOccur();
	while (1) {
		start();
		cout << "请输入你要进行的页面置换算法：";
		cin >> select;
		if (select == 1) {
			//listPrint();
			OPT();
			continue;
		}
		if (select == 2) {
			//listPrint();
			FIFO();
			continue;
		}
		if (select == 3) {
			//listPrint();
			LRU();
			continue;
		}
		if (select == 4) {
			//listPrint();
			usePBA();

			continue;
		}
		if (select == 5) {
			//listPrint();
			GCLOCK();
			continue;
		}
		if (select == 6) {
			OPT(); FIFO(); LRU();  GCLOCK();
			compare();
			
			continue;
		}
		if (select == 0) {
			break;
		}
	}
	
	system("pause");
	return 0;
}


bool isInNodes(int n)
{
	int i;

	for (i = 0; i < 3; i++)
	{
		if (nodes[i].data == list[n])
		{
			return true;
		}
	}

	return false;
}

LNode* isinLinks(int n)
{
	LNode*p, *q;
	p = idle.next;
	q = NULL;

	while (p)
	{
		if (p->data == list[n])
		{
			if (q != NULL)
			{
				q->next = p->next;
				p->next = NULL;
				idle.num--;
				break;
			}

			else
			{
				idle.next = NULL;
			}
		}

		q = p;
		p = p->next;
	}

	if (p == NULL)
	{
		p = modified.next;

		while (p != NULL)
		{
			if (p->data == list[n])
			{
				if (p == modified.next)
				{
					modified.next = p->next;
				}

				else
				{
					q->next = p->next;
					p->next = NULL;
					modified.num--;
				}

				if (modified.num == 0)
				{
					modified.next = NULL;
				}

				break;
			}

			q = p;
			p = p->next;
		}
	}

	return p;
}


void PBA(int n)
{
	int size = 3;
	if (isInNodes(n))
	{
		//printf("已装入内存\n");
	}

	else
		if (index == size)
		{
			LNode *p;

			if ((p = isinLinks(n)) != NULL)
			{
				nodes = (LNode*)realloc(nodes, (size + 1) * sizeof(LNode));
				nodes[size].data = p->data;
				nodes[size].flag = p->flag;
				nodes[size].modify = p->modify;
				nodes[size].next = p->next;
				free(p);
				size++;
				index++;
			}

			else
			{
				lost++;//缺页

				if (nodes[n % 3].modify == 1)
				{
					addToLink(nodes[n % 3].data, 1);
				}

				else
				{
					addToLink(nodes[n % 3].data, 0);
				}

				nodes[n % 3].data = list[n];
				nodes[n % 3].flag = 1;
				nodes[n % 3].next = NULL;

				if (rand() % 10 < 4)
				{
					nodes[n % 3].modify = 0;
				}

				else
				{
					nodes[n % 3].modify = 1;
				}
			}
		}

		else
		{
			nodes[index].data = list[n];
			nodes[index].flag = 1;
			nodes[index].next = NULL;

			if (rand() % 10 < 4)
			{
				nodes[index].modify = 1;
			}

			else
			{
				nodes[index].modify = 0;
			}

			index++;
		}
}

void addToLink(int data, int type)
{
	LNode* p;
	LNode* q;
	q = (LNode*)malloc(sizeof(LNode));
	q->data = data;
	q->flag = 1;

	if (type == 1)
	{
		q->modify = 1;
		p = modified.next;
	}

	else
	{
		q->modify = 0;
		p = idle.next;
	}

	q->next = NULL;

	if (p == NULL)
	{
		if (type == 0)
		{
			idle.next = q;
		}

		else
		{
			modified.next = q;
		}
	}

	else
	{
		while (p)
		{
			if (p->next == NULL)
			{
				p->next = q;
				break;
			}

			else
			{
				p = p->next;
			}
		}
	}

	if (type == 0)
	{
		idle.num += 1;

		if (idle.num == 10)
		{
			emptyIdle();
		}
	}

	else
	{
		modified.num += 1;

		if (modified.num == 10)
		{
			emptyModi();
		}
	}
}
void emptyIdle()
{
	LNode* p;
	p = idle.next;

	while (p)
	{
		idle.next = p->next;
		free(p);
		p = idle.next;
	}

	idle.num = 0;
}
void emptyModi()
{
	LNode* p;
	p = modified.next;

	while (p)
	{
		modified.next = p->next;
		free(p);
		p = modified.next;
	}

	modified.num = 0;
}

void usePBA() {
	t1 = clock();
	int size = 3;
	int i = 0, j = 0;
	idle.num = 0;
	idle.next = NULL;
	modified.num = 0;
	modified.next = NULL;
	nodes = (LNode*)malloc(size * sizeof(LNode));

	for (i = 0; i < size; i++)
	{
		nodes[i].data = -1;
		nodes[i].flag = 0;
		nodes[i].modify = 0;
		nodes[i].next = NULL;
	}

	for (i = 0; i < NUM; i++)
	{
		PBA(i);

		/*for (j = 0; j < size; j++)
		{
			printf("%d ", nodes[j].data);
		}

		printf("\n");*/
	}
	t2 = clock();
	printf("页面缓冲置换算法（PBA）缺页率：%f   缺页数%d\n  时间开销%lf", 1.0*lost / NUM, lost, double(t2 - t1) / CLOCKS_PER_SEC);
}