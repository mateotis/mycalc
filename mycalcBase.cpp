#include<iostream>
#include<string>
#include<cmath>
#include<stack>
#include<ctype.h>

using namespace std;

bool isOperator(char); //Helper method which checks if the input char is an operator
int convertOpToInt (char); //Helper mehtod which converts operators into int so their precdence can be compared
bool isleq(char, char); //Helper method which compare two operators  and return True if first operator has less or equal predence than the right operator
string infix2postfix(string);   //Helper Method which converts an infix notaiton into a postfix notation (lab4)
float evaluate(string postfix)	//Method which will evaluate a PostfixExpression and return the result
{	
	stack <float> myStack;				//1. Create a stack of type float to store the operands
	//2.	Scan the POSTFIX expression from left to right for every element
	for(int i = 0; i < postfix.length(); i++) {
		//	a.	if the element is an operand push it to the stack
		if(isOperator(postfix.at(i)) == false) {
			float elem = postfix.at(i) - 48;
			myStack.push(elem);
		}
		
		//	b.	if the element is an operator pop 2 elements from the stack, apply the operator on it and push the result back to the stack
		if(isOperator(postfix.at(i))) {
			float a = myStack.top();
			myStack.pop();
			float b = myStack.top();
			myStack.pop();
			float result;
			cout << postfix.at(i) << endl;
			switch(postfix.at(i)) {
				case '+': result = b + a;
					break;
				case '-': result = b - a;
					break;
				case '*': result = b * a;
					break;					
				case '/': result = b / a;
					break;
				case '%':
					result = fmodf(b, a);
			}

			myStack.push(result);
		}

	}
	//3. return the value from the top of the stack (i.e. the final answer)
	return myStack.top();
}

int main()
{
	while(true)
	{
		string infix;  						//Infix expression
		cout<<"Enter an Infix Expression: ";
		cin>>infix;
	
		if(infix=="exit") 
			break;

		string postfix = infix2postfix(infix);	//Postfix Expression
		float ans=evaluate(postfix);			//evaluate the postfix Expresion
		cout<<"The postfix form is: "<<postfix<<endl;
		cout<<infix<<"="<<ans<<endl;		//print the final answer
	}

	return EXIT_SUCCESS;
}
//=====================================================================
bool isOperator(char ch)
{
	if( ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch=='^' || ch=='%')
		return true;
	else
		return false;

}
//Helper mehtod which converts operators into int so their precdence can be compared
int convertOpToInt (char ch)
{
    if (ch=='+' || ch=='-') return 1;
    if (ch=='*' || ch=='/' || ch=='%') return 2;
    if (ch=='^') return 3;
    return 0;
}

//Helper method which compare two operators and return True if first operator
//has less or equal predence than the right operator
bool isleq(char opA, char opB)
{
	return (convertOpToInt(opA)<=convertOpToInt(opB));
}

// Helper Method which converts an Infix Notaiton to a Postfix notation
string infix2postfix(string infix)
{
	stack <char> mystack;
	string postfix="";
	// 1.	Push “(“onto Stack, and add “)” to the end of INFIX.
	mystack.push('(');
	infix = infix + ')';

	// 2.	Scan INFIX from left to right and repeat Step 3 to 6 for each element of INFIX until the Stack is empty.
	for(int i = 0; i < infix.length(); i++) {

		// 3.	If an operand is encountered, add it to POSTFIX.
		if(isOperator(infix.at(i)) == false && infix.at(i) != '(' && infix.at(i) != ')') {
			postfix += infix.at(i);
		}
		// 4.	If a left parenthesis is encountered, push it onto Stack.
		if(infix.at(i) == '(') { mystack.push(infix.at(i)); }
		// 5.	If an operator is encountered, then: 
		if(isOperator(infix.at(i)) == true && infix.at(i) != '(' && infix.at(i) != ')') {
			cout << "Top: " << mystack.top() << endl;
			cout << infix.at(i) << infix.at(i+1) << endl;

			// 	a.	Repeatedly pop from Stack and add to POSTFIX each operator (on the top of Stack) which has the same precedence as or higher precedence than operator.
			while(isleq(infix.at(i), mystack.top())) {
				if(infix.at(i) == '/' && infix.at(i+1) == '/') {
					cout << "in here1" << endl;
					postfix += "//";
					mystack.pop();
				}
				else {
					postfix += mystack.top();
					mystack.pop();
				}

			}
			// 	b.	Add operator to Stack.
			if(infix.at(i) == '/' && infix.at(i+1) == '/') {
				cout << "in here" << endl;
				mystack.push('//');
			}
			else {
				mystack.push(infix.at(i));
			}

		}
		// 6.	If a right parenthesis is encountered, then: 
		if(infix.at(i) == ')') {

			// 	a.	Repeatedly pop from Stack and add to POSTFIX each operator (on the top of Stack) until a left parenthesis is encountered.
			while(mystack.top() != '(') {
				postfix += mystack.top();
				mystack.pop();
			}
			// 	b.	Remove the left Parenthesis.
			mystack.pop();
		}
	}

	return postfix;
}