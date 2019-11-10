// Main: parses input and sends it to be processed and evaluated, then writes the results

#include<iostream>
#include<string>
#include<cmath>
#include<stack>
#include<ctype.h>
#include<vector>
#include<algorithm>
#include<fstream>

#include "preprocessor.h"
#include "evaluator.h"

using namespace std;

struct Expression {
	string name;
	string exp;
	float ans;
	bool isEval;
	//Expression() : ans(0), isEval(false) {}
};

int main() {

	vector<Expression> expressions;
	vector<string> invExps; // A vector to store the invalid expression names
	ifstream fin("file2.txt");

	if (fin.is_open()) {
		string line;
		int lineCount = 1;
		while(getline(fin, line)) {
			lineCount++;
			//cout << line << endl;
			string expName = "";
			string expValue = "";
			int nameEnd = line.find("=");
			int valueEnd = line.find(";");
			int leftParNum = count(line.begin(), line.end(), '('); // Since we'll check whether the parantheses are properly paired
			int rightParNum = count(line.begin(), line.end(), ')');

			if(valueEnd != string::npos && leftParNum == rightParNum && line.at(line.length() - 2) == ' ' && line.at(nameEnd-1) == ' ' && line.at(nameEnd+1) == ' ') { // Sanity checks to ensure proper semicolon and equal sign placement
				for(int i = 0; i < nameEnd - 1; i++) { // Parses variable names
					expName += line.at(i);
				}

				for(int j = nameEnd + 2; j < valueEnd - 1; j++) { // Parses expression
					expValue += line.at(j);
				}

				Expression expr1 = {expName, expValue, 0, false};
				expressions.push_back(expr1);

				expName = "";
				expValue = "";
				if (fin.eof()) {
					break; // Prevents duplication of last line
				}
			}
			else { // We add the names of invalid expressions to a separate vector to cross-check with our expressions list later
				string invExpName = "";
				int i = 0;
				while(isalpha(line.at(i))) {
					invExpName += line.at(i);
					i++;
				}
				invExps.push_back(invExpName);
				cout << "Invalid syntax on line " << lineCount << " for expression " << invExpName << ". Expression will not be evaluated." << endl;
			}


		}
	}
	fin.close();

	for(int i = 0; i < invExps.size(); i++) { // Cleans out expressions that rely on invalid expressions
		for(int j = 0; j < expressions.size(); j++) {
			string expE = expressions.at(j).exp;
			if(expE.find(invExps.at(i)) != string::npos) {
				cout << "Expression " << expressions.at(j).name << " relies on an invalid expression. It will not be evaluated." << endl;
				expressions.erase(expressions.begin() + j);
			}
		}
	}

	for(int i = 0; i < expressions.size(); i++) {
		cout << expressions[i].exp << endl;
	}

 	for(int i = 0; i < expressions.size(); i++) {
 		Expression expToTokenise = expressions.at(i);
 		vector<string> tokenisedExp = tokenise(expToTokenise.exp);

 		cout << "TOKENISED EXPRESSION IS: ";
	 	for(int i = 0; i < tokenisedExp.size(); i++) {
			cout << tokenisedExp.at(i) << ',';
		}
		cout << endl;

 		vector<string> postfixExp = infix2postfix(tokenisedExp);

 		cout << "POSTFIX IS: ";
 		for(int i = 0; i < postfixExp.size(); i++) {
 			cout << postfixExp.at(i) << ",";	
 		}
 		cout << endl;
 	}

}