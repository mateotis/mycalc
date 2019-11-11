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

void evaluate(Expression& exp, vector<Expression>& expressions, int& expCount)	//Method which will evaluate a PostfixExpression and return the result
{	
/*	for(int i = 0; i < expressions.size(); i++) {
		cout << expressions.at(i).name << " eval status is " << expressions.at(i).isEval << endl;
	}*/
	stack <float> myStack;				//1. Create a stack of type float to store the operands

	//2.	Scan the POSTFIX expression from left to right for every element
	for(int i = 0; i < exp.postfix.size(); i++) {
		string elem = exp.postfix.at(i);
		//	a.	if the element is an operand push it to the stack

		if(isOperator(elem) == false) { 
			if(isalpha(elem.at(0))) { // We only need to check the first character: if that's a letter, we have a variable
				for(int j = 0; j < expressions.size(); j++) {
					//cout << expressions.at(j).name << endl;
					if(expressions.at(j).name == elem) {
						if(expressions.at(j).isEval == true) { // If the variable we need is already calculated, we can use it
							float fElem = expressions.at(j).ans;
							myStack.push(fElem);
							//cout << fElem << " is the value of variable " << expressions.at(j).name << endl;
						}
						else { // If it's not, we can't evaluate this expression for now
							return;
						}
					}
				}
			}
			else {
			//convert string into float element
				cout << "Element to convert: " << elem << endl;
				float fElem = stof(elem);
				myStack.push(fElem);
			}
		}
		//	b.	if the element is an operator pop 2 elements from the stack, apply the operator on it and push the result back to the stack
		
		if (elem =="-")
		{

			if (myStack.size() > 1) //if it is a binary minus
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

		else if(isOperator(elem) and isBinary(elem)) {
			float a = myStack.top();
			myStack.pop();
			float b = myStack.top();
			myStack.pop();
			float result;
			//cout << postfix.at(i) << endl;
			if (elem=="+"){result = b + a; }
			//else if (postfix.at(i)=="-"){result = b - a; }
			else if (elem=="*"){result = b * a; }
			else if (elem=="/"){result = b / a; }
			else if (elem=="%"){result= fmodf(b, a);}
			else if (elem=="//"){result = (b - fmodf(b, a))/a; } 
			myStack.push(result);
		}
			
		//if it is a unary operator
		else if (isOperator(elem) and !isBinary(elem))
		{
			float a = myStack.top();
			myStack.pop();
			float result;
			if (elem=="**"){result = a * a; }
			else if (elem=="--"){result = a-1; }
			else if (elem=="++"){result = a+1; }
			myStack.push(result);
		}
	}
	//3. return the value from the top of the stack (i.e. the final answer)
	exp.isEval = true;
	exp.ans = myStack.top();
	expCount--;
	return;
}

bool isBinary(string op)
{
	if (op=="+" || op=="*" || op=="/" || op=="^" || op=="%" || op=="//")
		return true;
	else if (op=="--"|| op== "**" || op== "++")
		return false;


}

//check if stack has two or one element, if it has two, then binary, if one, then unary
