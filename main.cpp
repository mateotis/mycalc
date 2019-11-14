// Main: parses input and sends it to be processed and evaluated, then writes the results

#include<iostream>
#include<string>
#include<vector>
#include<ctype.h> // For isalpha/isdigit/other type checks
#include<algorithm> // For find/count methods
#include<fstream> // For input/output
#include<sstream> // For copying float output into file

#include "preprocessor.h"
#include "evaluator.h"

using namespace std;

void invalidFilter(vector<Expression>& expressions, vector<string>& invExps) { // Cleans out expressions that are invalid or rely on invalid expressions

	for(int i = 0; i < invExps.size(); i++) {
		for(int j = 0; j < expressions.size(); j++) {
			string expE = expressions.at(j).name + " = " + expressions.at(j).exp; // For readability; we are looking at the entire expression (name and value) to catch all invalids
			if(expE.find(invExps.at(i)) != string::npos) { // If we find an invalid variable in an expression, we remove that expression
				cout << "Expression " << expressions.at(j).name << " relies on an invalid expression. It will not be evaluated." << endl;
				invExps.push_back(expressions.at(j).name); // To ensure the loop runs until all invalids are caught
				expressions.erase(expressions.begin() + j);
			}
		}
	}
}

int main(int argc, char* args[]) {

	string inputFile = ""; // This is a mandatory fill
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

	if(inputFile == "") {
		cout << "Please specify an input file!" << endl;
		return EXIT_SUCCESS;
	}

	vector<Expression> expressions; // A vector to store all input expressions
	vector<string> invExps; // Here we store invalid expression names, which we will use after parsing to remove invalids from the expressions list
	vector<string> expVariables; // Storing all variables occuring in all the expressions, also for sanity checking purposes	

	ifstream fin(inputFile);
	if (fin.is_open()) { // Parsing begins
		string line;
		int lineCount = 0;
		while(getline(fin, line)) {
			lineCount++;
			string expName = "";
			string expValue = "";
			int nameEnd = line.find('='); // An expression must have an equal sign
			int valueEnd = line.find(';'); // And a semicolon
			int leftParNum = count(line.begin(), line.end(), '('); // We'll also check whether the parentheses are properly paired
			int rightParNum = count(line.begin(), line.end(), ')');
			int equalNum = count(line.begin(), line.end(), '='); // One = and ; are enough
			int semicNum = count(line.begin(), line.end(), ';');

			if(valueEnd != string::npos && nameEnd != string::npos && leftParNum == rightParNum && line.at(line.length() - 2) == ' ' && line.back() == ';' && line.at(nameEnd-1) == ' ' && line.at(nameEnd+1) == ' ' && equalNum == 1 && semicNum == 1) { // Sanity checks
				for(int i = 0; i < nameEnd - 1; i++) { // Parses variable names
					expName += line.at(i);
				}

				for(int j = nameEnd + 2; j < valueEnd - 1; j++) { // Parses expression
					expValue += line.at(j);
					string expChar = "";
					expChar += line.at(j);
					if((isOperator(expChar) && line.at(j+1) == ')') && expValue.length() > 1) { // Further sanity check (to make sure we don't have an operator right before a closing bracket)
						cout << "Invalid syntax on line " << lineCount << " for expression " << expName << ". Expression will not be evaluated. Reason for error: operator right before closing parentheses." << endl;
						invExps.push_back(expName);
						expName = "";
						expValue = "";
					}
				}

				if(expName != "" && expValue != "") {
					string lastChar(1, expValue.back());
					if(isOperator(lastChar) || lastChar == "(") { // Sanity check for having an operator or a left parentheses at the end
						cout << "Invalid syntax on line " << lineCount << " for expression " << expName << ". Expression will not be evaluated. Reason for error: operator or left parentheses at the end." << endl;
						invExps.push_back(expName);
						expName = "";
						expValue = "";
					}
				}

				if(expName != "" && expValue != "") { // To weed out the previously detected invalid expressions
					vector<string> startingTokens; // Placeholder vectors for tokenised and postfix expressions
					vector<string> startingPostfix;
					Expression expr1 = {expName, expValue, startingTokens, startingPostfix, 0, false};
					expressions.push_back(expr1);
				}

				expName = "";
				expValue = "";
				if (fin.eof()) {
					break; // Prevents duplication of last line
				}
			}
			else { // We add the names of invalid expressions to a separate vector to cross-check with our expressions list later
				string invExpName = "";
				int i = 0;
				while(isalpha(line.at(i))) { // Quickly get the name of the expression
					invExpName += line.at(i);
					i++;
				}
				invExps.push_back(invExpName);

				// Error messages to inform the user what went wrong with each expression
				cout << "Invalid syntax on line " << lineCount << " for expression " << invExpName << ". Expression will not be evaluated. Reason for error: ";

				if(nameEnd == string::npos) { cout << "no equal sign." << endl; }
				else if(line.back() != ';' || valueEnd == string::npos) { cout << "no semicolon at end or at all." << endl; }
				else if(equalNum > 1) { cout << "multiple equal signs." << endl; }
				else if(semicNum > 1) { cout << "multiple semicolons." << endl; }
				else if(leftParNum != rightParNum) { cout << "unequal brackets." << endl; }
				else if(line.at(line.length() - 2) != ' ') { cout << "no space before the semicolon." << endl; }
				else if(line.at(nameEnd-1) != ' ' || line.at(nameEnd+1) != ' ') { cout << "no spaces between the equal sign." << endl; }
				else {cout << "generic error." << endl; }
			}
		}
		fin.close();
	}

	if(expressions.size() == 0) { // If we have no valid expressions, we just stop
		return EXIT_SUCCESS;
	}

	invalidFilter(expressions, invExps); // First run of the filter - takes care of invalid expressions discovered during parsing

	for(int l = 0; l < expressions.size(); l++) { // Tokenises every expression and converts them to postfix
 		Expression& exp = expressions.at(l);

 		exp.tokens = tokenise(exp.exp, exp.name, expressions, invExps);

 		exp.postfix = infix2postfix(exp.tokens);
 	}

 	invalidFilter(expressions, invExps); // Second run of the filter - takes care of invalids discovered during tokenisation

	int expCount = expressions.size(); // Count of remaining expressions to be evaluated
	int i = 0;
 	while(expCount > 0) { // Runs until all expressions are evaluated
		Expression& exp = expressions.at(i);
 		if(exp.isEval == false) {
  			evaluate(exp, expressions, expCount); // The actual evaluation; expCount is sent as a reference so it can be decreased within the function
 		}
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
		 		if(!isnan(exp.ans)) { // Only write if we got an actual numerical result
			 		ostringstream ansSS; // A stringstream is the best way to cast a float to string (to_string leaves too many zeros)
			 		ansSS << exp.ans;
			 		string ansString(ansSS.str());
			 		string line = exp.name + " = " + ansString +"\n"; // Proper formatting of line
			 		fout << line;
		 		}
		 	}
		 	fout.close();
	 	}
 	}
 	else { // If not, just print them on the terminal
	  	for(int i = 0; i < expressions.size(); i++) {
	 		Expression& exp = expressions.at(i);
	 		if(!isnan(exp.ans)) {
 		 		ostringstream ansSS;
		 		ansSS << exp.ans;
		 		string ansString(ansSS.str());
		 		cout << exp.name << " = " << exp.ans << endl;		
	 		}
 		}	
 	}

 	return EXIT_SUCCESS;
}