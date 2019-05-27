#include<iostream>
#include<cstdlib>
#include<ctime>
#include<algorithm>
#include <iomanip>
using namespace std;
//-----------------���ݶ���------------
#define MAX 1000000
int pageNum;   //ҳ�����
int missCount;   //ȱҳ����
double missRag;   //ȱҳ��
int memNum;    //�ڴ��������
int memcount;   
int ss;        //��������ʼҳ�ţ�
int NUM;       //�߼���ַҳ������
int speed;     //�������ƶ�����
double t = 0.5;
double r;
int N;            //�����ڴ�ߴ�
int list[MAX];    //ҳ������������
int wrList[MAX];  //ҳ���д���ʷ�ʽ
int mem[MAX];      //�ڴ��ʼ��Ϊ-1
int id;          //��ǰҳ�����λ��
int states[MAX];  //�ڴ�ҳ��״̬����
clock_t t1, t2;     //��ʼʱ���Լ�����ʱ��
int index = 0;//ָʾ��ǰ�±�

int lost = 0;//û�ҵ���ҳ����

bool  isInNodes(int n); //
void addToLink(int data, int type);
void emptyIdle();
void emptyModi();
void PBA(int n);
void usePBA();
struct LNode
{
	int data;
	int flag;//����λ
	int modify;//�޸�λ
	LNode* next;
};
struct Link
{
	int num;//��ǰ�����ϵĽ����
	LNode* next;
};
LNode* nodes;
Link idle;
Link modified;

double a1, a2, a3, a4, a5, b1, b2, b3, b4, b5;   //���ڶ���ҳ���û��㷨�Ƚ�
								
												 //������������ҳ��������������������������������������������������������
#define randomNum(a,b) rand() % (b - a + 1) + a
#define random(x) rand()%x
void listPrint();
void randOccur() {
	cout << "���������ɵ�ҳ��������еĸ���:";
	cin >> NUM;
	cout << "�����������ڴ�ĳߴ�:";
	cin >> N;
	cout << "��������ʼҳ�ţ�";
	cin >> ss;
	cout << "�����õĹ�����ҳ��������";
	cin >> pageNum;
	cout << "�����ù������ƶ����ʣ�";
	cin >> speed;
	cout << "������ʵ���ڴ�����";
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
				wrList[i] = 1;//��д��ʽ����
			}
			else {
				wrList[i] = 0;//�Զ���ʽ����
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
//�ڴ������ʼ��
void pre() {
	//------------��ʼ��------------
	for (int i = 0; i < memNum; i++) {
		mem[i] = -1;
	}
	memcount = 0;
}
void start() {
	cout << "******************ҳ���û��㷨*****************" << endl;
	cout << "1.����û��㷨             2.�Ƚ��ȳ��û��㷨  " << endl;
	cout << "3.���δʹ���û��㷨       4.ҳ�滺���㷨" << endl;
	cout << "5.�Ľ��� Clock �û��㷨    6.�Ƚϸ��㷨Ч�� " << endl;
	cout << "0.�˳� " << endl;
	cout << "***********************************************" << endl;
}
//����������
void listPrint() {
	cout << "--------------�����������---------------" << endl;
	for (int i = 0; i < NUM; i++) {
		cout << list[i] << "  ";
	}
	cout << endl;
	cout << "----------------------------------------" << endl;
}
//�����Ƿ����ڴ���
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
//����list����󱻷��ʵ�Ԫ��
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
//����û��㷨
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
					//cout << "����ȱҳ�ж�" << endl;
				}
			}
		}
		//memPrint();
	}
	t2 = clock();
	missRag = missCount * 1.0 / NUM;
	cout << "ȱҳ��:" << missRag << "\t" << "ʱ�俪��:" << double(t2 - t1) / CLOCKS_PER_SEC << endl;
	a1 = missRag;
	b1 = double(t2 - t1) / CLOCKS_PER_SEC;
}
//Ԥ�ȴ���FIFO��������
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
//�Ƚ��ȳ���̭�㷨
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
					//cout << "����ȱҳ�ж�" << endl;
				}
			}
		}
		//memPrint();
	}
	t2 = clock();
	missRag = missCount * 1.0 / NUM;
	cout << "ȱҳ��:" << missRag << "\t" << "ʱ�俪��:" << double(t2 - t1) / CLOCKS_PER_SEC << endl;
	a2 = missRag;
	b2 = double(t2 - t1) / CLOCKS_PER_SEC;
}
//�������û�б����ʵ�Ԫ��
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
//���δʹ��
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
					//cout << "����ȱҳ�ж�" << endl;
				}
			}
		}
		//memPrint();
	}
	t2 = clock();
	missRag = missCount * 1.0 / NUM;
	cout << "ȱҳ��:" << missRag << "\t" << "ʱ�俪��:" << double(t2 - t1) / CLOCKS_PER_SEC << endl;
	a3 = missRag;
	b3 = double(t2 - t1) / CLOCKS_PER_SEC;
}
//ѭ��ɨ��
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
//---����λ�������
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

//���ҸĽ�ʱ���㷨
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

//---�޸�λ�������
void rPrint() {
	cout << "��";
	for (int i = 0; i < memNum; i++) {
		if (i == memNum - 1) {
			cout << wrList[i];
			break;
		}
		cout << wrList[i] << ",";
	}
	cout << "��" << endl;
}
//�Ľ�ʱ��
void GCLOCK() {
	pre();
	id = -1;
	//��ʼ��������ҳ�ķ���λ��0��
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
					//cout << "����ȱҳ�ж�" << endl;
				}
			}
		}
		//memPrint();
		//weiPrint();
		//rPrint();
	}
	t2 = clock();
	missRag = missCount * 1.0 / NUM;
	cout << "ȱҳ��:" << missRag << "\t" << "ʱ�俪��:" << double(t2 - t1) / CLOCKS_PER_SEC << endl;
	a5 = missRag;
	b5 = double(t2 - t1) / CLOCKS_PER_SEC;
}
void compare() {
	double a = a1 * 100;
	cout << setiosflags(ios::fixed);
	cout << "����    " << "\t" << "OPT" << "\t" << "FIFO" << "\t" << "LRU" << "\t" << "PBA" << "\t" << "Gclock" << endl;
	cout << "ȱҳ��  " << "\t" << setprecision(2) << a << "%\t" << a2 * 100 << "%\t" << a3 * 100 << "%\t" << a4 * 100 << "%\t" << a5 * 100 << "%" << endl;
	cout << "ʱ�俪��" << "\t" << setprecision(4) << b1 << "\t" << b2 << "\t" << b3 << "\t" << b4 << "\t" << b5 << endl;

}
int main() {
	int select;
	cout << "---------------ҳ���û���ʼ������-------------" << endl;
	randOccur();
	while (1) {
		start();
		cout << "��������Ҫ���е�ҳ���û��㷨��";
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
		//printf("��װ���ڴ�\n");
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
				lost++;//ȱҳ

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
	printf("ҳ�滺���û��㷨��PBA��ȱҳ�ʣ�%f   ȱҳ��%d\n  ʱ�俪��%lf", 1.0*lost / NUM, lost, double(t2 - t1) / CLOCKS_PER_SEC);
}