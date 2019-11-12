// Main: parses input and sends it to be processed and evaluated, then writes the results

#include<iostream>
#include<string>
#include<cmath>
#include<stack>
#include<ctype.h>
#include<vector>
#include<algorithm>
#include<fstream>
#include<sstream>

#include "preprocessor.h"
#include "evaluator.h"

using namespace std;

int main(int argc, char* args[]) {

	string inputFile;
	string outputFile = ""; // Initialised as empty string to later check if we have to print to terminal

	for(int i = 0; i < argc; i++) {
		string str(args[i]);
		if (str == "-i") {
			string inputFileStr(args[i+1]); // The parameter after -i is the input file name
			inputFile = inputFileStr; // Since inputFileStr is only valid within the loop
		}

		if (str == "-o") {
			string outputFileStr(args[i+1]); // Same for the output
			outputFile += outputFileStr;
		}
	}

	vector<Expression> expressions;
	vector<string> invExps; // A vector to store the invalid expression names
	ifstream fin(inputFile);

	if (fin.is_open()) {
		string line;
		int lineCount = 0;
		while(getline(fin, line)) {
			cout << line << endl;
			lineCount++;
			//cout << line << endl;
			string expName = "";
			string expValue = "";
			int nameEnd = line.find("=");
			int valueEnd = line.find(";");
			int leftParNum = count(line.begin(), line.end(), '('); // Since we'll check whether the parentheses are properly paired
			int rightParNum = count(line.begin(), line.end(), ')');

			if(valueEnd != string::npos && nameEnd != string::npos && leftParNum == rightParNum && line.at(line.length() - 2) == ' ' && line.back() == ';' && line.at(nameEnd-1) == ' ' && line.at(nameEnd+1) == ' ') { // Sanity checks
				for(int i = 0; i < nameEnd - 1; i++) { // Parses variable names
					expName += line.at(i);
				}

				for(int j = nameEnd + 2; j < valueEnd - 1; j++) { // Parses expression
					expValue += line.at(j);
				}

				vector<string> startingTokens;
				vector<string> startingPostfix;
				Expression expr1 = {expName, expValue, startingTokens, startingPostfix, 0, false};
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

				cout << "Invalid syntax on line " << lineCount << " for expression " << invExpName << ". Expression will not be evaluated. Reason for error: ";

				if(nameEnd == string::npos) { cout << "no equal sign." << endl; }
				else if(leftParNum != rightParNum) { cout << "unequal brackets." << endl; }
				else if(line.at(line.length() - 2) != ' ') { cout << "no space before the semicolon." << endl; }
				else if(line.at(nameEnd-1) != ' ' || line.at(nameEnd+1) != ' ') { cout << "no spaces between the equal sign." << endl; }
				else if(line.back() != ';' || valueEnd == string::npos) { cout << "no semicolon at end or at all." << endl; }

			}


		}
		fin.close();
	}

	if(expressions.size() == 0) { // If we have no valid expressions, we just stop
		return EXIT_SUCCESS;
	}

	for(int i = 0; i < invExps.size(); i++) { // Cleans out expressions that rely on invalid expressions
		for(int j = 0; j < expressions.size(); j++) {
			string expE = expressions.at(j).exp;
			string lastChar(1, expE.back()); // To make sure there's no operator at the end
			cout << lastChar << endl;
			if(expE.find(invExps.at(i)) != string::npos) {
				cout << "Expression " << expressions.at(j).name << " relies on an invalid expression. It will not be evaluated." << endl;
				invExps.push_back(expressions.at(j).name); // To ensure all invalids are caught
				expressions.erase(expressions.begin() + j);
			}
			else if(isOperator(lastChar)) {
				cout << "Expression " << expressions.at(j).name << " has an operator at the end, which is invalid. It will not be evaluated." << endl;
				invExps.push_back(expressions.at(j).name);
				expressions.erase(expressions.begin() + j);
			}
		}
	}

	for(int i = 0; i < expressions.size(); i++) {
		cout << expressions[i].exp << endl;
	}

	for(int l = 0; l < expressions.size(); l++) { // Tokenises every expression and converts them to postfix
 		Expression& exp = expressions.at(l);
 		exp.tokens = tokenise(exp.exp);

 		cout << "TOKENISED EXPRESSION IS: ";
	 	for(int j = 0; j < exp.tokens.size(); j++) {
			cout << exp.tokens.at(j) << ',';
		}
		cout << endl;

 		exp.postfix = infix2postfix(exp.tokens);

 		cout << "POSTFIX IS: ";
 		for(int k = 0; k < exp.postfix.size(); k++) {
 			cout << exp.postfix.at(k) << ",";	
 		}
 		cout << endl;
 	}

	int expCount = expressions.size();
	int i = 0;
 	while(expCount > 0) { // Runs until all expressions are evaluated
		Expression& exp = expressions.at(i);
		//cout << exp.name << " evaluation status in main is " << exp.isEval << endl;
 		if(exp.isEval == false) {
  			evaluate(exp, expressions, expCount);
 		}
 			
/* 		if(exp.isEval == true) {
 			cout << "The answer is " << exp.ans << endl;
 		}*/

 		//cout << exp.name << " evaluation status main is " << exp.isEval << endl;
 		//cout << endl;
 		i++;
 		if(i == expressions.size()) { // If we reach the end of our list once and there are still expressions left to evaluate, run the loop again
 			i = 0;
 		}
 	}

 	if(outputFile != "") { // If we have an output file, write results to it
	 	ofstream fout(outputFile);
	 	if(fout.is_open()) {
		  	for(int i = 0; i < expressions.size(); i++) {
		 		Expression& exp = expressions.at(i);
		 		ostringstream ansSS; // A stringstream is the best way to cast a float to string (to_string leaves too many zeros)
		 		ansSS << exp.ans;
		 		string ansString(ansSS.str());
		 		string line = exp.name + " = " + ansString +"\n"; // Proper formatting of line
		 		fout << line;
		 	}
		 	fout.close();
	 	}
 	}
 	else { // If not, just print them on the terminal
	  	for(int i = 0; i < expressions.size(); i++) {
	 		Expression& exp = expressions.at(i);
	 		ostringstream ansSS;
	 		ansSS << exp.ans;
	 		string ansString(ansSS.str());
	 		cout << exp.name << " = " << exp.ans << endl;
 		}	
 	}

 	return EXIT_SUCCESS;
}