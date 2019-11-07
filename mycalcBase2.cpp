#include<iostream>
#include<string>
#include<cmath>
#include<stack>
#include<ctype.h>
#include<vector>

using namespace std;

bool isOperator(string ch); //Helper method which checks if the input char is an operator
int convertOpToInt (string); //Helper method which converts operators into int so their precdence can be compared
bool isleq(char, char); //Helper method which compare two operators  and return True if first operator has less or equal predence than the right operator
vector<string> infix2postfix(vector<string> infix); //Helper Method which converts an infix notaiton into a postfix notation (lab4)
vector<string> tokenise(string infix); 
float evaluate(vector<string> postfix)	//Method which will evaluate a PostfixExpression and return the result
{	
	stack <float> myStack;				//1. Create a stack of type float to store the operands
	//2.	Scan the POSTFIX expression from left to right for every element
	for(int i = 0; i < postfix.size(); i++) {
		//	a.	if the element is an operand push it to the stack

		if(isOperator(postfix.at(i)) == false) {

			//convert string into float element

			float elem = stof(postfix.at(i));
			myStack.push(elem);
		}
		//	b.	if the element is an operator pop 2 elements from the stack, apply the operator on it and push the result back to the stack
		if(isOperator(postfix.at(i))) {
			float a = myStack.top();
			myStack.pop();
			float b = myStack.top();
			myStack.pop();
			float result;
			//cout << postfix.at(i) << endl;
			if (postfix.at(i)=="+"){result = b + a; }
			else if (postfix.at(i)=="-"){result = b - a; }
			else if (postfix.at(i)=="*"){result = b * a; }
			else if (postfix.at(i)=="/"){result = b / a; }
			else if (postfix.at(i)=="%"){result= fmodf(b, a);}

			myStack.push(result);
		}


	}
	//3. return the value from the top of the stack (i.e. the final answer)
	return myStack.top();
}

int main()
{	
	/*vector<string> infix; 
	string input;

	while(true)
	{							//Infix expression
		cout<<"Enter an Infix Expression: ";
		while (cin >> input){
    		infix.push_back(input);

    		if(input =="exit") {
			break;
			}
			vector<string> token_infix= tokenise(infix); 
			vector <string> postfix = infix2postfix(token_infix);	//Postfix Expression
			float ans =evaluate(postfix);			//evaluate the postfix Expresion
			cout<<"The postfix form is: ";
			for(int i; i<postfix.size(); i++){cout<<postfix[i];}
			cout<<endl;
			cout<<"The answer is="<<ans<<endl;
    	}

		//cout<<infix<<"="<<ans<<endl;		//print the final answer
	} */

	while(true)
	{
		string infix;
		cout<<"Enter an Infix Expression: ";
		cin>>infix;
		if(infix=="exit") 
			break;
		vector <string> postfix = infix2postfix(tokenise(infix));
		float ans =evaluate(postfix);
		cout<<"The postfix form is: ";
		for(int i; i<postfix.size(); i++){cout<<postfix.at(i);}
			cout<<endl;
			cout<<"The answer is "<<ans<<endl;
	}
	return EXIT_SUCCESS;
}
//=====================================================================

bool isOperator(string ch)
{
	if( ch=="+" || ch=="-" || ch=="*" || ch=="/" || ch=="^" || ch=="%")
		return true;
	else
		return false;

}
//Helper mehtod which converts operators into int so their precdence can be compared
int convertOpToInt (string ch)
{
    if (ch=="+" || ch=="-") return 1;
    if (ch=="*" || ch=="/" || ch=="%") return 2;
    if (ch=="^") return 3;
    return 0;
}

//Helper method which compare two operators and return True if first operator
//has less or equal predence than the right operator
bool isleq(string opA, string opB)
{
	return (convertOpToInt(opA)<=convertOpToInt(opB));
}

// Helper Method which converts an Infix Notation to a Postfix notation
vector <string> infix2postfix(vector<string> infix)
{
	stack <string> mystack;
	vector <string> postfix;

	// 1.	Push “(“onto Stack, and add “)” to the end of INFIX.
	mystack.push("(");
	infix.push_back(")");

	while(!mystack.empty())
	{
		for(int j = 0; j < infix.size(); j++) 
		{
			if (isOperator(infix.at(j))== false and infix.at(j) != "(" and infix.at(j) != ")")
			{
				postfix.push_back(infix.at(j));
			}
			else if (infix.at(j)=="(") 
			{
				mystack.push("(");
			}
			else if (isOperator(infix.at(j)))
			{
				while(isOperator(mystack.top()) and isleq(infix.at(j), mystack.top()))
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






	/*// 2.	Scan INFIX from left to right and repeat Step 3 to 6 for each element of INFIX until the Stack is empty.
	for(int i = 0; i < infix.size(); i++) {
		// 3.	If an operand is encountered, add it to POSTFIX.
		if(isOperator(infix[i]) == false && infix[i] != "(" && infix[i] != ")") {
			postfix.push_back(infix[i]);
		}
		// 4.	If a left parenthesis is encountered, push it onto Stack.
		else if(infix[i] == "(") { mystack.push(infix[i]); }
		// 5.	If an operator is encountered, then: 
		else if(isOperator(infix[i]) == true && infix[i] != "(" && infix[i] != ")") {
			cout << "Top: " << mystack.top() << endl;
			cout << infix[i] << infix[i+1] << endl;

			// 	a.	Repeatedly pop from Stack and add to POSTFIX each operator (on the top of Stack) which has the same precedence as or higher precedence than operator.
			while(isleq(infix[i], mystack.top())) {
				if(infix[i] == "/" && infix[i+1] == "/") {
					cout << "in here1" << endl;
					postfix.push_back("//");
					mystack.pop();
				}
				else {
					postfix.push_back(mystack.top());
					mystack.pop();
				}
			}
			// 	b.	Add operator to Stack.
			if(infix[i] == "/" && infix[i+1] == "/") {
				cout << "in here" << endl;
				mystack.push("//");
			}
			else {
				mystack.push(infix[i]);
			}

		}
		// 6.	If a right parenthesis is encountered, then: 
		else if(infix[i] == ")") {

			// 	a.	Repeatedly pop from Stack and add to POSTFIX each operator (on the top of Stack) until a left parenthesis is encountered.
			while(mystack.top() != "(") {
				postfix.push_back(mystack.top());
				mystack.pop();
			}
			// 	b.	Remove the left Parenthesis.
			mystack.pop();
		}
	}
	*/

