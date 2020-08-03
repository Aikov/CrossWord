#include <regex>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

class fqueryFlow{
	public :
		void csvReader(string filename);
		void requirementReader(string filename);
		fqueryFlow(string outname);
	private :
		regex pattern;
		vector<string> candidate;
		vector<string> c_dict;
		string outPutFilename;
		void docWriter(string filename);
		void setPattern(map<int, char> m, int size);
		vector<string> _query(int size);
};

void fqueryFlow::csvReader(string filename){
	if(filename.length()<4||filename.substr(filename.length()-4, 4) != ".csv") filename += ".csv";
	//add .csv to open correct file
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
				this->c_dict.push_back(first);
				//push dic to c_dict, beacuse of read order, this will be correct order.
				break;
			}
		}
	}while(in.peek()!=EOF);
	in.close();
}

void fqueryFlow::setPattern(map<int, char> m, int size){
	string p="^";
	map<int, char>::iterator iter=m.begin();
	for(int i=1;i<=size;i++){
		if(iter!=m.end()){
			if(i==iter->first){
				p += iter->second;
				iter++;
			}else p+="[\\w|-]"; //set regex pattern
		}
		else p+="[\\w|-]";
	}
	p+="$";
	this->pattern.assign(p);   //set
	return ;
}

vector<string> fqueryFlow::_query(int size){
	vector<string> candidateSet;
	int j=0;
	for(int i=0;i<this->c_dict.size()&&j<size;i++){
		if(regex_match((this->c_dict)[i], this->pattern)){
			candidateSet.push_back(this->c_dict[i]);        //add word to candidate set
			j++;
		}
	}
	return candidateSet;
}

void fqueryFlow::requirementReader(string filename){
	if(filename.length()<4||filename.substr(filename.length()-4, 4) != ".txt") filename += ".txt";
	//add .txt to open correct file
	ifstream in(filename);
	if(!in){
		cout<<"Error, can`t open file!"<<endl;
		cout<<"Check your file name again or contact with you admin!"<<endl;
		exit(0);
	}
	do{
		string line;
		getline(in, line);
		int i, j, size;
		bool isSizeSet = false;  //set word size
		bool isIllegal = false;
		map<int, char> m;
		for(int i=0,j=-1;i<=line.length();i++){
			if(line[i]==' '||i==line.length()){
				string now = line.substr(j+1, i-j-1);
				//get process string
				if(!isSizeSet){
					for(int k=0;k<now.length();k++){
						if(!isdigit(now[k])){
							cout<<"Message from fquery.exe: Requirement \""<<line<<"\" is illegal!"<<endl;
							isIllegal = true;
							break;
						}
					}
					if(isIllegal) break;
					else {
						size = stoi(now.c_str());
						isSizeSet=true;
					}
				}
				else{
					string withSure = now.substr(0, now.length()-1);
					//process order
					for(int i=0;i<withSure.length();i++){
						if(!isdigit(withSure[i])){
							cout<<"Message from fquery.exe: Requirement \""<<line<<"\" is illegal!"<<endl;
							isIllegal = true;
							break;
						}
					}
					if(isIllegal) break;
					else{
						int posi = stoi(withSure);
						if(posi>size||!isalpha(now[now.length()-1])){
							cout<<"Message from fquery.exe: Requirement \""<<line<<"\" is illegal!"<<endl;
							isIllegal = true;
							break;
						}else{
							char letter;
							if(isupper(now[now.length()-1])) letter = now[now.length()-1] + 'a' - 'A';
							else letter = now[now.length()-1];
							if(m.find(posi) == m.end()) {  
								m.insert(make_pair(posi, letter)); 
								//add order to map
							}
							else{ 
								m[posi] = letter;
							}
						}
					}
				}
				j=i;
			}
		}
		if(isIllegal) continue;
		ofstream out(this->outPutFilename, ios::app);
		//open at append mode
		out<<line;
		out.close();
		this->setPattern(m, size);
		this->candidate = this->_query(20);
		this->docWriter(this->outPutFilename);
		//write candidate set to output file 
	}while(in.peek()!=EOF);
	in.close();
}

void fqueryFlow::docWriter(string filename){
	ofstream out(filename, ios::app);
	if(this->candidate.size()==0){
		out<<endl<<"No match to this requirement.";
		//deal with candidate set is empty
		out<<endl<<endl<<endl<<endl;
		return ;
	}
	for(int i=0;i<this->candidate.size();i++){
		if(i%5==0) out<<endl;
		//make a good shape
		out<<candidate[i]<<" ";
	}
	out<<endl<<endl<<endl<<endl;
}

fqueryFlow::fqueryFlow(string outname){
	if(outname.length()<4||outname.substr(outname.length()-4, 4) != ".txt") outname += ".txt";
	this->outPutFilename = outname;
}

int main(int argc, char* argv[]){
	string _inNameCSV;
	string _inNameDOC;
	string _outNameDOC;
	if(argc<3){
		cout<<"Usage:"<<endl;
		cout<<"fquery [csv_name] [requirement_name] [output_name]"<<endl;
		cout<<"csv_name and requirement_name are required!"<<endl;
		cout<<"outputname is optional, default is \"newCandidate.txt\""<<endl;
		//usage tips
		return 0;
	}else if(strstr(argv[1], ".")&&!strstr(argv[1], ".csv")){
		cout<<"Message from fquery.exe: please give a legal csv file!"<<endl;
		return 0;
	}else if(strstr(argv[2], ".")&&!strstr(argv[2], ".txt")){
		cout<<"Message from fquery.exe: please give a legal requirement file!"<<endl;
		return 0;
	}else{
		_inNameCSV = argv[1];
		_inNameDOC = argv[2];
	}
	if(argc==4&&strstr(argv[3], ".")&&!strstr(argv[3], ".txt")){
		cout<<"Message from fquery.exe: Your output file name is illegal, using default name."<<endl;
		_outNameDOC = "newCandidate.txt";
	}else if(argc!=4){
		_outNameDOC = "newCandidate.txt";
	}else _outNameDOC = argv[3];
	
	fqueryFlow f = fqueryFlow(_outNameDOC);
	f.csvReader(_inNameCSV);
	f.requirementReader(_inNameDOC);
	return 0;
}
