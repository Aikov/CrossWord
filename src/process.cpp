#include <iostream>
#include <map>
#include <fstream>
#include <cstring>
using namespace std;
class processFlow{
	public:
		void docReader(string filename);
		void csvWriter(string filename);
		processFlow();
	private:
		map<string, int> container;
		bool isIgnore(string read);
		pair<string, int> getMax(map<string, int>*); 
		string split(string p);
};


pair<string, int> processFlow::getMax(map<string, int>* body){
	pair<string, int> max = make_pair(" ", -2);  //init compare pair
	map<string, int>::iterator iter = body->begin();
	while(iter != body->end()){
		if(iter->second > max.second) max = *iter;     
		//use selection to get the max frequency word
		iter++;
	}
	return max;
}

bool processFlow::isIgnore(string read){
	return (read[0]=='<' || read[read.length()-1] == '>');   //ignore the <> lebel in text begin
}

string processFlow::split(string p){
	for (int j = 0; j < p.length(); j++) {
        if (isupper(p[j])) p[j] += 'a' - 'A'; //switch all upper alpha to lower
    }
    int mark = 0, i = 0;
    for (; i < p.length(); i++) {
        if (!isalpha(p[i])) {       
            mark++;
        } else break;
    }
    //get the length of none-alpha char at the begin of each word
    p.erase(0, mark);  //erase them from word 
    for (mark = p.length(), i=0; i < p.length(); i++) {
        if (!isalpha(p[i]) && p[i] != '\0' && p[i]!='-') { 
            mark = i;
            break;
        }
    }
    //split none-alpha char from woed read in
    p.erase(mark, p.length());  //erase them
    return p;
}

processFlow::processFlow(){
	this->container.insert(make_pair("<EMPTY>", -1));
	//init container with a empty pair
}

void processFlow::docReader(string filename){
	if(filename.length()<4||filename.substr(filename.length()-4, 4) != ".txt") filename += ".txt";
	// add .txt to open correct file
	ifstream in(filename);
	if(!in){
		cout<<"Error, can`t open file!"<<endl;
		cout<<"Check your file name again or contact with you admin!"<<endl;
		exit(0);
	}
	do{
		string line;
		in>>line;      //read word from opened file
		if(!isIgnore(line)) {    //if this is not "<>"
			line = split(line);  //get the real word from read-in data
			if(this->container.find(line) == this->container.end()) {   //if this word not appear yet
				this->container.insert(make_pair(line, 1));  //insert it
			} 
			else{
				this->container[line] ++;    //else plus 1 to the frenquence of this word
			}
		}
	} while(in.peek()!=EOF);
	in.close();
}

void processFlow::csvWriter(string filename){
	if(filename.length()<4||filename.substr(filename.length()-4, 4) != ".csv") filename += ".csv";
	// add .csv to open correct file
	ofstream out(filename);
	do{
		pair<string, int> a = getMax(&this->container);    //select sort 
		if(a.first!="<EMPTY>"&&a.first!=""){     //detect end label
			string outdata = a.first + "," + to_string(a.second) + ",";  //change format to csv 
			out << outdata << endl;	           //write
		}  
		this->container.erase(a.first);        //erase after write 
	}while(this->container.count("<EMPTY>")!=0);
	out.close();
}

int main(int argc, char* argv[]){
	if(argc==1){
		cout<<"Usage:"<<endl;
		cout<<"process [doc_name] [csv_name]"<<endl;
		cout<<"doc_name param is required!"<<endl;
		cout<<"csv_name is optional, default is \" newDic.csv\""<<endl;    //usage tips
		return 0;
	}else if(argc==2&&strstr(argv[1], ".csv")){                      //check the first file name 
		cout<<"Message from process.exe: lack of input doc file name!";
		return 0;
	}
	string _inName = argv[1];
	string _outName;
	if(argc==3) _outName = argv[2];
	else _outName = "newDic.csv";
	cout<<"Engaging process!"<<endl;
	processFlow p = processFlow();
	p.docReader(_inName);
	p.csvWriter(_outName);
	cout<<"Process complete!"<<endl;
	return 0;
}
