#include<iostream>
#include<string>
#include<cmath>
#include<stack>
#include<ctype.h>
#include<vector>

using namespace std;


vector<string> tokenise(string infix) {
	vector<string> exp;

	string operand = "";
	string op = ""; // "operator" is not a valid variable name, sadly

	int i = 0;
	while(i < infix.length() - 1) {
		while((infix.at(i) == '(') || (infix.at(i) == ')')) {
			string paren(1, infix.at(i)); // Since parantheses can only be one character each, we can safely convert them to string
			exp.push_back(paren);
			cout << paren << " is parantheses." << endl;
			if(i < infix.length() - 1) { // Boundary check
				i++;
			}
			else {
				break;
			}
		}

		while(isdigit(infix.at(i)) == true) { // All the digits that are next to each other make one number
			operand += infix.at(i); // Operands and operators are tokenised into strings
			if(i < infix.length() - 1) { // Boundary check
				i++;
			}
			else {
				break;
			}
		}
		if(operand != "") { // To avoid pushing empty values into the vector
			cout << operand << " is a number." << endl;
			exp.push_back(operand);
		}
		operand = "";

		while(isdigit(infix.at(i)) == false && infix.at(i) != '(' && infix.at(i) != ')') { // Covers multi-character operators
			if(infix.at(i) == '-' && infix.at(i+1) == '-') { // Edge case for --
				cout << "-- is an operator." << endl;
				exp.push_back("--");
				i += 2;
			}
			else if(infix.at(i) == '-' && i == 0 && isdigit(infix.at(i+1)) == true) { // Edge case where there is a negative at the start of the expression
				operand += infix.at(i);
				i++;
				while(isdigit(infix.at(i)) == true) {
					//cout << "in here" << endl;
					operand += infix.at(i); // Operands and operators are tokenised into strings
					if(i < infix.length() - 1) { // Boundary check
						i++;
					}
					else {
						break;
					}					
				}
				if(operand != "") {
					cout << operand << " is a number." << endl;
					exp.push_back(operand);
					operand = "";
				}

			}
			else if(infix.at(i) == '-' && isdigit(infix.at(i+1)) == true && isdigit(infix.at(i-1)) == false) {  // This handles - as a negative marker which should be part of an operand
				operand += infix.at(i);
				i++;
				while(isdigit(infix.at(i)) == true) {
					//cout << "in here" << endl;
					operand += infix.at(i); // Operands and operators are tokenised into strings
					if(i < infix.length() - 1) { // Boundary check
						i++;
					}
					else {
						break;
					}					
				}
				if(operand != "") {
					cout << operand << " is a number." << endl;
					exp.push_back(operand);
					operand = "";
				}

			}
			else {
				op += infix.at(i);
				if(i < infix.length() - 1) {
					i++;
				}
				else {
					break;
				}
			}
		}
		
		if(op != "") {
			cout << op << " is an operator." << endl;
			exp.push_back(op);			
		}
		op = "";
	}

	return exp;
}

int main() {

	string infix = "--120//((--343)+(546-781*(**5)))";
	vector<string> expression = tokenise(infix);

	for(int i = 0; i < expression.size(); i++) {
		cout << expression.at(i);
	}
	cout << endl;
}