// lexicalanalysis.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <string>
#include<stack>
#include "NFAnode.h"
#include"DFAnode.h"
using namespace std;

string addassosiation(string s) {
	bool flag = false;
	string result = "";
	for (int i = 0; i < s.length(); i++) {
		if (flag) {
			if (s[i] != '('&&s[i] != ')'&&s[i] != '*'&&s[i] != '|') {
				result += '+';
			}
			else if (s[i] == '(') {
				result += '+';
				flag = false;
			}
			else if (s[i] == '|') {
				flag = false;
			}
		}
		else {
			if (s[i] != '('&&s[i] != ')'&&s[i] != '*'&&s[i] != '|')
				flag = true;
		}
		result += s[i];
	}
	return result;
}
string zhongtohou(string s) {
	stack<char> p;
	s += '#';
	string result = "";
	p.push('#');
	for (int i = 0; i < s.length(); i++) {
		if (s[i] != '('&&s[i] != ')'&&s[i] != '*'&&s[i] != '|'&&s[i] != '+'&&s[i] != '#') {
			result += s[i];
			continue;
		}
		switch (s[i])
		{
		case '(': {
			p.push(s[i]);
			break;
		}
		case ')': {
			while (p.top() != '(') {
				result += p.top();
				p.pop();
			}
			p.pop();
			break;
		}
		case '|': {
			while (p.top() != '#'&&p.top()!='(') {
				result += p.top();
				p.pop();
			}
			p.push(s[i]);
			break;
		}
		case '+': {
			while (p.top() == '*'||p.top()=='+') {
				result += p.top();
				p.pop();
			}
			p.push(s[i]);
			break;
		}
		case '*': {
			while (p.top() == '*') {
				result += p.top();
				p.pop();
			}
			p.push(s[i]);
			break;
		}
		case '#': {
			while (p.top() != '#') {
				result += p.top();
				p.pop();
			}
			break;
		}
		default:
			break;
		}
	}
	return result;
}

 NFA houtoNFA(string s) {
	stack<NFA> p;
	for (int j = 0; j < s.length(); j++) {
		switch (s[j])
		{
		case'+': {
			NFA n1 = p.top();
			p.pop();
			NFA n2 = p.top();
			p.pop();
			p.push(n2 + n1);
			break;
		}
		case'|': {
			NFA n1 = p.top();
			p.pop();
			NFA n2 = p.top();
			p.pop();
			p.push(n2 | n1);
			break;
		}
		case'*': {
			NFA n = p.top();
			p.pop();
			p.push(n.oper());
			break;
		}
		default: {
			p.push(NFA::singlechar(s[j]));
			break;
		}
		}
	}
	return p.top();
}

int main()
{
	string s;
	cout << "此程序正则表达式规则：（1）或只能用|不能用+ （2）大写E表示空不能作为输入 （3）括号用英文括号 "<< endl;
	cout << "（若输入程序不出dfa，表明输入格式错误）" << endl;
	cout << "请输入一个正则表达式" << endl;
	cin >> s;
	cout << "添加隐含连接符+得到" << endl;
	cout << addassosiation(s) << endl;
	cout << "转后缀表达式:" << endl;
	cout << zhongtohou(addassosiation(s)) << endl;
	NFA RE=houtoNFA(zhongtohou(addassosiation(s)));
	//输出NFA
	//NFA::output(RE);
	//cout << endl;
	DFAnode* DFAre=NFAtoDFA(RE, s);
	//输出合并前的DFA
	//outputDFA(DFAre);
	//cout << endl;
	cout << "最终DFA为" << endl;
	newDFAnode* minDFA=mergeDFA(RE, s, DFAminimize(DFAre, s));
	outputnewDFA(minDFA);
	cout << endl;
	cout << endl;
	while (1) {
		string s1;
		cout << "请输入一个验证字符串:";
		cin >> s1;
		if (yanzhen(minDFA, s1)) cout << "字符串与正则式匹配" << endl;
		else cout << "字符串与正则式不匹配" << endl;
		cout << endl;
	}
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
