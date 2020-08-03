#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;
class mainInterface{
	public:
		void readNaturalcmdLine(void);
		void runNowtimecmdLine(void);
		mainInterface();
	private:
		const char* cmdLineEncoder(void);
		string naturalcmdLine;
};

const char* mainInterface::cmdLineEncoder(void){   //Encode natural cmdline to system cmd line
	bool triggered=false;
	for(int i=0;i<this->naturalcmdLine.length();i++){    //delete the space befroe real line
		if(this->naturalcmdLine[i]==' '||this->naturalcmdLine[i]=='\t')
			continue;
		else {
			this->naturalcmdLine.erase(0, i);
			break;
		}
	} 
	
	for(int i=0;i<this->naturalcmdLine.length();i++){
		if(this->naturalcmdLine[i]==' '){
			string _name = this->naturalcmdLine.substr(0, i);
			this->naturalcmdLine.erase(0, i);
			_name += ".exe";                          //add .exe to privent from any possible error
			this->naturalcmdLine = _name + this->naturalcmdLine;
			triggered = true;
			break;
		}
	}
	if(!triggered)                 //if there is only one word in cmdline
		this->naturalcmdLine += ".exe";
	
	                               // as system require, system cmd line must be a const char*
	return this->naturalcmdLine.c_str();
}

void mainInterface::runNowtimecmdLine(void){
	system(this->cmdLineEncoder());                    //running now time command 
	return ;
}

void mainInterface::readNaturalcmdLine(void){
	getline(cin, this->naturalcmdLine);                  // read a cmd line inside object 
	if(this->naturalcmdLine=="exit") exit(0);            // exit as user need
}

mainInterface::mainInterface(){                          //constructor init the object with null string
	this->naturalcmdLine = "";
}

int main(){
	cout<<"CrosswordHelper [ver 2.0.1]"<<endl;
	cout<<"(c) 2020 Du ZhaoLong All rights reserved"<<endl<<endl<<endl; //welcome 
	mainInterface interface = mainInterface();
	while(1){
	cout<<"New cmd: ";            //tip 
	interface.readNaturalcmdLine();      //read a cmd in
	interface.runNowtimecmdLine();       //running cmdline
	}
	return 0;
}

