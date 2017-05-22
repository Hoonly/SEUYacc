#pragma once
#ifndef LR1_H
#define LR1_H

#include<vector>
#include<map>
#include<set>
using namespace std;

struct producer {
	string left;
	vector<string> right;

};
struct LrItem
{
	string left;
	vector<string> right;
	int dotpos;
	vector<string>predict;
	bool &operator<=(const LrItem &item) {
		bool tmp=true;
		if (item.dotpos == this->dotpos) {
			if (item.left == this->left) {
				if (item.right == this->right) {
					for (int i = 0; i < this->predict.size(); i++) {
						bool findpre = false;
						for (int j = 0; j < item.predict.size(); j++) {
							if (this->predict[i] == item.predict[j]) {
								findpre = true;
							}
						}
						if (!findpre){
							tmp = false;
						}
					}
				}
				else
					tmp = false;
			}
			else
				tmp = false;
		}
		else
			tmp = false;
		return tmp;
	}
	bool &operator==(const LrItem &item) {
		bool tmp = true;
		if (item.dotpos == this->dotpos) {
			if (item.left == this->left) {
				if (item.right == this->right) {
					if (this->predict.size() == item.predict.size())
						for (int i = 0; i < this->predict.size(); i++) {
							bool findpre = false;
							for (int j = 0; j < item.predict.size(); j++) {
								if (this->predict[i] == item.predict[j]) {
									findpre = true;
								}
							}
							if (!findpre) {
								tmp = false;
							}
						}
					else
						tmp = false;
				}
				else
					tmp = false;
			}
			else
				tmp = false;
		}
		else
			tmp = false;
		return tmp;
	}

};

struct LrState
{
	int Number;
	vector<LrItem> items;
	vector<pair<string, int>>edges;
	vector<string>outedge;
	bool &operator==(const LrState &state) {
		bool ifequal = true;
		/*if (this->edges.size() == state.edges.size())
		{
			for (int i = 0; i < this->edges.size(); i++) {/*�жϳ���
				bool ifedgesequal = false;
				for (int j = 0; j < state.edges.size(); j++) {
					if (this->edges[i] == state.edges[j]) {
						ifedgesequal = true;
						break;
					}
				}
				if (!ifedgesequal) {
					ifequal = false;
					break;
				}
			}
		}
		else
		{
			ifequal = false;
		}*/
		if (this->items.size() == state.items.size()) {
			for (int i = 0; i < this->items.size(); i++) {/*�ж�items*/
				bool ifitemssequal = false;
				for (int j = 0; j < state.items.size(); j++) {
					if (this->items[i] == state.items[j]) {
						ifitemssequal = true;
						break;
					}
				}
				if (!ifitemssequal) {
					ifequal = false;
					break;
				}
			}
		}
		else
		{
			ifequal = false;
		}
		return ifequal;
	}
};
class LR1
{
public:
	LR1(producer);
	void createLR1();
	vector<LrState> AllStates;
	LrState createNewState(int,string);//��ת����State����ţ�����������
	vector<producer>expressions;
	LrState adjust(LrState tmpState);
	int currentStateNum = 0;
	bool LR1::ifTerminals(string a);
	LrState LR1::addpredict(LrState tmpState, int k, int j);//k��ԭ�е�ʽ�ӵ��±�
	int ifStateExistant(LrState newState);//�ж�State�Ƿ���ڣ��������ڣ�����-1������ڣ������Ǹ�State�ı��
};
#endif // !LR1_H
