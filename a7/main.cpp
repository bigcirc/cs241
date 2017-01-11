#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>

using std::vector;
using std::map;
using std::cout;
using std::cerr;
using std::cin;
using std::istringstream;
using std::string;
using std::istream;
using std::endl;

map<string, string> trans;
vector<string> rule;
vector<string> symbols;
vector<string> states;
string ENDFILE;
int symRead = 0;


void skipLine(istream &in) {
    string s;
    getline(in, s);
}

void skipGrammar(istream &in) {
    int states, terminals;
    in >> states;
    skipLine(in);
    for (int i = 0; i < states; ++i) {
        skipLine(in);
    }
    in >> terminals;
    skipLine(in);
    for (int i=0; i < terminals; ++i) {
        skipLine(in);
    }
}

string reduce(int prodRule) {
    cout << rule[prodRule] << endl;
    string newTerm, oldTerm;
    int oldTermNum = 0;
    istringstream iss(rule[prodRule]);
    iss >> newTerm;
    while (!iss.eof()) {
        iss >> oldTerm;
        ++oldTermNum;
    }
    for (int i =0; i < oldTermNum; ++i) {
        symbols.pop_back();
        states.pop_back();
    }
    string searchKey = states[states.size()-1] + " " + newTerm;
    string result = trans[searchKey];
    istringstream iss1(result);
    string newState;
    iss1 >> oldTerm >> newState;
    symbols.push_back(newTerm);
    states.push_back(newState);
    return newTerm;
    
}

int main(int argc, const char * argv[]) {
    
    skipGrammar(cin);
    string startState;
    cin >> startState;
    
    int numRule, numStates, numTrans;
    
    // get production rule
    cin >> numRule;
    skipLine(cin);
    for (int i =0; i < numRule; ++i) {
        string s;
        getline(cin, s);
        rule.push_back(s);
        istringstream iss(s);
        string start;
        iss >> start;
        if (start == startState) ENDFILE = s;
    }
    
    cin >> numStates;
    cin >> numTrans;
    skipLine(cin);
    
    // get reduce & shift rules
    for (int i = 0; i < numTrans; ++i) {
        string s, start, input, action, end;
        getline(cin,s);
        istringstream iss(s);
        iss >> start >> input >> action >> end;
        string key = start + " " + input;
        string value = action + " " +end;
        trans[key] = value;
    }
    
    states.push_back("0");
    
    while (true) {
        string currSym;
        if (cin >> currSym) {
            ++symRead;
            string currState = states[states.size()-1];
            string searchKey  = currState + " " + currSym;
            if (! (trans.count(searchKey) > 0)) {
                cerr << "ERROR at " << symRead << endl;
                exit(EXIT_SUCCESS);
            } else {
                string result = trans[searchKey];
                istringstream iss(result);
                string action;
                iss >> action;
                if (action == "shift") {
                    string endSate;
                    iss >> endSate;
                    symbols.push_back(currSym);
                    states.push_back(endSate);
                } else if (action == "reduce") {
                    int prodRule;
                    iss >> prodRule;
                    bool finish = false;
                    string newTerm = reduce(prodRule);
                    bool error = false;
                    do {
                        string searchKey = states[states.size()-1] + " " + currSym;
                        if (trans.count(searchKey) > 0) {
                            result = trans[searchKey];
                            istringstream iss1(result);
                            iss1 >> action;
                            if (action == "shift") {
                                string endState;
                                iss1 >> endState;
                                symbols.push_back(currSym);
                                states.push_back(endState);
                                finish = true;
                            } else if (action == "reduce") {
                                iss1 >> prodRule;
                                reduce(prodRule);
                            }
                        } else {
                            cerr << "ERROR at " << symRead << endl;
                            exit(EXIT_SUCCESS);
                        }
                    } while (!finish);
                    
                    if (error) break;
                }
            }
        } else break;
    }
        cout << ENDFILE << endl;
}

