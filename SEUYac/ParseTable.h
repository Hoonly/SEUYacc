#ifndef PARSETABLE_H
#define PARSETABLE_H
#include <vector>
#include"LR1.h"
using namespace std;
#define shift 1
#define reduce 2
#define gotoState 3
#define accept 4
#define error 5
//���弸�ֻ����Ĳ������ڴ˴�����Щ����ӳ�������

//vector<int> productionSize;//��¼�ķ�����ʽ�Ҷ˵�size
//vector<int> productionReduce;//��¼�ķ�����ʽ���

struct TranAction {//һ��Action�Ľṹ��������㣬�������յ�
	string sample;
	int action;
	int index;
	TranAction() {};
	TranAction(int s, int a, int i)
	{
		sample = s;
		action = a;
		index = i;
	}
};

struct State {//״̬�ṹ��������״̬���������ֱ�ʾ���͸�״̬��Ӧ��ת���б�
	int state;
	vector<TranAction> tranlist;
	State(int s) :state(s) {};
	State(int s, vector<TranAction> tlist)
	{
		state = s;
		tranlist = tlist;
	}
};

class ParseTable {
public:
	ParseTable(vector<LrState>) {

	};
	vector<State> pTable;
	//set<string> terminals;
	//set<string> unterminals;
};

#endif