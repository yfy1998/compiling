#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<queue>
#include"Struct.h"
using namespace std;	
static vector<string> cfg;			//上下文无关文法
static vector<char> noter;			//非终结符集合
static vector<char> ter;			//终结符集合
static vector<vector<char>> First;	//First集
static int statenumber=0;			//状态数量
static vector<DFAnode*> state;		//DFA状态集合
static vector<vector<string>> table;//转换表
bool isterminal(char a) {
	if (a >= 65 && a <= 90)return false;
	return true;
}

void caculter() {
	for (int i = 0; i < cfg.size(); i++) {
		for (int j = 3; j < cfg[i].length(); j++) {
			if (isterminal(cfg[i][j])) {
				int z;
				for (z = 0; z < ter.size(); z++) {
					if (ter[z] == cfg[i][j])break;
				}
				if (z == ter.size()) ter.push_back(cfg[i][j]);
				if (cfg[i][j] == 'i') {
					j = j + 2;
				}
			}
		}
	}
}

bool isexist(vector<char> set, char a) {
	for (int i = 0; i < set.size(); i++) {
		if (set[i] == a) return true;
	}
	return false;
}
bool istokong(char noter) {
	vector<string> right;
	for (int i = 0; i < cfg.size(); i++) {
		if (cfg[i][0] == noter) {
			int j = 3;
			string s = "";
			while (j < cfg[i].length()) {
				if (cfg[i][j] != '|') {
					s += cfg[i][j];
				}
				else {
					right.push_back(s);
					s = "";
				}
				j++;
			}
			right.push_back(s);
		}
	}
	for (int i = 0; i < right.size(); i++) {
		if (right[i] == "@") return true;
	}
	return false;
}
vector<char> first(char noter) {
	vector<string> right;
	for (int i = 0; i < cfg.size(); i++) {
		if (cfg[i][0] == noter) {
			int j = 3;
			string s = "";
			while (j < cfg[i].length()) {
				if (cfg[i][j] != '|') {
					s += cfg[i][j];
				}
				else {
					right.push_back(s);
					s = "";
				}
				j++;
			}
			right.push_back(s);
		}
	}
	vector<char> firstset;
	for (int i = 0; i < right.size(); i++) {
		int j;
		for (j = 0; j < right[i].length(); j++) {
			if (isterminal(right[i][j])) {
				if (!isexist(firstset, right[i][j]))
					firstset.push_back(right[i][j]);
				break;
			}
			else {
				for (int z = 0; z < first(right[i][j]).size(); z++) {
					if (first(right[i][j])[z] == '@') continue;
					if (!isexist(firstset, first(right[i][j])[z]))
						firstset.push_back(first(right[i][j])[z]);
				}
				if (!istokong(right[i][j]))break;
			}
		}
		if (j == right[i].length()) firstset.push_back('@');
	}
	return firstset;
}

void outputFirst() {
	for (int i = 0; i < First.size(); i++) {
		cout << "first(" << noter[i] << "):";
		for (int j = 0; j < First[i].size(); j++) {
			cout << First[i][j] << "   ";
		}
		cout << endl;
	}
}

bool issamepro(production a,production b) {
	if (a.index == b.index&&a.outlook == b.outlook&&a.s == b.s)
		return true;
	else return false;
}

bool isexistpro(vector<production> pset,production p) {
	for (int i = 0; i < pset.size(); i++) {
		if (issamepro(pset[i], p)) return true;
	}
	return false;
}

int noterfindindex(char noterminal) {
	for (int i = 0; i < noter.size(); i++) {
		if (noterminal == noter[i]) return i;
	}
	return -1;
}

vector<char> caculoutlook(vector<production> pset,char noter) {
	vector<char> re;
	for (int z = 0; z < pset.size();z++) {
		production p=pset[z];
		string s = p.s;
		int index = p.index;
		if (p.s[index] == noter) {
			int i;
			for (i = index + 1; i < s.length(); i++) {
				if (isterminal(s[i])) {
					re.push_back(s[i]);
					break;
				}
				else {
					int index = noterfindindex(s[i]);
					bool f = false;
					for (int i = 0; i < First[index].size(); i++) {
						if (First[index][i] != '@')re.push_back(First[index][i]);
						else f = true;
					}
					if (!f)break;
				}
			}
			if (i == s.length()) {
				for (int j = 0; j < p.outlook.size(); j++) {
					re.push_back(p.outlook[j]);
				}
			}
		}
	}
	return re;
}

vector<string> findnoterstr(char noter) {
	vector<string> re;
	for (int i=0; i < cfg.size(); i++) {
		if (cfg[i][0] == noter)  re.push_back(cfg[i]);
	}
	return re;
}
void extendpro(vector<production>& proset) {
	//bool* flag=new bool[noter.size()];
	//for (int i = 0; i < noter.size(); i++) {
	//	flag[i] = false;
	//}
	queue<production> q;
	for (int i = 0; i < proset.size(); i++) {
		q.push(proset[i]);
	}
	while (!q.empty()) {
		production p = q.front();
		q.pop();
		int index = p.index;
		if (isterminal(p.s[index])) continue;
		else {
			vector<char> t = caculoutlook(proset,p.s[index]);
			vector<string> sset = findnoterstr(p.s[index]);
			for (int j = 0; j < sset.size(); j++) {
				if (!isexistpro(proset, production(sset[j], 3, t))) {
					proset.push_back(production(sset[j], 3, t));
					q.push(production(sset[j], 3, t));
				}
			}
		}
	}
}

bool isexiststate(vector<production> p,int &number) {
	for (int i = 0; i < state.size(); i++) {
		if (state[i]->pset.size() == p.size()) {
			int j;
			for (j = 0; j < p.size(); j++) {
				if (!issamepro(state[i]->pset[j], p[j]))break;
			}
			if (j == p.size()) {
				number = i;
				return true;
			}
		}
	}
	return false;
}


DFAnode* getDFA() {
	DFAnode* head = new DFAnode(statenumber++);
	vector<production> h;
	h.push_back(production(cfg[0], 3, '$'));
	extendpro(h);
	for (int i = 0; i < h.size(); i++) {
		head->pset.push_back(h[i]);
	}
	state.push_back(head);
	queue<DFAnode*> q;
	q.push(head);
	while (!q.empty()) {
		DFAnode* cur=q.front();
		q.pop();
		bool* flag = new bool[cur->pset.size()];
		for (int i = 0; i < cur->pset.size(); i++) {
			flag[i] = false;
		}
		for (int i = 0; i < cur->pset.size();i++) {
			if (!flag[i]) {
				flag[i] = true;
				int index = cur->pset[i].index;
				char a = cur->pset[i].s[index];
				if (index == cur->pset[i].s.length()) continue;
				vector<production> k;
				if (a == 'i')index += 2;
				k.push_back(production(cur->pset[i].s,index+1,cur->pset[i].outlook));
				for (int j = 0; j < cur->pset.size(); j++) {
					if (!flag[j]) {
						int index1 = cur->pset[j].index;
						char b = cur->pset[j].s[index1];
						if (a == b) {
							flag[j] = true;
							if (index1 == cur->pset[j].s.length()) continue;
							if (a == 'i')index1 += 2;
							k.push_back(production(cur->pset[j].s, index1 + 1, cur->pset[j].outlook));
						}
					}
				}
				extendpro(k);
				int number;
				if (isexiststate(k, number)) {
					cur->nextnode.push_back(state[number]);
					cur->transform.push_back(a);
				}
				else {
					DFAnode* x = new DFAnode(statenumber++);
					x->pset = k;
					cur->nextnode.push_back(x);
					cur->transform.push_back(a);
					state.push_back(x);
					q.push(x);
				}
			}
		}
	}
	return head;
}

int getcolumnindex(char a) {
	if (isterminal(a)) {
		if (a == '$') return ter.size();
		for (int i = 0; i < ter.size();i++) {
			if (ter[i] == a) return i;
		}
	}
	else {
		for (int i = 0; i < noter.size(); i++) {
			if (noter[i] == a)return ter.size() + 1 + i;
		}
	}
	return -1;
}

int getreuduceindex(string s) {
	for (int i = 0; i < cfg.size(); i++) {
		if (s == cfg[i])return i;
	}
	return -1;
}

void constructtable() {
	for (int i = 0; i < statenumber; i++){
		vector<string> h(ter.size()+noter.size()+1,"");
		table.push_back(h);
	}
	for (int i = 0; i < statenumber; i++) {
		for (int j = 0; j < state[i]->nextnode.size();j++) {
			string s = "";
			if (isterminal(state[i]->transform[j])) s += "s";
			else s += "g";
			s += to_string(state[i]->nextnode[j]->index);
			int col = getcolumnindex(state[i]->transform[j]);
			table[i][col] = s;
		}
		for (int z = 0; z < state[i]->pset.size(); z++) {
			production p = state[i]->pset[z];
			if (p.s.length() == p.index) {
				string s = "";
				s += "r";
				int reduceindex = getreuduceindex(p.s);
				s += to_string(reduceindex);
				for (int l = 0; l < p.outlook.size(); l++) {
					int col = getcolumnindex(p.outlook[l]);
					table[i][col] = s;
				}
			}
		}
	}
}

void outputtable() {
	cout << "parsingtable:(规约的产生式序号是输入产生式从0开始编号)" << endl;
	for (int i = 0; i < ter.size(); i++) {
		if (ter[i] == 'i')cout << "\tint";
		else cout << "\t" << ter[i];
	}
	cout << "\t" << "$";
	for (int i = 0; i < noter.size(); i++) {
		cout << "\t" << noter[i];
	}
	cout << endl;
	for (int i = 0; i < statenumber; i++) {
		cout << i;
		for (int j = 0; j < table[i].size(); j++) {
			if (table[i][j] == "r0") cout << "\tACC";
			else cout <<"\t"<< table[i][j];
		}
		cout << endl;
	}
}
void outputcstack(stack<char> a) {
	while (!a.empty()) {
		cout << a.top();
		a.pop();
	}
}

void outputsstack(stack<int> a) {
	while (!a.empty()) {
		cout << a.top();
		a.pop();
	}
}

void check(string s) {
	cout << "状态栈\t符号栈\t字符串\tACTION"<<endl;
	stack<int> State;
	stack<char> c;
	State.push(0);
	c.push('$');
	s += "$";
	int i = 0;
	while (1) {
		outputsstack(State);
		cout << "\t";
		outputcstack(c);
		cout << "\t" << s.substr(i,s.length()-i);
		int col = getcolumnindex(s[i]);
		int stop = State.top();
		char ctop = c.top();
		string action = table[stop][col];
		if (action == "") {
			cout << "\tUNACC" << endl;
			break;
		}
		if (action == "r0") {
			cout << "\tACC"<<endl;
			break;
		}
		if (action[0] == 's') {
			int d = stoi(action.substr(1,action.length()-1));
			State.push(d);
			c.push(s[i]);
			if (s[i] == 'i') i += 2;
			i++;
			cout << "\t" << action<<endl;
			continue;
		}
		if (action[0] == 'r') {
			int d = stoi(action.substr(1, action.length() - 1));
			string pro = cfg[d];
			cout << "\t" << action << ":" << pro << endl;
			int count = 0;
			int i = 3;
			while (i < pro.length()) {
				if (pro[i] == 'i') count++;
				i++;
			}
			int m = pro.length() - 3 - 2 * count;
			while (m>0) {
				State.pop();
				c.pop();
				m--;
			}
			c.push(pro[0]);
			int col = getcolumnindex(pro[0]);
			string s1=table[State.top()][col];
			if (s1[0] == 'g') {
				int d = stoi(s1.substr(1, s1.length() - 1));
				State.push(d);
			}
		}
	}
}
int main() {
	cout << "输入规则：不要输入带有|的产生式，将其分解为多条子产生式作为输入" << endl;
	cout << "依次逐行输入上下文无关文法产生式（输入0结束）：" << endl;
	string s;
	cin >> s;
	while (s != "0") {
		cfg.push_back(s);
		cin >> s;
	}
	for (int i = 0; i < cfg.size(); i++) {
		int j;
		for (j = 0; j < noter.size(); j++) {
			if (cfg[i][0] == noter[j]) break;
		}
		if (j == noter.size()) noter.push_back(cfg[i][0]);
	}
	caculter();
	for (int i = 0; i < noter.size(); i++) {
		First.push_back(first(noter[i]));
	}
	outputFirst();
	getDFA();
	constructtable();
	outputtable();
	cout << endl;
	cout << "输入一个验证字符串:";
	string yan;
	cin >> yan;
	check(yan);
	system("pause");
	return 0;
}