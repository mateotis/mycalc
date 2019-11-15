// Preprocessor: tokenises the parsed input and converts it to postfix notation

#include<iostream>
#include<string>
#include<cmath>
#include<stack>
#include<ctype.h>
#include<vector>

#include "preprocessor.h"

using namespace std;

bool isOperator(string ch) // Called during infix-to-postfix, hence it takes a string
{
	if( ch=="+" || ch=="-" || ch=="*" || ch=="/" || ch=="%" || ch=="//" || ch=="--" || ch=="++" || ch=="**")
		return true;
	else
		return false;

}

bool isParentheses(char ch) { // Called only during tokenising, hence it takes a char
	if(ch == ')' || ch == '(')
		return true;
	else
		return false;
}

int convertOpToInt (string ch) // Helps compare precedence
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

vector<string> tokenise(string infix, string name, vector<Expression>& expressions, vector<string>& invExps) {
	vector<string> exp;

	string operand = ""; // Numbers
	string op = ""; // "operator" is not a valid variable name, sadly
	string var = ""; // Variable names

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
				bool varIsPresent = false;
				for(int i = 0; i < expressions.size(); i ++) { // Wr're looking whether the variable we just found is the name of any expression
					if(expressions.at(i).name == var) { // If it is, then it's cool
						varIsPresent = true;
						break;
					}
				}
				if(!varIsPresent) { invExps.push_back(name); } // If it's not, then this very expression is invalid - we'll take care of it soon
				if(var == name) { invExps.push_back(name); } // Checks for self-assignment, which would lead to an infinite loop
				exp.push_back(var); // But for now, we let the program continue
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
			if(i < infix.length() - 1) { i++; continue; } // Boundary checks after every comparison ensures a smooth character-by-character movement to the end of the expression 
			else { break; }			
		}
		
		else if(isParentheses(elem)) { // Parentheses
			string paren(1, elem);
			exp.push_back(paren);
			if(i < infix.length() - 1) { i++; continue; }
			else { break; }
		}

		else if(isalpha(elem)) { // Variables
			var += elem;
			if(isalpha(infix.at(i+1)) == 0) {
				bool varIsPresent = false;
				for(int i = 0; i < expressions.size(); i ++) {
					if(expressions.at(i).name == var) {
						varIsPresent = true;
						break;
					}
				}
				if(var == name) { invExps.push_back(name); }
				if(!varIsPresent) { invExps.push_back(name); }
				exp.push_back(var);
				var = "";
			}
			if(i < infix.length() - 1) { i++; continue; }
			else { break; }
		}

		else if(isdigit(elem)) { // Numbers
			operand += elem;
			if(isdigit(infix.at(i+1)) == 0) {
				exp.push_back(operand);
				operand = "";
			}
			if(i < infix.length() - 1) { i++; continue; }
			else { break; }			
		}

		else { // Operators of all kinds
			if(elem == '-' && infix.at(i+1) == '-' && infix.at(i+2) == '-') { // Edge case for --- (converts it to - and --)
				exp.push_back("-");
				exp.push_back("--");
				if(i < infix.length() - 3) { i = i + 3; continue; }
				else { break; }	
			}
			else if(elem == '-' && infix.at(i+1) == '-' && infix.at(i+2) != '-') { // Edge case for --
				exp.push_back("--");
				if(i < infix.length() - 2) { i = i + 2; continue; }
				else { break; }				
			}
			else if(i == 0 && elem == '-' && (isdigit(infix.at(i+1)) || isalpha(infix.at(i+1))) ) { // Edge case for a negative number at the start (avoids the i-1 checks)
				if(isdigit(infix.at(i+1))) {
					operand += elem;
					if(i < infix.length() - 1) { i++; continue; }
					else { break; }
				}
				else {
					var += elem;
					if(i < infix.length() - 1) { i++; continue; }
					else { break; }
				}
				
			}
			else if(i != 0 && elem == '-' && infix.at(i-1) != ')' && isdigit(infix.at(i-1)) == 0 && isalpha(infix.at(i-1)) == 0 && (isdigit(infix.at(i+1)) || isalpha(infix.at(i+1))) && infix.at(i-1) != '-') { // When - is a negative marker instead of an operator
				if(isdigit(infix.at(i+1))) { // The - is treated as part of the number/variable instead of a separate operator
					operand += elem;
					if(i < infix.length() - 1) { i++; continue; }
					else { break; }
				}
				else {
					var += elem;
					if(i < infix.length() - 1) { i++; continue; }
					else { break; }
				}
			}
			else { // All other operators
				op += elem;
				if(op.length() > 2) { // In case of three or more operators with the same character being next to each other: it splits them into unary and binary and adds them separately
					string unaryOp = op.substr(op.length() - 2, 2); // The unary operator (which always has two characters) comes second
					string binaryOp = op.substr(0, op.length() - 2); // The binary operator (whatever is left after slicing off the unary) comes first
					exp.push_back(binaryOp);
					exp.push_back(unaryOp);
					op = "";
					if(i < infix.length() - 1) { i++; continue; }
				}

				if(isalpha(infix.at(i+1)) || isdigit(infix.at(i+1)) || isspace(infix.at(i+1)) || isParentheses(infix.at(i+1)) || infix.at(i+1) != elem) { // These checks all mark the end of an operator
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


vector <string> infix2postfix(vector<string> infix) // Converts a tokenised infix expression to postfix - working with a vector ensures that each part of the expression is treated separately and prioritised correctly
{
	stack <string> pfStack;
	vector <string> postfix;

	pfStack.push("(");
	infix.push_back(")");

	while(!pfStack.empty())
	{
		for(int j = 0; j < infix.size(); j++) 
		{
			if (isOperator(infix.at(j))== false && infix.at(j) != "(" && infix.at(j) != ")") // If we have an operand, we add it to the postfix vector
			{
				postfix.push_back(infix.at(j));
			}
			else if (infix.at(j)=="(") // Same for the left bracket
			{
				pfStack.push("(");
			}
			else if (isOperator(infix.at(j))) // For operators, we pop and push based on precedence
			{
				while(isOperator(pfStack.top()) && isleq(infix.at(j), pfStack.top()))
				{
					postfix.push_back(pfStack.top());
					pfStack.pop();
				}

				pfStack.push(infix.at(j));
			}
			else if (infix.at(j)==")")
			{
				while(pfStack.top() != "(") // With a left bracket, we pop until we find its right pair
				{
					postfix.push_back(pfStack.top());
					pfStack.pop();
				}
				pfStack.pop();
			}
		}
		return postfix;
	}
}