#include <fstream>
#include <iostream>
#include <cstdlib>
#include <map>
#include <cstring>
#include <vector>

using namespace std;

class selfImprove{
	public:
		void csvReader(string filename);
		void csvWriter(string filename);
		selfImprove();
	private:
		map<string, int> container;
		pair<string, int> getMax(map<string, int>* body);
		vector<string> wordCleaner(string word);
};


selfImprove::selfImprove(){
	this->container.insert(make_pair("<EMPTY>", -1));
	//init empty lebel
}


pair<string, int> selfImprove::getMax(map<string, int>* body){
	pair<string, int> max = make_pair(" ", -2);
	map<string, int>::iterator iter = body->begin();
	while(iter != body->end()){
		if(iter->second > max.second) max = *iter;     
		//use selection to get the max frequency word
		iter++;
	}
	return max;
}

void selfImprove::csvWriter(string filename){
	if(filename.length()<4||filename.substr(filename.length()-4, 4) != ".csv") filename += ".csv";
	ofstream out(filename);
	do{
		pair<string, int> a = getMax(&this->container);    //selcet sort
		if(a.first!="<EMPTY>"&&a.first!=""){
			string outdata = a.first + "," + to_string(a.second) + ",";  //to the csv format
			out << outdata << endl;	
		}  
		this->container.erase(a.first);                //erase after write
	}while(this->container.count("<EMPTY>")!=0);
	out.close();
}

void selfImprove::csvReader(string filename){
	if(filename.length()<4||filename.substr(filename.length()-4, 4) != ".csv") filename += ".csv";
	// add .csv to open right file
	ifstream in(filename);
	if(!in){
		cout<<"Error, can`t open file!"<<endl;
		cout<<"Check your file name again or contact with you admin!"<<endl;
		exit(0);
	}
	do{
		string line;
		getline(in, line);
		for(int i=0, j=0;i<line.length();i++){
			if(line[i]==',') {
				string first=line.substr(j, i-j);       //split number and word
				j = i;
				int second = atoi(line.substr(i+1, line.length()-2-i).c_str());   //switch to number
				vector<string> temp = this->wordCleaner(first);           //clean the useless char in word
				for(int i=0;i<temp.size();i++){
					pair<string, int> p = make_pair(temp[i], second);
					if(this->container.find(p.first) == this->container.end()) {   //if this word not appear yet
						this->container.insert(p);  //insert it
					}
					else{ 
						this->container[p.first] += p.second;    //add two frequency to 1
					}
				}
				break;
			}
		}
	}while(in.peek()!=EOF);
	in.close();
}

vector<string> selfImprove::wordCleaner(string word){
	vector<string> temp;
	if(strstr(word.c_str(), "[=")){
		for(int i=0,j=0;i<word.length();i++){
			if(word[i]=='=') j=i;        //deal with 'd etc.
			if(word[i]==']'){
				temp.push_back(word.substr(j+2, i-j-2));
				return temp;
			}
		}
	}
	if(strstr(word.c_str(), "/")){    //deal with P / p etc 
		temp.push_back(word.substr(word.length()-1, 1));
		return temp;
	}
	for(int i=0;i<word.length();i++)   //deal with dight
		if(isdigit(word[i]))
			return temp;
	int i, j;
	for(i=0,j=-1;i<word.length();i++){         //spilt those words with ' '
		if(isupper(word[i])) word[i] += 'a' - 'A';
		if(word[i]==' ') {
			temp.push_back(word.substr(j+1, i-j-1));
			j=i;
		}
	}
	temp.push_back(word.substr(j+1, i-j-1));
	return temp;
}

int main(int argc, char* argv[]){
	string _inName, _outName;
	if(argc==1){
		cout<<"Usage:"<<endl;
		cout<<"selfImprove [old csv_name] {-o|[new csv_name]}"<<endl;
		cout<<"old csv_name param is required!"<<endl;
		cout<<"default new csv_name is Improved+[old csv_name]"<<endl;
		cout<<"-o change data in origin file(not recommend)";          //usage tips
		return 0;
	}
	if(strstr(argv[1], ".")&&!strstr(argv[1], ".csv")){           // check .csv file 
		cout<<"Message from selfImprove.exe: please give a legel csv file!"<<endl;
		return 0;
	}else _inName = argv[1];
	if(argc==2) {
		_outName = "Improved_";
		_outName.append(argv[1]);                    // new data file name 
	}else if(!strcmp(argv[2], "-o")) _outName = argv[1];
	else _outName = argv[2];
	cout<<"Engaging selfImprove!"<<endl;
	selfImprove processflow = selfImprove();
	processflow.csvReader(_inName);
	processflow.csvWriter(_outName);
	cout<<"Improve complete!"<<endl;
	return 0;
}
