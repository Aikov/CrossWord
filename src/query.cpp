#include <regex>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

class queryFlow{
	public :
		void csvReader(string filename);
		vector<string> _query(int size);
		void setPattern(map<int, char> m, int size);
	private :
		regex pattern;
		vector<string> c_dict;
};


void queryFlow::csvReader(string filename){
	if(filename.length()<4||filename.substr(filename.length()-4, 4) != ".csv") filename += ".csv";
	//add .csv  to open correct file
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
				string first=line.substr(j, i-j);
				//spilt to part in csv 
				this->c_dict.push_back(first);
				break;
			}
		}
	}while(in.peek()!=EOF);
	in.close();
}

void queryFlow::setPattern(map<int, char> m, int size){
	string p="^";        //beginning of regex
	map<int, char>::iterator iter=m.begin();
	for(int i=1;i<=size;i++){
		if(iter!=m.end()){
			if(i==iter->first){
				p += iter->second;
				iter++;
			}else p+="[\\w|-]";      //construct regex as ^[\w|-]fix[\w|-]..$
		}
		else p+="[\\w|-]";
	}
	p+="$";
	this->pattern.assign(p);   //set this regex as pattern 
	return ;
}

vector<string> queryFlow::_query(int size){
	vector<string> candidateSet;
	int j=0;
	for(int i=0;i<this->c_dict.size()&&j<size;i++){
		if(regex_match((this->c_dict)[i], this->pattern)){
			candidateSet.push_back(this->c_dict[i]);    //push this word to candidata word set 
			j++;
		}
	}
	return candidateSet;
}

int main(int argc, char* argv[]){
	string _inName;
	if(argc==1){
		cout<<"Usage:"<<endl;
		cout<<"query [csv_name]"<<endl;
		cout<<"[csv_name] which dictionary you need to query"<<endl;
		cout<<"csv_name param is required!"<<endl;
		return 0;                               //tips
	}else if(strstr(argv[1], ".")&&!strstr(argv[1], ".csv")){
		cout<<"Message from query.exe: please give a legel csv file!"<<endl;
		return 0;
	}else _inName = argv[1];
	queryFlow q = queryFlow();
	q.csvReader(_inName);        //open dictionary
	cout<<"Please input size of your word: ";
	int wordSize;
	cin>>wordSize;
	cout<<"Please input your condition in this format"<<endl;
	cout<<"3b means the third letter is b"<<endl;
	cout<<"please input one by one, like 3b 4c"<<endl;
	cout<<"if you want to end input, input a #"<<endl;
	cout<<"the last input of the same position will determine result!"<<endl;  //tips
	map<int, char> m;
	string temp;
	while(1){
		cin>>temp;
		if(temp=="#") break;
		string withSure = temp.substr(0, temp.length()-1);
		bool isIllegal = false;
		for(int i=0;i<withSure.length();i++){
			if(!isdigit(withSure[i])){               //check format of input order
				cout<<"Invaild input!"<<endl;
				isIllegal = true;
				continue;
			}
		}
		if(isIllegal) continue;
		int posi = stoi(withSure);
		if(posi>wordSize||!isalpha(temp[temp.length()-1])){
			cout<<"Invaild input!"<<endl;
			continue;
		}else{
			char letter;
			if(isupper(temp[temp.length()-1])) letter = temp[temp.length()-1] + 'a' - 'A';
			else letter = temp[temp.length()-1];
			if(m.find(posi) == m.end()) {  
					m.insert(make_pair(posi, letter));       //add to order map
				}
				else{ 
					m[posi] = letter;
				}
		}
	}
	cout<<"Input how many words you want: ";
	int setSize;
	cin>>setSize;
	q.setPattern(m, wordSize);
	vector<string> candidateSet = q._query(setSize);
	for(int i=0;i<candidateSet.size();i++){
		cout<<candidateSet[i]<<endl;
	}
	if(candidateSet.size()==0) cout<<"Sorry, no words satisfy your requirement in dictionary!"<<endl;
	cout<<"Query complete!"<<endl;
	return 0;
}

