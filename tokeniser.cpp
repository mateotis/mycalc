#include<iostream>
#include<string>
#include<cmath>
#include<stack>
#include<ctype.h>
#include<vector>

using namespace std;

void tokenise(string infix) {
	vector<string> operands;
	vector<string> operators;

	string operand = "";
	string op = ""; // "operator" is not a valid variable name, sadly

	int i = 0;
	while(i < infix.length() - 1) {
		while(isdigit(infix.at(i)) == true) { // All the digits that are next to each other make one number
			operand += infix.at(i); // Operands and operators are tokenised into strings
			if(i < infix.length() - 1) { // Boundary check
				i++;
			}
			else {
				break;
			}
		}
		cout << operand << " is a number." << endl;
		operands.push_back(operand);
		operand = "";

		while(isdigit(infix.at(i)) == false) { // Covers multi-character operators
			op += infix.at(i);
			if(i < infix.length() - 1) {
				i++;
			}
			else {
				break;
			}
		}
		cout << op << " is an operator." << endl;
		operators.push_back(op);
		op = "";
	}

	operands.erase(operands.begin()); // Hack but it gets rid of the weird empty elements that make it into these vectors
	operators.pop_back();

	for(int i = 0; i < operands.size(); i++) {
		cout << operands.at(i) << " ";
	}
	cout << endl;
	for(int i = 0; i < operators.size(); i++) {
		cout << operators.at(i) << " ";
	}
}

int main() {
	//char ops = {'+', '-', '*', '/', '%', '//'}

	string infix = "--12//34+56-78";
	tokenise(infix);
}