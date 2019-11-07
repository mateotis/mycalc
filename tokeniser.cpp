#include<iostream>
#include<string>
#include<cmath>
#include<stack>
#include<ctype.h>
#include<vector>
#include<fstream>

using namespace std;


vector<string> tokenise(string infix) {
	vector<string> exp;

	string operand = "";
	string op = ""; // "operator" is not a valid variable name, sadly
	string var = "";

	int i = 0;

	while(i < infix.length()) {
		char elem = infix.at(i);

		if(infix.length() == 1) { // If the whole expression is one character, just add it and be done with it
			string elemS(1, elem);
			exp.push_back(elemS);
			return exp;
		}

		if(i == infix.length() - 1) { // If we're on the last character, then we add it to the last unfinished token before returning
			if(op != "") {
				op += elem;
				exp.push_back(op);
				return exp; 
			}
			else if(operand != "") {
				operand += elem;
				exp.push_back(operand); 
				return exp; 
			}
			else if(var != "") {
				var += elem;
				exp.push_back(operand); 
				return exp; 
			}
			else { // If it's none of these, it's a parentheses
				string elemS(1, elem);
				exp.push_back(elemS);
				return exp;
			}
		}

		// Otherwise, we head into tokenisation...
		if(isspace(elem)) { // Filtering out whitespace
			cout << i << " is a space." << endl;
			if(i < infix.length() - 1) { i++; continue; }
			else { break; }			
		}
		
		else if(elem == '(' || elem == ')') { // Parentheses
			cout << i << " is a parentheses." << endl;
			string paren(1, elem);
			exp.push_back(paren);
			if(i < infix.length() - 1) { i++; continue; }
			else { break; }
		}

		else if(isalpha(elem)) { // Variables
			cout << i << " is a variable." << endl;
			var += elem;
			if(isalpha(infix.at(i+1)) == 0) {
				exp.push_back(var);
				var = "";
			}
			if(i < infix.length() - 1) { i++; continue; }
			else { break; }
		}

		else if(isdigit(elem)) { // Numbers
			cout << i << " is a number." << endl;
			operand += elem;
			if(isdigit(infix.at(i+1)) == 0) {
				exp.push_back(operand);
				operand = "";
			}
			if(i < infix.length() - 1) { i++; continue; }
			else { break; }			
		}

		else { // Operators of all kinds
			if(elem == '-' && infix.at(i+1) == '-') { // Edge case for --
				cout << i << " is a -- operator." << endl;
				exp.push_back("--");
				if(i < infix.length() - 2) { i = i + 2; continue; }
				else { break; }				
			}
			else if(i == 0 && elem == '-' && (isdigit(infix.at(i+1)) || isalpha(infix.at(i+1))) ) { // Edge case for a negative number at the start (avoids the i-1 checks)
				if(isdigit(infix.at(i+1))) {
					operand += elem;
					cout << i << " is a negative number." << endl;
					if(i < infix.length() - 1) { i++; continue; }
					else { break; }
				}
				else {
					var += elem;
					cout << i << " is a negative variable." << endl;
					if(i < infix.length() - 1) { i++; continue; }
					else { break; }
				}				
			}
			else if(i != 0 && elem == '-' && isdigit(infix.at(i-1)) == 0 && isalpha(infix.at(i-1)) == 0 && (isdigit(infix.at(i+1)) || isalpha(infix.at(i+1))) ) { // When - is a negative marker instead of an operator
				if(isdigit(infix.at(i+1))) {
					operand += elem;
					cout << i << " is a negative number." << endl;
					if(i < infix.length() - 1) { i++; continue; }
					else { break; }
				}
				else {
					var += elem;
					cout << i << " is a negative variable." << endl;
					if(i < infix.length() - 1) { i++; continue; }
					else { break; }
				}
			}
			else { // All other operators
				op += elem;
				cout << i << " is an operator." << endl;
				if(isalpha(infix.at(i+1)) || isdigit(infix.at(i+1)) || isspace(infix.at(i+1)) || infix.at(i+1) == '(' || infix.at(i+1) == ')') {
					exp.push_back(op);
					op = "";
				}
				if(i < infix.length() - 1) { i++; continue; }
				else { break; }
			}
		}

	}

	return exp;
}

struct Expression {
	string name;
	string exp;
	float ans;
	bool isEval;
	//Expression() : ans(0), isEval(false) {}
};

int main() {

	vector<Expression> expressions;
	ifstream fin("file1.txt");

	if (fin.is_open()) {
		string line;
		int lineCount = 0;
		while(getline(fin, line)) {
			lineCount++;
			//cout << line << endl;
			string expName = "";
			string expValue = "";
			int nameEnd = line.find("=");
			int valueEnd = line.find(";");

			if(valueEnd != string::npos && line.at(line.length() - 2) == ' ' && line.at(nameEnd-1) == ' ' && line.at(nameEnd+1) == ' ') { // Sanity checks to ensure proper semicolon and equal sign placement
				// TODO: Add check for equal number of left and right parantheses
				//cout << nameEnd << endl;
				for(int i = 0; i < nameEnd - 1; i++) { // Parses variable names
					expName += line.at(i);
				}
				//cout << expName << endl;

				for(int j = nameEnd + 2; j < valueEnd - 1; j++) { // Parses expression
					expValue += line.at(j);
				}
				//cout << expValue << endl;
				Expression expr1 = {expName, expValue, 0, false};
				expressions.push_back(expr1);
				//cout << expr1.name << ", " << expr1.exp << ", " << expr1.ans << ", " << expr1.isEval << endl;

				expName = "";
				expValue = "";
				if (fin.eof()) {
					break; // Prevents duplication of last line
				}
			}
			else {
				cout << "Invalid syntax on line " << lineCount << ". Expression will not be evaluated." << endl;
			}


		}
	}
	fin.close();

	for(int i = 0; i < expressions.size(); i++) {
		cout << expressions[i].exp << endl;
	}

	//string infix = "1+2+3+4";
	//vector<string> expression = tokenise(infix);



 	for(int i = 0; i < expressions.size(); i++) {
 		Expression expToTokenise = expressions.at(i);
 		vector<string> tokenisedExp = tokenise(expToTokenise.exp);

 		cout << "EXPRESSION IS: ";
	 	for(int i = 0; i < tokenisedExp.size(); i++) {
			cout << tokenisedExp.at(i) << ',';
		}
		cout << endl;
 	}

/*	for(int i = 0; i < expression.size(); i++) {
		cout << expression.at(i);
	}*/
	cout << endl;
}