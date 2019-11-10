// Preprocessor: tokenises the parsed input and converts it to postfix notation

#include<iostream>
#include<string>
#include<cmath>
#include<stack>
#include<ctype.h>
#include<vector>

#include "preprocessor.h"
#include "evaluator.h"

using namespace std;

bool isOperator(string ch)
{
	if( ch=="+" || ch=="-" || ch=="*" || ch=="/" || ch=="%" || ch=="//" || ch=="--" || ch=="++" || ch=="**")
		return true;
	else
		return false;

}

int convertOpToInt (string ch) // Method to help compare precedence
{
    if (ch=="+" || ch=="-") return 1;
    if (ch=="*" || ch=="/" || ch=="%" || ch=="//") return 2;
    if (ch=="--" || ch=="++" || ch=="**") return 3;
    return 0;
}

bool isleq(string opA, string opB) // Compares precedence and returns whether one is less or equal to the other
{
	return (convertOpToInt(opA)<=convertOpToInt(opB));
}

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
				exp.push_back(var); 
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
			//cout << i << " is a space." << endl;
			if(i < infix.length() - 1) { i++; continue; }
			else { break; }			
		}
		
		else if(elem == '(' || elem == ')') { // Parentheses
			//cout << i << " is a parentheses." << endl;
			string paren(1, elem);
			exp.push_back(paren);
			if(i < infix.length() - 1) { i++; continue; }
			else { break; }
		}

		else if(isalpha(elem)) { // Variables
			//cout << i << " is a variable." << endl;
			var += elem;
			if(isalpha(infix.at(i+1)) == 0) {
				exp.push_back(var);
				var = "";
			}
			if(i < infix.length() - 1) { i++; continue; }
			else { break; }
		}

		else if(isdigit(elem)) { // Numbers
			//cout << i << " is a number." << endl;
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
				//cout << i << " is a -- operator." << endl;
				exp.push_back("--");
				if(i < infix.length() - 2) { i = i + 2; continue; }
				else { break; }				
			}
			else if(i == 0 && elem == '-' && (isdigit(infix.at(i+1)) || isalpha(infix.at(i+1))) ) { // Edge case for a negative number at the start (avoids the i-1 checks)
				if(isdigit(infix.at(i+1))) {
					operand += elem;
					//cout << i << " is a negative number." << endl;
					if(i < infix.length() - 1) { i++; continue; }
					else { break; }
				}
				else {
					var += elem;
					//cout << i << " is a negative variable." << endl;
					if(i < infix.length() - 1) { i++; continue; }
					else { break; }
				}				
			}
			else if(i != 0 && elem == '-' && isdigit(infix.at(i-1)) == 0 && isalpha(infix.at(i-1)) == 0 && (isdigit(infix.at(i+1)) || isalpha(infix.at(i+1))) ) { // When - is a negative marker instead of an operator
				if(isdigit(infix.at(i+1))) {
					operand += elem;
					//cout << i << " is a negative number." << endl;
					if(i < infix.length() - 1) { i++; continue; }
					else { break; }
				}
				else {
					var += elem;
					//cout << i << " is a negative variable." << endl;
					if(i < infix.length() - 1) { i++; continue; }
					else { break; }
				}
			}
			else { // All other operators
				op += elem;
				//cout << i << " is an operator." << endl;
/*				if(op.length() > 2) {
					string unaryOp = op.substr(op.length() - 2);
					exp.push_back(unaryOp)
					exp.push_back(op);
					op = "";					
				}*/
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


vector <string> infix2postfix(vector<string> infix) // Converts a tokenised infix expression to postfix
{
	stack <string> mystack;
	vector <string> postfix;

	mystack.push("(");
	infix.push_back(")");

	while(!mystack.empty())
	{
		for(int j = 0; j < infix.size(); j++) 
		{
			if (isOperator(infix.at(j))== false && infix.at(j) != "(" && infix.at(j) != ")")
			{
				postfix.push_back(infix.at(j));
			}
			else if (infix.at(j)=="(") 
			{
				mystack.push("(");
			}
			else if (isOperator(infix.at(j)))
			{
				while(isOperator(mystack.top()) && isleq(infix.at(j), mystack.top()))
				{
					postfix.push_back(mystack.top());
					mystack.pop();
				}

				mystack.push(infix.at(j));
			}
			else if (infix.at(j)==")")
			{
				while(mystack.top() != "(")
				{
					postfix.push_back(mystack.top());
					mystack.pop();
				}
				mystack.pop();
			}
		}
		return postfix;
	}
}