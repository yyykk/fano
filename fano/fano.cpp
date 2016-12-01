// fano.cpp : 定义控制台应用程序的入口点。
//

#include "iostream"
#include <fstream>
#include "map"
#include "string"
#include "math.h"
using namespace std;


multimap<float, pair<string, string>> W;

void show(multimap<float, pair<string, string>> word);

typedef decltype(W.begin()) map_pointer;

int initial(string file_name, int max){
	float probability = 0, temp = 0;
	string str = "";
	ifstream infile;
	infile.open(file_name, ios::in);
	for (int i = 0; i < max; ++i){
		infile >> temp >> str;
		probability += temp;
		W.insert({ temp, make_pair(str, "") });
	}
	if ((probability - 1) < 0.0001){
		return 1;
	}else{
		cout << "Sum of probability != 1" << endl;
		return 0;
	}
}

void coding(decltype(W.begin()) word, float value){
	if (fabs(word->first - value) < 0.00001){
		return;
	}
	float pre_value = 0, now_value = 0, high_value = 0, low_value = 0, temp = 0;
	auto pre_pointer = word;
	auto now_pointer = word;
	auto low = word;
	for (auto m = word; ; ++m){
		pre_pointer = now_pointer;
		now_pointer = m;
		pre_value = now_value;
		now_value += m->first;
		if (now_value >= (value / 2)){
			break;
		}
	}
	if ((now_value - (value / 2)) < ((value / 2) - pre_value)){
		high_value = now_value;
		low = now_pointer;
	}
	else{
		high_value = pre_value;
		low = pre_pointer;
	}
	low_value = value  - high_value;
	for (auto m = word;; ++m){
		temp += m->first;
		if (temp <= high_value){
			m->second.second += "1";
		}
		else{
			m->second.second += "0";
		}
		if (temp == value){
			break;
		}
	}
	coding(word, high_value);
	coding(++low, low_value);
}

int make_code(map_pointer high_pointer, map_pointer low_pointer){
	float high_value = high_pointer->first, low_value = low_pointer->first;
	auto hp = high_pointer, lp = low_pointer;
	hp->second.second += "1";
	lp->second.second += "0";
	while (1){
		if (++hp == lp){//cam not hp + 1
			--hp;
			if (hp != high_pointer){
				make_code(high_pointer, hp);
			}
			if (lp != low_pointer){
				make_code(lp, low_pointer);
			}
			return 1;
		}else if (high_value >= low_value){
			--hp;
			--lp;
			low_value += lp->first;
			lp->second.second += "0";
		}else{
			--hp;
			++hp;
			high_value += hp->first;
			hp->second.second += "1";
		}
	}
}

void show(multimap<float, pair<string, string>> word){
	ofstream Outfile;
	Outfile.open("text.txt", ios::out);
	Outfile << "***********************************************************" << endl;
	for (auto &m : word){
		Outfile << m.second.first << "\t" << m.second.second << endl;
		cout << m.second.first << "\t" << m.second.second << endl;
	}
}

int main(){
	initial("probability_68.txt", 68);
	//coding(W.begin(), 1);
	if (make_code(W.begin(), --W.end())){
		show(W);
	}else{
		cout << "error" << endl;
	}
	
	system("pause");
}





