// Evaluator: evaluates the preprocessed postfix expressions and returns their results
#include<iostream>
#include<string>
#include<cmath>
#include<stack>
#include<ctype.h>
#include<vector>
#include "preprocessor.h"
#include "evaluator.h"
using namespace std;

float evaluate(vector<string> postfix)	//Method which will evaluate a PostfixExpression and return the result
{	
	stack <float> myStack;				//1. Create a stack of type float to store the operands
	//2.	Scan the POSTFIX expression from left to right for every element
	for(int i = 0; i < postfix.size(); i++) {
		//	a.	if the element is an operand push it to the stack

/*		if(isOperator(postfix.at(i)) == false) {

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
*/
		if(isOperator(postfix.at(i)) == false) {
			//convert string into float element
			float elem = stof(postfix.at(i));
			myStack.push(elem);
		}
		//	b.	if the element is an operator pop 2 elements from the stack, apply the operator on it and push the result back to the stack
		
		if (postfix.at(i) =="-")
		{

			if (myStack.size()==2) //if it is a binary minus
			{
				float a = myStack.top();
				myStack.pop();
				float b = myStack.top();
				myStack.pop();
				float result;
				result = b - a;
				myStack.push(result);
			}
				
			else if (myStack.size()==1)   //if it is a unary minus
			{
				float a = myStack.top();
				myStack.pop();
				float result;
				result=-a;
				myStack.push(result);
			}
		}

		else if(isOperator(postfix.at(i)) and isUnary(postfix.at(i))) {
			float a = myStack.top();
			myStack.pop();
			float b = myStack.top();
			myStack.pop();
			float result;
			//cout << postfix.at(i) << endl;
			if (postfix.at(i)=="+"){result = b + a; }
			//else if (postfix.at(i)=="-"){result = b - a; }
			else if (postfix.at(i)=="*"){result = b * a; }
			else if (postfix.at(i)=="/"){result = b / a; }
			else if (postfix.at(i)=="%"){result= fmodf(b, a);}
			else if (postfix.at(i)=="//"){result = (b - fmodf(b, a))/a; } 
			myStack.push(result);
		}
			
		//if it is a unary operator
		else if (isOperator(postfix.at(i)) and !isUnary(postfix.at(i)))
		{
			float a = myStack.top();
			myStack.pop();
			float result;
			if (postfix.at(i)=="**"){result = a * a; }
			else if (postfix.at(i)=="--"){result = a-1; }
			else if (postfix.at(i)=="++"){result = a+1; }
			myStack.push(result);
		}
	}
	//3. return the value from the top of the stack (i.e. the final answer)
	return myStack.top();
}

bool isUnary(string op)
{
	if (op=="+" || op=="*" || op=="/" || op=="^" || op=="%" || op=="//")
		return true;
	else if (op=="--"|| op== "**" || op== "++")
		return false;


}

//check if stack has two or one element, if it has two, then binary, if one, then unary
