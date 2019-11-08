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
