// fano.cpp : 定义控制台应用程序的入口点。
//

#include "iostream"
#include "math.h"
#include "map"
#include "fstream"
#include "sstream"
using namespace std;

multimap<double, pair<string, string>> W;

typedef multimap<double, pair<string, string>>::iterator map_pointer;

class fano{
public:
	fano(string file_name);
	void show();
	string to_fano_code(string str = "");

private:
	int get_probability(string file_name);
	int make_code(map_pointer begin_p, map_pointer end_p);

private:
	multimap<double, pair<string, string>> word;
	multimap<string, string> already;
	string get_text;
};

fano::fano(string file_name){
	get_probability(file_name);
	make_code(word.begin(), --word.end());
	for (auto &w : word){
		already.find(w.second.first)->second = w.second.second;
	}
}

int fano::get_probability(string file_name){
	double word_amount = 0;
	double probability = 0;
	ifstream infile;
	ofstream outfile;
	map<char, double> word_count;
	infile.open(file_name, ios::in);
	if (!infile){
		cout << "can not open word text" << endl;
		return 0;
	}else{
		getline(infile, get_text);
	}
	for (auto &s : get_text){
		++word_count[s];
		++word_amount;
	}
	outfile.open("probability.txt", ios::out);
	for (auto &w : word_count){
		ostringstream stream;
		probability = w.second / word_amount;
		stream << w.first;
		outfile << stream.str() << "\t" << probability << endl;
		word.insert({probability, make_pair(stream.str(), "")});
		already.insert(make_pair(stream.str(), ""));
	}
	return 1;
}

int fano::make_code(map_pointer high_pointer, map_pointer low_pointer){
	double high_value = high_pointer->first, low_value = low_pointer->first;
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

void fano::show(){
	for (auto &w : already){
		cout << w.first << "\t" << w.second << endl;
	}
}

string fano::to_fano_code(string str){
	string code = "";
	if (str == ""){
		str = get_text;
	}
	for (auto &s : str){
		ostringstream stream;
		stream << s;
		code += already.find(stream.str())->second;
	}
	return code;
}

int main(){
	fano f("demo.txt");
	f.show();
	cout << f.to_fano_code() << endl;
	system("pause");
}





