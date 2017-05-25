#pragma once
#ifndef LR1_H
#define LR1_H

#include<vector>
#include<map>
#include<set>
using namespace std;

struct producer {//������Ĳ���ʽ
	string left;
	vector<string> right;

};
struct LrItem
{
	string left;
	vector<string> right;
	int dotpos;
	vector<string>predict;
	bool &operator<=(const LrItem &item) {/*��==��࣬��û��Ҫ��Ԥ���������ֻҪǰ�ߵ�Ԥ��������ں����ҵ���������*/
		bool tmp = true;
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
						if (!findpre) {
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
	bool &operator==(const LrItem &item) {/*���صȺ���������ж��Ƿ�����״̬���*/
		bool tmp = true;//����Item�Ƿ���ͬ�ı�־λ
		if (item.dotpos == this->dotpos) {//���жϵ��λ��
			if (item.left == this->left) {//�жϲ���ʽ���
				if (item.right == this->right) {//����ʽ�ұ�
					if (this->predict.size() == item.predict.size())//Ԥ�������
						for (int i = 0; i < this->predict.size(); i++) {
							bool findpre = false;//Ԥ������λ������ܶ���붼�õ������ַ�ʽ
							for (int j = 0; j < item.predict.size(); j++) {
								if (this->predict[i] == item.predict[j]) {
									findpre = true;//�����ǰ���ҵ��˺����е�Ԥ�������ô���λ��1
								}
							}
							if (!findpre) {
								tmp = false;//�������һ����־λû���ҵ�����ô˵��������Item����ͬ��
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
	int Number;//State���
	vector<LrItem> items;
	vector<pair<string, int>>edges;//����
	vector<string>outedge;//�����һλ�ļ���
	bool &operator==(const LrState &state) {
		bool ifequal = true;//����State�Ƿ���ͬ�ı��λ
		if (this->items.size() == state.items.size()) {//�ж�Item������ߴ���Ч��
			for (int i = 0; i < this->items.size(); i++) {/*�ж�items*/
				bool ifitemssequal = false;//Item��ͬ�ı��λ
				for (int j = 0; j < state.items.size(); j++) {
					if (this->items[i] == state.items[j]) {
						ifitemssequal = true;//�ҵ�Item�����λ��1
						break;
					}
				}
				if (!ifitemssequal) {//���û�ҵ�Item��������State��ͬ
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
	vector<LrState> createLR1();//����LR1
	vector<LrState> AllStates;//��������State
	LrState createNewState(int, string);//��ת����State����ţ�����������
										//vector<producer>expressions;
	LrState adjust(LrState tmpState);//�ƶ���󣬶Ե����ķ��ս��������չ
									 //int currentStateNum = 0;
	bool LR1::ifTerminals(string a);//�ж��Ƿ�Ϊ�ս��
	LrState LR1::addpredict(LrState tmpState, int k, int j);//k��ԭ�е�ʽ�ӵ��±�
	int ifStateExistant(LrState newState);//�ж�State�Ƿ���ڣ��������ڣ�����-1������ڣ������Ǹ�State�ı��
};
#endif // !LR1_H
