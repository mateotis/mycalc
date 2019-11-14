#include<iostream>
#include<string>
#include<cmath>
#include<stack>
#include<ctype.h>
#include<vector>

using namespace std;

struct Expression {
	string name; // Variable name
	string exp; // Parsed expression
	vector<string> tokens; // Tokenised expression
	vector<string> postfix; // Postfix expression
	float ans; // Result
	bool isEval; // Evaluation status
};

bool isOperator(string);
int convertOpToInt (string);
bool isleq(char, char);
vector<string> tokenise(string, string, vector<Expression>&, vector<string>&);
vector<string> infix2postfix(vector<string>);