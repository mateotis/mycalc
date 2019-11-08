#include<iostream>
#include<string>
#include<cmath>
#include<stack>
#include<ctype.h>
#include<vector>

using namespace std;

bool isOperator(string);
int convertOpToInt (string);
bool isleq(char, char);
vector<string> tokenise(string); 
vector<string> infix2postfix(vector<string>);