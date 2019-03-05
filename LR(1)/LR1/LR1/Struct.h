#pragma once
#include<string>
#include<vector>
using namespace std;
struct production {
	string s;
	int index;
	vector<char> outlook;
	production(string s,int i) {
		this->s = s;
		index = i;
	}
	production(string s, int i,char a) {
		this->s = s;
		index = i;
		outlook.push_back(a);
	}
	production(string s, int i, vector<char> a) {
		this->s = s;
		index = i;
		for (int i = 0; i < a.size(); i++) {
			outlook.push_back(a[i]);
		}
	}
	bool operator==(const production& a) {
		if (s ==a.s&&index == a.index&&outlook == a.outlook) return true;
		else return false;
	}
};

struct DFAnode{
	vector<production> pset;
	int index;
	vector<DFAnode*> nextnode;
	vector<char> transform;
	DFAnode(int i) {
		index = i;
	}
};