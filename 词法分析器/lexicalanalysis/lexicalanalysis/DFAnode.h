#pragma once
#include<vector>
#include<string>
#include<stack>
#include<iostream>
#include"NFAnode.h"
#include<queue>
using namespace std;
static int state = 0;
static int mindfastate = 0;
struct DFAnode {
	int number;
	char transform;
	vector<DFAnode*> nextnode;
	vector<NFAnode*> NFAset;
	bool isend;
	DFAnode(int a, char t,vector<NFAnode*> n,bool f) {
		number = a;
		transform = t;
		NFAset = n;
		isend = f;
	}
	void addnextnode(DFAnode*& next) {
		if (next == NULL) return;
		nextnode.push_back(next);
	}
};

struct newDFAnode {
	int number;
	vector<char> transform;
	vector<newDFAnode*> nextnode;
	vector<NFAnode*> NFAset;
	bool isend;
	newDFAnode(int a, vector<NFAnode*> n, bool f) {
		number = a;
		NFAset = n;
		isend = f;
	}
	void addnextnode(newDFAnode*& next,char tra) {
		if (next == NULL) return;
		nextnode.push_back(next);
		transform.push_back(tra);
	}
};

void NFAequalstatereverse(NFAnode* n, vector<NFAnode*>& re);
vector<NFAnode*> NFAequalstate(NFAnode* n) {
	vector<NFAnode*> re;
	re.push_back(n);
	NFAequalstatereverse(n, re);
	return re;
}

void NFAequalstatereverse(NFAnode* n,vector<NFAnode*>& re) {
	for (int z = 0; z < n->nextnode.size(); z++) {
		if (n->nextnode[z]->transform == 'E') {
			re.push_back(n->nextnode[z]);
			NFAequalstatereverse(n->nextnode[z], re);
		}
	}
}

vector<char> transformset(string s) {
	vector<char> re;
	for (int z=0; z < s.length(); z++) {
		if (s[z] != '('&&s[z] != ')'&&s[z] != '|'&&s[z] != '*') {
			bool flag = false;
			for (int j = 0; j < re.size(); j++) {
				if (s[z] == re[j]) flag = true;
			}
			if (!flag) {
				re.push_back(s[z]);
			}
		}
	}
	return re;
}

vector<NFAnode*> transformstate(DFAnode* n,char t) {
	vector<NFAnode*> re;
	for (int i = 0; i < n->NFAset.size(); i++) {
		for (int j = 0; j < n->NFAset[i]->nextnode.size(); j++) {
			if (n->NFAset[i]->nextnode[j]->transform == t) {
				//re.push_back(n->NFAset[i]->nextnode[j]);
				vector<NFAnode*> equalstate = NFAequalstate(n->NFAset[i]->nextnode[j]);
				for (int z = 0; z < equalstate.size(); z++) {
					re.push_back(equalstate[z]);
				}
			}
		}
	}
	return re;
}

bool issame(DFAnode* cur,vector<NFAnode*> n) {
	if (cur->NFAset.size() != n.size()) return false;
	for (int i = 0; i < cur->NFAset.size(); i++) {
		int j;
		for (j = 0; j < n.size(); j++) {
			if (cur->NFAset[i] == n[j]) {
				break;
			}
		}
		if (n.size() == j) return false;
	}
	return true;
}

bool samestate(DFAnode* cur, vector<NFAnode*> n, DFAnode*& out,bool*& flag) {
	if (!flag[cur->number]) {
		if (issame(cur, n)) {
			out = cur;
			return true;
		}
		flag[cur->number] = true;
	}
	else return false;
	for (int i = 0; i < cur->nextnode.size(); i++) {
		if (samestate(cur->nextnode[i], n,out,flag)) {
			return true;
		}
	}
	return false;
}

bool isfinalstate(vector<NFAnode*> n, NFAnode* e) {
	for (int i = 0; i < n.size(); i++) {
		if (n[i] == e)return true;
	}
	return false;
}

DFAnode* NFAtoDFA(NFA n,string s) {
	DFAnode* h;
	if (isfinalstate(NFAequalstate(n.head), n.end)) {
		 h = new DFAnode(state++, 'E', NFAequalstate(n.head),true);
	}
	else  h = new DFAnode(state++, 'E', NFAequalstate(n.head), false);
	vector<char> transformsett = transformset(s);
	DFAnode* currentnode = h;
	queue<DFAnode*> q;
	q.push(h);
	while (!q.empty()) {
		currentnode = q.front();
		q.pop();
		for (int i = 0; i < transformsett.size(); i++) {
			DFAnode* next;
			bool* flag = new bool[state];
			for (int j = 0; j < state; j++) {
				flag[j] = false;
			}
			vector<NFAnode*> t=transformstate(currentnode, transformsett[i]);
;			if (!samestate(h,t,next,flag)) {
				if(isfinalstate(t,n.end)){
				next = new DFAnode(state++, transformsett[i], t,true);
				}
				else next = new DFAnode(state++, transformsett[i], t, false);
				currentnode->addnextnode(next);
				q.push(next);
			}
			else {
				currentnode->addnextnode(next);
			}
		}
	}
	return h;
}

bool issameNFAset(vector<NFAnode*> n1, vector<NFAnode*> n2) {
	if (n1.size() != n2.size()) return false;
	for (int i = 0; i < n1.size(); i++) {
		int j;
		for (j = 0; j < n2.size(); j++) {
			if (n1[i] == n2[j]) {
				break;
			}
		}
		if (n2.size() == j) return false;
	}
	return true;
}

void findmin(vector<vector<DFAnode*>>& a);

void minimizetraversingDFA(DFAnode* cur, vector<DFAnode*>& notter, vector<DFAnode*>& ister, bool*& flag);
vector<vector<DFAnode*>> DFAminimize(DFAnode* head,string s) {
	vector<DFAnode*> notterminal;
	vector<DFAnode*> isterminal;
	vector<char> transset = transformset(s);
	bool* flag = new bool[state];
	for (int j = 0; j < state; j++) {
		flag[j] = false;
	}
	minimizetraversingDFA(head,notterminal, isterminal, flag);
	bool* f1 = new bool[notterminal.size()];
	for (int j = 0; j < notterminal.size(); j++) {
		f1[j] = false;
	}
	vector<vector<DFAnode*>> result;
	for (int i = 0; i < notterminal.size(); i++) {
		if (f1[i])continue;
		vector<DFAnode*> re;
		re.push_back(notterminal[i]);
		f1[i] = true;
		for (int j = i + 1; j < notterminal.size(); j++) {
			int z;
			for (z = 0; z < transset.size(); z++) {
				if (!issameNFAset(transformstate(notterminal[i], transset[z]), transformstate(notterminal[j], transset[z])))
					break;
			}
			if (z == transset.size()) {
				re.push_back(notterminal[j]);
				f1[j] = true;
			}
		}
		result.push_back(re);
		re.clear();
	}
	bool* f2 = new bool[isterminal.size()];
	for (int j = 0; j < isterminal.size(); j++) {
		f2[j] = false;
	}
	for (int i = 0; i < isterminal.size(); i++) {
		if (f2[i])continue;
		vector<DFAnode*> re;
		re.push_back(isterminal[i]);
		f2[i] = true;
		for (int j = i + 1; j < isterminal.size(); j++) {
			int z;
			for (z = 0; z < transset.size(); z++) {
				if (!issameNFAset(transformstate(isterminal[i], transset[z]), transformstate(isterminal[j], transset[z])))
					break;
			}
			if (z == transset.size()) {
				re.push_back(isterminal[j]);
				f2[j] = true;
			}
		}
		result.push_back(re);
		re.clear();
	}
	findmin(result);
	return result;
}

void minimizetraversingDFA(DFAnode* cur,vector<DFAnode*>& notter,vector<DFAnode*>& ister,bool*& flag) {
	if (flag[cur->number])return;
	if (cur->isend) ister.push_back(cur);
	else notter.push_back(cur);
	flag[cur->number] = true;
	for (int i = 0; i < cur->nextnode.size(); i++) {
		minimizetraversingDFA(cur->nextnode[i], notter, ister, flag);
	}
}

void findmin(vector<vector<DFAnode*>>& a) {
	vector<DFAnode*> cur;
	for (int i = 0; i < a.size(); i++) {
		cur = a[i];
		int minstate = INT_MAX;
		for (int j = 0; j < cur.size(); j++) {
			if (cur[j]->number < minstate) {
				minstate = cur[j]->number;
			}
		}
		for (int j = 0; j < cur.size(); j++) {
			if (cur[j]->number == minstate) {
				if (j == 0) break;
				else {
					DFAnode* t = cur[0];
					cur[0] = cur[j];
					cur[j] = t;
					break;
				}
			}
		}
	}
}
//vector<DFAnode*> nextDFAnode(DFAnode* head,DFAnode* aim) {
//	vector<DFAnode*> re;
//}
//
//void nextDFAnodereverse(vector<DFAnode*>& re,DFAnode* cur,DFAnode* aim) {
//	if (cur == aim) return;
//	for (int i = 0; i < cur->nextnode.size(); i++) {
//		if (cur->nextnode[i] == aim) {
//			re.push_back(cur->nextnode[i]);
//
//		}
//	}
//}

bool ismerge(vector<vector<DFAnode*>> mergeitem,vector<NFAnode*> cur,vector<NFAnode*>& out) {
	for (int i = 0; i < mergeitem.size(); i++) {
		for (int j = 0; j < mergeitem[i].size(); j++) {
			if (issame(mergeitem[i][j], cur)) {
				if (j == 0) {
					out = cur;
					return false;
				}
				else {
					out = mergeitem[i][0]->NFAset;
					return true;
				}
			}
		}
	 }
	out = cur;
	return false;
}

vector<NFAnode*> transformstate1(newDFAnode* n, char t) {
	vector<NFAnode*> re;
	for (int i = 0; i < n->NFAset.size(); i++) {
		for (int j = 0; j < n->NFAset[i]->nextnode.size(); j++) {
			if (n->NFAset[i]->nextnode[j]->transform == t) {
				//re.push_back(n->NFAset[i]->nextnode[j]);
				vector<NFAnode*> equalstate = NFAequalstate(n->NFAset[i]->nextnode[j]);
				for (int z = 0; z < equalstate.size(); z++) {
					re.push_back(equalstate[z]);
				}
			}
		}
	}
	return re;
}

bool issame1(newDFAnode* cur, vector<NFAnode*> n) {
	if (cur->NFAset.size() != n.size()) return false;
	for (int i = 0; i < cur->NFAset.size(); i++) {
		int j;
		for (j = 0; j < n.size(); j++) {
			if (cur->NFAset[i] == n[j]) {
				break;
			}
		}
		if (n.size() == j) return false;
	}
	return true;
}

bool samestate1(newDFAnode* cur, vector<NFAnode*> n, newDFAnode*& out, bool*& flag) {
	if (!flag[cur->number]) {
		if (issame1(cur, n)) {
			out = cur;
			return true;
		}
		flag[cur->number] = true;
	}
	else return false;
	for (int i = 0; i < cur->nextnode.size(); i++) {
		if (samestate1(cur->nextnode[i], n, out, flag)) {
			return true;
		}
	}
	return false;
}
newDFAnode* mergeDFA(NFA n, string s, vector<vector<DFAnode*>> mergeitem) {
	newDFAnode* h;
	if (isfinalstate(NFAequalstate(n.head), n.end)) {
		h = new newDFAnode(mindfastate++,  NFAequalstate(n.head), true);
	}
	else  h = new newDFAnode(mindfastate++,  NFAequalstate(n.head), false);
	vector<char> transformsett = transformset(s);
	newDFAnode* currentnode = h;
	queue<newDFAnode*> q;
	q.push(h);
	while (!q.empty()) {
		currentnode = q.front();
		q.pop();
		for (int i = 0; i < transformsett.size(); i++) {
			newDFAnode* next;
			bool* flag = new bool[mindfastate];
			for (int j = 0; j < mindfastate; j++) {
				flag[j] = false;
			}
			vector<NFAnode*> t = transformstate1(currentnode, transformsett[i]);
			vector<NFAnode*> ter;
			ismerge(mergeitem, t, ter);
			;if (!samestate1(h, ter, next, flag)) {
				if (isfinalstate(t, n.end)) {
					next = new newDFAnode(mindfastate++,  ter, true);
				}
				else next = new newDFAnode(mindfastate++,  ter, false);
				currentnode->addnextnode(next, transformsett[i]);
				q.push(next);
			}
			else {
				currentnode->addnextnode(next, transformsett[i]);
			}
		}
	}
	return h;
}
void outputDFAnode(DFAnode* head, int*& time);
 void outputDFA(DFAnode* h) {
	if (h == NULL) return;
	int* outtime = new int[state];
	for (int z = 0; z < state; z++) {
		outtime[z] = 0;
	}
	outputDFAnode(h, outtime);
}
 void outputDFAnode(DFAnode* head, int*& time) {
	cout << head->transform << "   " << head->number << endl;
	time[head->number]++;
	if (time[head->number] >= 2)return;
	for (int i = 0; i < head->nextnode.size(); i++) {
		cout << head->number << "   ";
		outputDFAnode(head->nextnode[i], time);
	}
}

 void terminalreversing(newDFAnode* cur, vector<int>& re, bool*& flag);
 vector<int> findterminal(newDFAnode* head) {
	 vector<int> re;
	 bool* flag = new bool[mindfastate];
	 for (int i = 0; i < mindfastate; i++) {
		 flag[i] = false;
	 }
	 terminalreversing(head, re, flag);
	 return re;
 }
 void terminalreversing(newDFAnode* cur,vector<int>& re,bool*& flag) {
	 if (flag[cur->number]) return;
	 if (cur->isend)re.push_back(cur->number);
	 flag[cur->number] = true;
	 for (int i = 0; i < cur->nextnode.size(); i++) {
		 terminalreversing(cur->nextnode[i], re, flag);
	 }
 }
 void outputnewDFAnode(newDFAnode* head, int*& time);
 void outputnewDFA(newDFAnode* h) {
	 if (h == NULL) return;
	 int* outtime = new int[mindfastate];
	 for (int z = 0; z < state; z++) {
		 outtime[z] = 0;
	 }
	 cout << "Ç°Ò»¸ö×´Ì¬ -> ×ª»»×Ö·û -> ºóÒ»¸ö×´Ì¬" << endl;
	 cout << "³õÌ¬:";
	 outputnewDFAnode(h, outtime);
	 vector<int> ter=findterminal(h);
	 cout << "ÖÕÌ¬:";
	 for (int i = 0; i < ter.size(); i++) {
		 cout << ter[i] << " ";
	 }
 }
 void outputnewDFAnode(newDFAnode* head, int*& time) {
	 cout << head->number << endl;
	 time[head->number]++;
	 if (time[head->number] >= 2)return;
	 for (int i = 0; i < head->nextnode.size(); i++) {
		 cout << head->number << "             "<<head->transform[i]<<"            ";
		 outputnewDFAnode(head->nextnode[i], time);
	 }
 }

 bool yanzhen(newDFAnode* head,string s) {
	 newDFAnode* cur = head;
	 int i;
	 for (i = 0; i < s.length(); i++) {
		 int j;
		 for ( j = 0; j < cur->transform.size(); j++) {
			 if (s[i] == cur->transform[j]) {
				 cur = cur->nextnode[j];
				 break;
			 }
		 }
		 if (j == cur->transform.size()) break;
	 }
	 if (i < s.length()) return false;
	 else
	 {
		 if (cur->isend) return true;
		 else return false;
	 }
  }

