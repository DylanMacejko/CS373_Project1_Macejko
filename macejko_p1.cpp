#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>

using namespace std;

struct transition{
	int input_state;
	char letter;
	int output_state;
};

struct state{
	int statenum;
	bool acceptstate;
	bool addedtoaccept;
	bool addedtoreject;
};

struct results{
	int state;
	int rejectstatus;
};

bool mycomparator(struct transition t1, struct transition t2){
	return t1.input_state < t2.input_state;
}

int main(int argc, char * argv[]){
	if(argc != 3){
		cout << "Incorrect number of arguments." << endl;
		cout << "Usage: ./macejko_p1 filename.txt inputString" << endl;
	}else{
		string inputString = (string) argv[2];
		vector<struct state> states;
		vector<struct transition> transitions;
		for(int i=0; i<1001; i++){
			struct state temp;
			temp.statenum = i;
			temp.acceptstate = false;
			temp.addedtoaccept = false;
			temp.addedtoreject = false;
			states.push_back(temp);
		}

		ifstream inputFile;

		inputFile.open(argv[1], ios::in);
		if(!inputFile.is_open()){
			cout << "The input file is empty!" << endl;
		}else{
			cout << "Starting file interpretation." << endl;
			string line;
			int start = -1;
			while(getline(inputFile, line)){
				cout << "Got the line: " << line << endl;
				if(line.substr(0, line.find('\t')) == "state"){
					line = line.substr(line.find('\t')+1);
					vector<string> stateparser;
					istringstream stringparser(line);
					string sub;
					while(getline(stringparser, sub, '\t')){
						stateparser.push_back(sub);
					}
					for(int i=0; i<stateparser.size(); i++){
						cout << "State parser i: " << stateparser[i] << endl;
						cout << (stateparser[i] == "start" || stateparser[i] == "accept") << endl;
					}
					for(unsigned long i = 1; i < stateparser.size(); i++){
						if(stateparser[i] == "accept"){
							states[stoi(stateparser[0])].acceptstate = true;
						}else if(stateparser[i] == "start"){
							start = stoi(stateparser[0]);
						}
					}
					stateparser.clear();
				}else if(line.substr(0, line.find('\t')) == "transition"){
					line = line.substr(line.find('\t')+1);
					vector<string> transitionparser;
					istringstream stringparser(line);
					string sub;
					int counter = 0;
					while(getline(stringparser, sub, '\t')){
						transitionparser.push_back(sub);
					}
					struct transition tempTransition;
					//cout << "This is the transition character: " << transitionparser[1] << endl;
					tempTransition.input_state = stoi(transitionparser[0]);
					tempTransition.letter = transitionparser[1].at(0);
					tempTransition.output_state = stoi(transitionparser[2]);
					transitions.push_back(tempTransition);
					transitionparser.clear();
				}
			}
			sort(transitions.begin(), transitions.end(), mycomparator);
			string inputstring = argv[2];
			//cout << "Here is the input string: " << inputstring << endl;
			vector<struct results> configurations;

			configurations.push_back({start, false});
			//cout << "Here is the starting configuration: " << configurations[0].state << endl;
			for(int character = 0; character < inputstring.length(); character++){
				//cout << "Here is the starting character: " << inputstring.at(character) << endl;
				//cout << "Here are all the current configurations: " << endl;
				//for(int i=0; i<configurations.size(); i++){
				//	cout << '\t' << configurations[i].state << endl;
				//}
				//cout << "Here is the number of current configurations: " << configurations.size() << endl;
				int currentconfigs = configurations.size();
				for(int config = 0; config<currentconfigs; config++){
					struct results currentEvaluatingState = configurations[config];
					if(configurations[config].rejectstatus){
						continue;
					}else{
						bool transitionstaken = false;
						int trans = 0;
						while((transitions[trans].input_state != configurations[config].state) && (trans < transitions.size())){
							trans++;
						}
						for(int tran = trans; tran<transitions.size() ; tran++){
							if(!transitionstaken && (transitions[tran].input_state == currentEvaluatingState.state) && (inputstring.at(character) == transitions[tran].letter)){
								configurations[config].state = transitions[tran].output_state;
								transitionstaken = true;
							}else if(transitionstaken && (transitions[tran].input_state == currentEvaluatingState.state) && (transitions[tran].letter == inputstring.at(character))){
								configurations.push_back({transitions[tran].output_state, false});
							}
						}
						if(!transitionstaken){
							configurations[config].rejectstatus = true;
						}
					}
				}

			}
			vector<int> accepts;
			vector<int> rejects;
			cout << "How many configurations are there? " << configurations.size() << endl;
			for(int i = 0; i<configurations.size(); i++){
				//cout << "Configurations state number: " << configurations[i].state << endl;
				//cout << "Configurations reject status: " << configurations[i].rejectstatus << endl;

				if(configurations[i].rejectstatus){
					continue;
				}else if(states[configurations[i].state].acceptstate && !states[configurations[i].state].addedtoaccept){
					accepts.push_back(configurations[i].state);
					states[configurations[i].state].addedtoaccept = true;
				}else if(!states[configurations[i].state].acceptstate && !states[configurations[i].state].addedtoreject){
					rejects.push_back(configurations[i].state);
					states[configurations[i].state].addedtoreject = true;
				}
			}
			if(accepts.size() == 0){
				cout << "reject";
				for(int i = 0; i<rejects.size(); i++)
					cout << " " << rejects[i];
				cout << endl;
			}else{
				cout << "accept";
				for(int j = 0; j<accepts.size(); j++)
					cout << " " << accepts[j];
				cout << endl;
			}
			
		}
	}


}
