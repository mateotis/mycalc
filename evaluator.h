#include<iostream>
#include<string>
#include<cmath>
#include<stack>
#include<ctype.h>
#include<vector>


struct Expression {
	string name; // Variable name
	string exp; // Parsed expression
	vector<string> tokens; // Tokenised expression
	vector<string> postfix; // Postfix expression
	float ans;
	bool isEval;
	//Expression() : ans(0), isEval(false) {}
};

void evaluate(Expression&, vector<Expression>&, int&);
bool isBinary(string op);