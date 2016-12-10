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
	for (auto &w : word){
		cout << w.first << "\t" << w.second.first << "\t" << w.second.second << endl;
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

unsigned char bin_to_8(string::iterator bin){
	unsigned char num8 = 0;
	for (int i = 8; i != 0; --i, ++bin){
		if (*bin == '1'){
			num8 += pow(2, i - 1);
		}
	}
	return num8;
}

void init8(string &str){
	for (int i = (8 - (str.size() % 8)); i != 0; --i){
		str += "0";
	}
}

void test(char *file_name, string code){
	//char md[100] = "md ";
	//strcat(md, file_name);
	//system(md);
	ofstream outfile;
	string file = "./test/" + string(file_name) + ".dat";
	outfile.open(file, ios::binary | ios::out);
	init8(code);
	for (auto iter = code.begin(); iter != code.end(); iter += 8){
		unsigned  char temp = bin_to_8(iter);
		outfile.write((char*)&temp, sizeof(temp));
	}
	outfile.close();
}


int main(){
	char* text[5] = { "demo0", "demo1", "demo2", "demo3", "demo4" };
	for (int i = 0; i < 5; ++i){
		string file_name = "./test/" + string(text[i]) + ".txt";
		fano *demo = new fano(file_name);//创建对象
		string word_2_code = demo->to_fano_code();
		demo->show();
		test(text[i], word_2_code);
		delete demo;
	}
	system("pause");
}





