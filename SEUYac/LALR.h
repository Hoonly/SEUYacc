#pragma once
#ifndef LALR_H
#include "LR1.h"
class LALR {
public:
	LALR(vector<LrState>);//���캯��
	vector<LrState>AllState;//����LALR�����е�
	vector<vector<int>>StateSet;//����LALR�е�STATE��LR1��State�Ķ�Ӧ��ϵ
	vector<LrState> getAllState();//����AllStates
};

#endif // !LALR_H
