#include <iostream>
#include <fstream>
#include <map>
#include <cstdlib>
#include <cstring>

using namespace std;

class mergeFlow{
	public:
		void csvReader(string filename);
		void csvWriter(string filename);
		mergeFlow();
	private:
		map<string, int> container;
		pair<string, int> getMax(map<string, int>* body);
};

mergeFlow::mergeFlow(){
	// init with a enpty lebel
	this->container.insert(make_pair("<EMPTY>", -1));
}

pair<string, int> mergeFlow::getMax(map<string, int>* body){   //selecetion sort
	pair<string, int> max = make_pair(" ", -2);
	map<string, int>::iterator iter = body->begin();
	while(iter != body->end()){
		if(iter->second > max.second) max = *iter;     
		//use selection to get the max frequency word
		iter++;
	}
	return max;
}

void mergeFlow::csvWriter(string filename){
	if(filename.length()<4||filename.substr(filename.length()-4, 4) != ".csv") filename += ".csv";
	//add .csv to open right file
	ofstream out(filename);
	do{
		pair<string, int> a = getMax(&this->container);    
		if(a.first!="<EMPTY>"&&a.first!=""){
			string outdata = a.first + "," + to_string(a.second) + ",";  // to csv format
			out << outdata << endl;	
		}  
		this->container.erase(a.first);                
	}while(this->container.count("<EMPTY>")!=0);
	out.close();
}

void mergeFlow::csvReader(string filename){
	if(filename.length()<4||filename.substr(filename.length()-4, 4) != ".csv") filename += ".csv";
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
				j = i;
				int second = atoi(line.substr(i+1, line.length()-2-i).c_str());
				pair<string, int> p = make_pair(first, second);
				if(this->container.find(p.first) == this->container.end()) {   //if this word not appear yet
					this->container.insert(p);  //insert it
				}
				else{ 
					this->container[p.first] += p.second;  // add to frequency to one
				}
				break;
			}
		}
	}while(in.peek()!=EOF);
	in.close();
}


int main(int argc, char* argv[]){
	string _inName1, _inName2, _outName;
	if(argc==1){
		cout<<"Usage:"<<endl;
		cout<<"merge [in csv_name1] [in csv_name2] [out csv_name]"<<endl;
		cout<<"2 in csv_name params are required!"<<endl;
		cout<<"default out csv_name is new_dict.csv"<<endl;
		return 0;
	}else if(argc==2){
		cout<<"Message from merge.exe: Please give 2 input files!"<<endl;
		return 0;
	}else if(strstr(argv[1], ".")&&!strstr(argv[1], ".csv")){
		cout<<"Message from merge.exe: please give a legel csv file!"<<endl;
		return 0;
	}else if(strstr(argv[2], ".")&&!strstr(argv[2], ".csv")){
		cout<<"Message from merge.exe: please give a legel csv file!"<<endl;
		return 0;
	}
	//usage tips
	if(argc == 3) _outName = "new_dict.csv";
	else _outName = argv[3];
	_inName1 = argv[1];
	_inName2 = argv[2];
	cout<<"Engaging Merge!"<<endl;
	mergeFlow p = mergeFlow();
	p.csvReader(_inName1);
	p.csvReader(_inName2);
	p.csvWriter(_outName);
	cout<<"Merge complete!"<<endl;
	return 0;
}
