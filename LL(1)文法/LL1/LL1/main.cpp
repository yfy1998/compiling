#include<iostream>
#include<string>
#include<vector>
#include<stack>
using namespace std;
static vector<string> cfg;
static vector<char> noter;
static vector<char> ter;
static vector<vector<char>> First;
static vector<vector<char>> oldFollow;
static vector<vector<char>> Follow;
static vector<vector<string>> parsingtable;
static char Begin;
bool isterminal(char a) {
	if (a >= 65 && a <= 90)return false;
	return true;
}
bool isexist(vector<char> set, char a);
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
		if (right[i] == "$") return true;
	}
	return false;
}
vector<char> first(char noter) {
	vector<string> right;
	for (int i = 0; i < cfg.size(); i++) {
		if (cfg[i][0] == noter) {
			int j = 3;
			string s = "";
			while (j<cfg[i].length()) {
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
					if (first(right[i][j])[z] == '$') continue;
					if (!isexist(firstset, first(right[i][j])[z]))
					firstset.push_back(first(right[i][j])[z]);
				}
				if (!istokong(right[i][j]))break;
			}
		}
		if (j == right[i].length()) firstset.push_back('$');
	}
	return firstset;
}
void outputFirst() {
	for (int i = 0; i < First.size(); i++) {
		cout << "first(" << noter[i] << "):";
		for (int j = 0; j < First[i].size();j++) {
			cout << First[i][j] << "   ";
		}
		cout << endl;
	}
}

vector<string> Right(char noter) {
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
	return right;
}

void findternimal() {
	for (int i = 0; i < noter.size(); i++) {
		vector<string> r = Right(noter[i]);
		for (int j = 0; j < r.size(); j++) {
			for (int z = 0; z < r[j].length(); z++) {
				if (isterminal(r[j][z]) && r[j][z] != '$') {
					if (!isexist(ter, r[j][z])) ter.push_back(r[j][z]);
;				}
			}
		}
	}
}
int noterfindindex(char noterminal) {
	for (int i = 0; i < noter.size(); i++) {
		if (noterminal == noter[i]) return i;
	}
	return -1;
}

bool isexist(vector<char> set,char a) {
	for (int i = 0; i < set.size(); i++) {
		if (set[i] == a) return true;
	}
	return false;
}

void follow(char noterminal) {
	for (int i = 0; i < noter.size(); i++) {
		vector<string> right = Right(noter[i]);
		for (int j = 0; j < right.size(); j++) {
			string s = right[j];
			int z = 0;
			bool flag = false;
			while (z<s.length()) {
				if (flag) {
					int index = noterfindindex(noterminal);
					if (isterminal(s[z])) {
						if (!isexist(Follow[index], s[z])) Follow[index].push_back(s[z]);
						break;
					}
					else {
						int index1= noterfindindex(s[z]);
						bool f = false;
						for (int i = 0; i < First[index1].size(); i++) {
							if (First[index1][i] != '$') {
								if(!isexist(Follow[index], First[index1][i]))
								Follow[index].push_back(First[index1][i]);
							}
							else f = true;
						}
						if (!f)break;
					}
				}
				if (s[z] == noterminal) {
					if (z + 1 == s.length()) {
						if (noter[i] == noterminal) {
							break;
						}
						else {
							int index1 = noterfindindex(noterminal);
							int index2 = noterfindindex(noter[i]);
							if (index1 >= 0&&index2>=0) {
								for (int r = 0; r < Follow[index2].size(); r++) {
									if (!isexist(Follow[index1], Follow[index2][r])) {
										Follow[index1].push_back(Follow[index2][r]);
										}
								}
							}
							break;
						}
					}
					else {
						flag = true;
					}
				}
				z++;
			}
			if (flag&&z == s.size()) {
				int index1 = noterfindindex(noterminal);
				int index2 = noterfindindex(noter[i]);
				if (index1 >= 0 && index2 >= 0) {
					for (int r = 0; r < Follow[index2].size(); r++) {
						if (!isexist(Follow[index1], Follow[index2][r])) {
							Follow[index1].push_back(Follow[index2][r]);
						}
					}
				}
			}
		}
	}
}

void caculatefollow() {
	Follow[0].push_back('@');
	while (1) {
		for (int i = 0; i < noter.size(); i++) {
			follow(noter[i]);
		}
		if (oldFollow == Follow) break;
		oldFollow = Follow;
	}
}

void outputFollow() {
	for (int i = 0; i < Follow.size(); i++) {
		cout << "follow:(" << noter[i] << "):";
		for (int j = 0; j < Follow[i].size(); j++) {
			cout << Follow[i][j] << "   ";
		}
		cout << endl;
	}
}

vector<char> rightfirst(char noterminal,string s) {
	vector<char> re;
	int i;
	for (i=0; i < s.length(); i++) {
		if (isterminal(s[i])) {
			if (s[i] != '$') {
				re.push_back(s[i]);
				break;
			}
		}
		else {
			int noindex = noterfindindex(s[i]);
			for (int z = 0; z < First[noindex].size(); z++) {
				if (First[noindex][z] == '$') continue;
				re.push_back(First[noindex][z]);
			}
			if (!istokong(s[i]))break; 
		}
	}
	if (i == s.length()) {
		int index = noterfindindex(noterminal);
		for (int i = 0; i < Follow[index].size(); i++) {
			if (!isexist(re, Follow[index][i]))
				re.push_back(Follow[index][i]);
		}
	}
	return re;
}

int findterdindex(char terminal) {
	for (int i = 0; i < ter.size(); i++) {
		if (ter[i] == terminal)return i;
	}
	return -1;
}
void calcuparsingtable() {
	for (int i = 0; i < noter.size(); i++) {
		vector<string> a(ter.size()+1,"");
		parsingtable.push_back(a);
	}
	for (int i = 0; i < noter.size(); i++) {
		vector<string> rightset = Right(noter[i]);
		int index1 = noterfindindex(noter[i]);
		for (int j = 0; j < rightset.size(); j++) {
			string s = rightset[j];
			vector<char> fillter = rightfirst(noter[i], s);
			for (int z = 0; z < fillter.size(); z++) {
				char terminal = fillter[z];
				int index2;
				if (terminal == '@') index2 = ter.size();
				else index2 = findterdindex(terminal);
				parsingtable[index1][index2] = s;
			}
		}
	}
}

void outputpasingtable() {
	cout << "parsing table:" << endl;
	cout << "   ";
	for (int i = 0; i < ter.size() + 1; i++) {
		if (i == ter.size())cout << '@' << endl;
		else cout << ter[i] << "\t";
	}
	for (int i = 0; i < parsingtable.size(); i++) {
		cout << noter[i] << " ";
		for (int j = 0; j < parsingtable[0].size(); j++) {
			cout << parsingtable[i][j] << "\t";
		}
		cout << endl;
	}
}

void outputstack(stack<char> a) {
	while (!a.empty()) {
		cout << a.top();
		a.pop();
	}
}
void check(string s) {
	stack<char> q;
	q.push('@');
	q.push(Begin);
	s += '@';
	cout << "stack\t\tinput\t\taction" << endl;
	int i = 0;
	while (!q.empty()) {
		outputstack(q);
		cout << "\t\t";
		cout << s.substr(i, s.length() - i) << '\t';
		if (i >=4)cout << '\t';
		if (q.top() == '@') {
			if (s[i] == '@') cout << "ACCEPT"<<endl;
			else cout << "NOT ACCEPT"<<endl;
			break;
		}
		if (s[i] == '@') {
			if (q.top() == '@') cout << "ACCEPT" << endl;
			else cout << "NOT ACCEPT" << endl;
			break;
		}
		if (q.top() == s[i]) {
			cout << "terminal" << endl;
			q.pop();
			i++;
			continue;
		}
		if (!isexist(ter, s[i])&&s[i]!='@') {
			cout << "NOT ACCEPT" << endl;
			break;
		}
		int index1 = noterfindindex(q.top());
		int index2;
		if (s[i] == '@') index2 = ter.size();
		else index2 = findterdindex(s[i]);
		if (parsingtable[index1][index2] != "") {
			string l = parsingtable[index1][index2];
			cout << l<<endl;
			q.pop();
			if (l == "$")continue;
			for (int j = l.length() - 1; j >= 0; j--) {
				q.push(l[j]);
			}
			continue;
		}
		
	}
}
int main() {
	cout << "注意:" << endl;
	cout << "(1)空字符为$，输入时空字符用$替换；" << endl;
	cout << "(2) @表示终止符" << endl << endl;
	cout << "依次输入消除左递归后的上下文无关文法产生式（输入0结束）："<<endl;
	string s;  
	cin >> s;
	while (s != "0") {
		cfg.push_back(s);
		cin >> s;
	}
	Begin = cfg[0][0];
	for (int i = 0; i < cfg.size(); i++) {
		int j;
		for (j = 0; j < noter.size(); j++) {
			if (cfg[i][0] == noter[j]) break;
		}
		if (j == noter.size()) noter.push_back(cfg[i][0]);
	}
	findternimal();
	for (int i = 0; i < noter.size(); i++) {
		First.push_back(first(noter[i]));
	}
	for (int i = 0; i < noter.size(); i++) {
		vector<char> a;
		oldFollow.push_back(a);
		Follow.push_back(a);
	}
	outputFirst();
	cout << endl;
	caculatefollow();
	outputFollow();
	cout << endl;
	calcuparsingtable();
	outputpasingtable();
	cout << endl;
	cout << "输入一个验证字符串:";
	string ch;
	cin >> ch;
	check(ch);
	system("pause");
	return 0;
}