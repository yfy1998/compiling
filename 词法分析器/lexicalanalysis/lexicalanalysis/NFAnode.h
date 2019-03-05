#pragma once
#include<vector>
#include<string>
#include<stack>
#include<iostream>
using namespace std;
struct NFAnode{
	int number;
	char transform;
	vector<NFAnode*> nextnode;
	NFAnode(int a, char t) {
		number = a;
		transform = t;
	}
	void addnextnode(NFAnode*& next) {
		if (next == NULL) return;
		nextnode.push_back(next);
	}
};

class NFA {
public:
	NFA() {
		head = NULL;
		end = NULL;
	}
	NFA(NFAnode* h,NFAnode* e) {
		head = h;
		end = e;
	}
	static NFA singlechar(char a) {
		NFAnode* n1 = new NFAnode(i++, 'E');
		NFAnode* n2 = new NFAnode(i++, a);
		n1->addnextnode(n2);
		return NFA(n1, n2);
	}
	NFA operator +(NFA n) {
		end->addnextnode(n.head);
		return NFA(this->head, n.end);
	}

	NFA operator |(NFA n) {
		NFAnode* n1 = new NFAnode(i++, 'E');
		NFAnode* n2 = new NFAnode(i++, 'E');
		n1->addnextnode(this->head);
		n1->addnextnode(n.head);
		this->end->addnextnode(n2);
		n.end->addnextnode(n2);
		return NFA(n1, n2);
	}
	NFA oper() {
		NFAnode* n1 = new NFAnode(i++, 'E');
		NFAnode* n2 = new NFAnode(i++, 'E');
		n1->addnextnode(this->head);
		n1->addnextnode(n2);
		this->end->addnextnode(n2);
		this->end->addnextnode(this->head);
		return NFA(n1, n2);
	}
	static void output(NFA n) {
		if (n.head == NULL) return;
		int* outtime=new int[i];
		for (int z = 0; z < i; z++) {
			outtime[z] = 0;
		}
		outputnode(n.head,outtime);
		cout << "end:"<<n.end->number;
	}
	static void outputnode(NFAnode* head,int*& time) {
		cout << head->transform << "   " << head->number << endl;
		time[head->number]++;
		if (time[head->number] == 2)return;
		for (int i= 0; i < head->nextnode.size(); i++) {
			cout << head->number << "   ";
			outputnode(head->nextnode[i],time);
		}
	}
	NFAnode* head;
	NFAnode* end;
	static int i;
};

int NFA::i = 0;

