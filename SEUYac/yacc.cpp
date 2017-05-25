

//vector<string> terminals;

//bool ifTerminals(string a);
//LrState addpredict(LrState tmpState, int k, int j);

//���������嶯��
#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<set>
#include<string>
#include<stack>
#include<sstream>
#include"LR1.h"
#include"LALR.h"
#include"ParseTable.h"
using namespace std;


ifstream ifile;
ofstream ofile;
int line = 0;
int checkState(char c);
#define FSTART -1
#define FEND -2
#define SECOND -3
#define ERROR -4

vector<producer>expressions;
vector<int>expUsage;
//������
struct op {
	string dest;//����������Ҳ�����
	int level;//���ȼ���levelԽ�����ȼ�Խ��
	string name;//����������
};
//����ʽ
set<string> terminals;//�ս��
set<string> unterminals;//���ս��
vector<op> oper;
vector<producer> pro;
vector<string> First(string);
int opLevel = 0;

void readFile()
{
	ifile.open("yacc.y", ios::in);
	ofile.open("yyyacc.cpp", ios::out);
	producer p;
	p.left = "S'";
	p.right.push_back("S");
	pro.push_back(p);
	if (!ifile.good())
	{
		cout << "Open file error!";
		return;
	}
	char c = ifile.get();
	int state = checkState(c);
	if (state != FSTART)
		cout << "File Error!";
	while (!ifile.eof() && state != FEND)
	{
		c = ifile.get();
		if (c == '%') {
			state = checkState(c);
			continue;
		}
		if (c == '\n')
			line++;
		ofile.put(c);
	}//���yacc.y˵�����ֵĽ���

	c = ifile.get();
	string type, meaning;
	while (true)
	{
		c = ifile.get();
		if (c == '%')
		{
			if (checkState(c) == -3)
				break;
			ifile >> type;
			if (type == "token")
			{
				c = ifile.get();
				while (c != '\n')
				{
					ifile >> meaning;
					terminals.insert(meaning);
					c = ifile.get();
				}
			}
			else if (type == "left")
			{
				c = ifile.get();
				while (c != '\n')
				{
					ifile >> meaning;
					op o;
					o.level = opLevel;
					o.dest = type;
					o.name = meaning;
					oper.push_back(o);
					c = ifile.get();
				}
				opLevel++;
			}
		}
	}//˵�����ֽ������

	c = ifile.get();
	string str;
	getline(ifile, str);
	while (true)
	{
		if (str == "%%")
			break;
		if (str == "\n" | str == "")
		{
			getline(ifile, str);
			continue;
		}
		else if (str.find(':') != -1)
		{
			int offset = str.find(':');
			string str1 = str.substr(0, offset);
			string str2 = str.substr(offset + 1, str.size() - 1);
			if (terminals.find(str1) == terminals.end() && unterminals.find(str1) == unterminals.end())
			{
				unterminals.insert(str1);
			}

			producer pr;
			pr.left = str1;
			pr.right.push_back(str2);
			while (true)
			{
				getline(ifile, str);
				if (str.find('|') != -1)
				{
					int offset = str.find('|');
					string str1 = str.substr(offset + 1, str.size() - 1);

					pr.right.push_back(str1);
				}
				else
					break;
			}
			pro.push_back(pr);
		}
		else {
			cout << "file error!" << endl;
		}
	}
	c = ifile.get();
	while (!ifile.eof())
	{
		ofile.put(c);
		c = ifile.get();
	}
}

int checkState(char c)
{
	if (c == '%')
	{
		char cc = ifile.get();
		switch (cc)
		{
		case '{':
			return -1;
		case '}':
			return -2;
		case '%':
			return -3;
		default:
			ifile.unget();
			return -4;
		}
	}
	return 0;
}
vector<string> First(string str)//��һ���ַ�����First
{
	vector<string> first;
	int value = 0;
M:
	if (terminals.find(str) != terminals.end())
	{
		first.push_back(str);
		value = 1;
	}
	else if (unterminals.find(str) != unterminals.end())
	{
		value = 1;
	R:
		for (int i = 0; i < pro.size(); i++)
		{
			if (pro[i].left == str)
			{
				for (int j = 0; j < pro[i].right.size(); j++)
				{
					string temp = pro[i].right[j];
					int offset = temp.find(" ");
					if (offset != -1)
						temp = temp.substr(0, offset);
					if (terminals.find(temp) != terminals.end())
						first.push_back(temp);
					else if (unterminals.find(temp) != unterminals.end())
					{
						str = pro[i].right[j];
						goto R;
					}
				}
			}
		}
	}
	else if (value == 0)
	{
		char c = str[0];
		stringstream stream;
		stream << c;
		str = stream.str();
		goto M;
	}
	else
	{
		cout << "first error";
	}
	return first;
}

LR1::LR1(producer pro0)
{
	LrState I0;//����һ��I0
	I0.Number = 0;//���ñ��
	LrItem tmpItem;//����һ���µ�Item(��Ϊ�����ķ�ֻ��һ������ʽ)
	tmpItem.left = pro0.left;
	for (int i = 0; i < pro0.right[0].size(); i++) {/*��Ϊpro0��Item�����ݽṹ��ͬ����Ҫ��pro0�Ĳ�����������string�ֳ���ĸ*/
		string str;
		stringstream stream;
		stream << pro0.right[0][i];
		str = stream.str();
		tmpItem.right.push_back(str);
	}
	//tmpItem.right = pro0.right;
	tmpItem.dotpos = 0;//���λ��
	tmpItem.predict.push_back("#");//Ԥ�����ʼ��
	I0.items.push_back(tmpItem);//Item����state
	I0 = adjust(I0);//�������ķ�������չ
	AllStates.push_back(I0);//�������
}
bool ItemExistance(LrState state, LrItem item) {/*�ж�state���Ƿ��Ѿ��б�item����Ԥ���������ͬ����*/
	for (int i = 0; i < state.items.size(); i++) {
		if (item <= state.items[i]) {
			return true;
		}
	}
	return false;
}
bool ifeleexist(vector<string> tmpvector, string tmpstring) {/*�ж�tmpstring�Ƿ��Ѿ���tmpvector�У��������Ԥ���*/
	for (int i = 0; i < tmpvector.size(); i++) {
		if (tmpvector[i] == tmpstring) {
			return true;
		}
	}
	return false;
}
LrState LR1::adjust(LrState tmpState)
{
	//	bool findSameItem = false;
	for (int i = 0; i < tmpState.items.size(); i++) {/*������ÿһ��Item���б�����չ*/
		if (tmpState.items[i].dotpos<tmpState.items[i].right.size() && !ifTerminals(tmpState.items[i].right[tmpState.items[i].dotpos])) {//����㲻������Һ�����Ƿ��ս�
			for (int j = 0; j < pro.size(); j++) {/*����pro��ÿһ������ʽ������е�ǰ���ս����Ӧ�Ĳ���ʽ*/
				if (pro[j].left == tmpState.items[i].right[tmpState.items[i].dotpos]) {//����Ǹ÷��ս������������ʽ
					for (int counter = 0; counter < pro[j].right.size(); counter++) {/*��pro��һ�����Ӧ�Ĳ���ʽ��ߣ���������ʽ�ұߡ�(��Ϊproducer�����ݽṹ��item��ͬ)*/
						LrItem tmpItem;
						tmpItem.dotpos = 0;
						tmpItem.left = pro[j].left;
						for (int k = 0; k < pro[j].right[counter].size(); k++) {/*pro�ұߺ�Item�ұߵ�ת��*/
							if (pro[j].right[counter] == "ID")
							{
								tmpItem.right.push_back(pro[j].right[counter]);
								break;
							}
							if (pro[j].right[counter] == "INTEGER") {
								tmpItem.right.push_back(pro[j].right[counter]);
								break;
							}
							if (pro[j].right[counter][k] != ' ') {
								string str;
								stringstream stream;
								stream << pro[j].right[counter][k];
								str = stream.str();
								tmpItem.right.push_back(str);
							}
						}
						if (tmpState.items[i].dotpos == tmpState.items[i].right.size() - 1 && !ifTerminals(tmpState.items[i].right[tmpState.items[i].dotpos])) {/*�����
																																								�ڷ��ս��֮ǰ�����Ҹ÷��ս�������һ�����ţ���ô
																																								���������ʽ��Ԥ���������չ�����Ĳ���ʽ��Ԥ�������*/
							for (int k = 0; k < tmpState.items[i].predict.size(); k++) {
								tmpItem.predict.push_back(tmpState.items[i].predict[k]);
							}
						}
						else {
							tmpItem.predict.push_back("#");
						}
						string tmpstring = "";
						for (int k = tmpState.items[i].dotpos + 1; k < tmpState.items[i].right.size(); k++) {
							tmpstring += tmpState.items[i].right[k];
						}
						if (tmpstring != "")
							for (int k = 0; k < First(tmpstring).size(); k++) {
								//tmpItem.predict.push_back(firststring[k]);

								if (!ifeleexist(tmpItem.predict, First(tmpstring)[k])) {
									tmpItem.predict.push_back(First(tmpstring)[k]);//����first��Ԥ���
								}

							}
						if (!ItemExistance(tmpState, tmpItem)) {
							tmpState.items.push_back(tmpItem);
						}
					}
				}
			}

		}
	}
	for (int j = tmpState.items.size() - 1; j >= 0; j--) {/*�ϲ�����ʽ��ͬ��Item*/
		for (int k = 0; k < j; k++) {
			if (tmpState.items[k].left == tmpState.items[j].left&&tmpState.items[k].right == tmpState.items[j].right&&tmpState.items[k].dotpos == tmpState.items[j].dotpos) {//�������
				tmpState = addpredict(tmpState, k, j);//�������Ԥ�����ͬ������ļӵ����еġ�
				tmpState.items.erase(tmpState.items.begin() + j);//ȥ�������ɵ�Item
				break;
			}
		}
	}
	for (int i = 0; i < tmpState.items.size(); i++) {/*�ѿ��Գ��ı߼��뵽outeredge*/
		if (tmpState.items[i].dotpos != tmpState.items[i].right.size()) {//�㲻��item���
			bool outedgeexist = false;
			for (int j = 0; j < tmpState.outedge.size(); j++) {
				if (tmpState.items[i].right[tmpState.items[i].dotpos] == tmpState.outedge[j]) {
					outedgeexist = true;
					continue;
				}
			}
			if (!outedgeexist) {
				tmpState.outedge.push_back(tmpState.items[i].right[tmpState.items[i].dotpos]);
			}
		}
	}
	return tmpState;
}

LrState LR1::addpredict(LrState tmpState, int k, int j) {//�ϲ���Ԥ�����k��ԭ�е�ʽ�ӵ��±�
	int a = tmpState.items[j].predict.size();
	int b = tmpState.items[k].predict.size();
	bool findpre = false;
	for (int newpre = 0; newpre <a; newpre++) {
		for (int oldpre = 0; oldpre < b; oldpre++) {
			if (tmpState.items[k].predict[oldpre] == tmpState.items[j].predict[newpre]) {/*�ҵ��˾ͽ���*/
				findpre = true;
				break;
			}
			else
				continue;
		}
		if (!findpre) {//û�ҵ��Ļ������뵽���е�ʽ�ӵ�Ԥ�����
			tmpState.items[k].predict.push_back(tmpState.items[j].predict[newpre]);
		}
		else
		{
			findpre = false;
		}
	}
	return tmpState;
}
LrState LR1::createNewState(int transferform, string edge) {
	LrState tmpState;
	tmpState = AllStates[transferform];
	tmpState.edges.clear();
	for (int i = tmpState.items.size() - 1; i >= 0; i--) {/*ȥ���������·���ķ��ս��������Ӻ���ǰ��*/
		if (tmpState.items[i].dotpos < tmpState.items[i].right.size()) {
			if (tmpState.items[i].right[tmpState.items[i].dotpos] != edge) {
				tmpState.items.erase(tmpState.items.begin() + i);
			}
			else
			{
				tmpState.items[i].dotpos++;
			}
		}
		else
		{
			tmpState.items.erase(tmpState.items.begin() + i);
			continue;
		}
	}
	tmpState.outedge.clear();
	tmpState = adjust(tmpState);
	return tmpState;
}
vector<LrState> LR1::createLR1() {
	for (int i = 0; i < AllStates.size(); i++) {//�������չ
		for (int j = 0; j < AllStates[i].outedge.size(); j++) {//������ǰ��ſ��Գ��ı�
			LrState tmpState;
			tmpState = createNewState(i, AllStates[i].outedge[j]);
			if (ifStateExistant(tmpState) == -1) {//������ɵ�״̬�����ڣ����뵽AllStates
				tmpState.Number = AllStates.size();
				AllStates[i].edges.push_back(pair<string, int>(AllStates[i].outedge[j], tmpState.Number));
				AllStates.push_back(tmpState);
			}
			else {//���״̬���ڣ���ô�����ɵ�State�ı�ָ���Ѵ��ڵ�State
				AllStates[i].edges.push_back(pair<string, int>(AllStates[i].outedge[j], ifStateExistant(tmpState)));
			}
		}
	}
	return AllStates;
}
int LR1::ifStateExistant(LrState newState) {/*�жϸ�״̬�Ƿ����*/
	for (int i = 0; i < AllStates.size(); i++) {
		if (newState == AllStates[i]) {
			return i;
			break;
		}
	}
	return -1;
}
bool LR1::ifTerminals(string a)/*�ж��Ƿ��ս��*/
{
	if (terminals.find("a") == terminals.end())
		return false;
	else
		return true;
}

int countFileLines(string fileName)//��������ļ�������
{
	ifstream ifile(fileName, ios::in);
	int n = 0;
	string temp;
	if (!ifile)
		return 0;
	else
	{
		while (getline(ifile, temp))
		{
			n++;
		}
		return n;
	}
	ifile.close();
}

int countSpace(string str)
{
	int number = 0;
	for (int i = 0; i < str.size(); i++)
		if (str[i] == ' ')
			number++;
	return number;
}

void outputProduction(producer pro)
{
	cout << pro.left <<"-->"<< pro.right[0] << endl;
}

void synana(ParseTable parseTable)//�����﷨����������﷨��������
{
	stack<int> stateStack;//״̬ջ
	stack<string>symbolStack;//����ջ
	stateStack.push(0);//״̬ջ��ѹ0
	vector<producer> allPro;
	ifstream ifile("lexfile.txt", ios::in);//��ȡ�ʷ�������������
	int lines = countFileLines("lexfile.txt");
	string *lab = new string[lines];
	string str;
	vector<string>strarr;//= new string[lines];
	for (int i = 0; i < lines; i++)//��ȡҪ�����﷨������Ƭ��
	{
		ifile >> lab[i];
		ifile >> str;
		strarr.push_back(str);
	}
	ifile.close();
	for(int i=0;i<pro.size();i++)//������ʽ����ʽת��Ϊ����Ҫ����ʽ
		for (int j = 0; j < pro[i].right.size(); j++)
		{
			producer p;
			p.left = pro[i].left;
			p.right.push_back(pro[i].right[j]);
			allPro.push_back(p);
		}
	int n = 0;
	while (1)//���﷨�������㷨
	{	
		int in = -1;	
		State state = parseTable.pTable[stateStack.top()];
		for (int i = 0; i < state.tranlist.size(); i++)
		{
			if (state.tranlist[i].sample == strarr[n])
			{
				in = i;
				if (state.tranlist[i].action == 1)
					n++;
				break;
			}
		}
		if (in != -1)
		{
			switch (state.tranlist[in].action)//���ݲ�ͬ�Ķ������в���
			{
			case 1:
				stateStack.push(state.tranlist[in].index);
				symbolStack.push(state.tranlist[in].sample);
				cout << "Shift" << state.tranlist[in].index<<endl;
				break;
			case 2:
				for (int counter = 0; counter < countSpace(allPro[state.tranlist[in].index].right[0])+1; counter++)
				{
					stateStack.pop();
					symbolStack.pop();
				}
				cout << "Reduce by  ";
				outputProduction(allPro[state.tranlist[in].index]);
				symbolStack.push(allPro[state.tranlist[in].index].left);
				state = parseTable.pTable[stateStack.top()];
				for (int i = 0; i < state.tranlist.size(); i++)
				{
					if (state.tranlist[i].sample == symbolStack.top())
					{
						stateStack.push(parseTable.pTable[stateStack.top()].tranlist[i].index);
						break;
					}
				}
				break;
			case 3:
				stateStack.push(state.tranlist[in].index);
				symbolStack.push(state.tranlist[in].sample);
				break;
			case 4:
				cerr << "Accept" << endl;
				return;
			case 5:
				cout << "Error" << endl;
				return;
			}
		}
		else
			cout << "error!" << endl;
	}
}

int main()
{
	readFile();
	LR1 newlr1(pro[0]);
	vector<LrState> LR1States =newlr1.createLR1();
	LALR newlalr(LR1States);
	vector<LrState>allState = newlalr.getAllState();
	ParseTable parseTable(allState);
	for (int i = 0; i < allState.size(); i++)//��LALR�Զ���ת��Ϊ�﷨������
	{
		State state(i);
		TranAction tran;
		for (int j = 0; j < allState[i].edges.size(); j++)
		{
			if (terminals.find(allState[i].edges[j].first) != terminals.end())
			{
				tran.action = shift;
				tran.index = allState[i].edges[j].second;
				tran.sample = allState[i].edges[j].first;
				state.tranlist.push_back(tran);
			}
			else if (unterminals.find(allState[i].edges[j].first) != unterminals.end())
			{
				tran.action = gotoState;
				tran.index = allState[i].edges[j].second;
				tran.sample = allState[i].edges[j].first;
				state.tranlist.push_back(tran);
			}
		}
		for (int m = 0; m < allState[i].items.size(); m++)
		{
			if (allState[i].items[m].dotpos == allState[i].items[m].right.size())
			{
				int number = 0;
				string str;
				for (int c = 0; c < allState[i].items[m].right.size(); c++)
				{
					str = str + allState[i].items[m].right[c];
					str = str + " ";
				}
				str = str.substr(0, str.size() - 1);
				for (int a = 0; a < pro.size(); a++)
				{
					if (allState[i].items[m].left == pro[a].left)
					{
						for (int b = 0; b < pro[a].right.size(); b++)
						{
							if (str == pro[a].right[b])
							{
								number += b;
								tran.index = number;
								if (number == 0)
								{
									for (int q = 0; q < allState[i].items[m].predict.size(); q++)
									{
										tran.action = accept;
										tran.sample = allState[i].items[m].predict[q];
										tran.index = number;
										state.tranlist.push_back(tran);
									}
								}
								else
								{						
									for (int q = 0; q < allState[i].items[m].predict.size();q++)
									{
										tran.action = reduce;
										tran.sample = allState[i].items[m].predict[q];
										tran.index = number;
										state.tranlist.push_back(tran);
									}		
								}
								
							}
						}
					}
					number += pro[a].right.size();
				}
			}
		}
		parseTable.pTable.push_back(state);
	}
	
	synana(parseTable);
	return 0;
}