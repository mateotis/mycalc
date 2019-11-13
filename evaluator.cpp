// Evaluator: evaluates the preprocessed postfix expressions and returns their results

#include<iostream>
#include<string>
#include<cmath>
#include<stack>
#include<ctype.h>
#include<vector>
#include<math.h> // For NaN and isnan()

#include "preprocessor.h"
#include "evaluator.h"

using namespace std;

void evaluate(Expression& exp, vector<Expression>& expressions, int& expCount)
{	
	stack <float> operandStack; // Where we store the operands

	for(int i = 0; i < exp.postfix.size(); i++) {
		string elem = exp.postfix.at(i);

		if(isOperator(elem) == false) { // Variables and operands
			if(isalpha(elem.at(0)) || (elem.at(0) == '-' && isalpha(elem.at(1))) ) { // We only need to check the first character: if that's a letter, we have a variable OR if we have a negative variable, we check the second character
				for(int j = 0; j < expressions.size(); j++) {
					if(elem.at(0) == '-') { // Special case for a negative variable to ensure accurate results fetch
						string elem2 = elem.substr(1, elem.length() - 1); // Cut the minus sign for the search
						if(expressions.at(j).name == elem2) {
							if(expressions.at(j).isEval == true) { // If the variable we need is already calculated, we can use it
								float fElem = expressions.at(j).ans;
								fElem = fElem * -1.0; // Applying the negation
								operandStack.push(fElem);
							}
							else { // If it's not, we can't evaluate this expression for now
								return;
							}
						}
					}
					else if(expressions.at(j).name == elem) { // For regular variables
						if(expressions.at(j).isEval == true) { // If the variable we need is already calculated, we can use it
							float fElem = expressions.at(j).ans;
							operandStack.push(fElem);
						}
						else { // If it's not, we can't evaluate this expression for now
							return;
						}
					}
				}
			}
			else { // Regular operands
				float fElem = stof(elem); // Very delicate method; we do all those sanity checks to protect it :)
				operandStack.push(fElem);
			}
		}

		if (elem =="-") // The minus sign is, as always, special: it can be a binary or a unary operator, which we determine through stack size
		{

			if (operandStack.size() > 1) // If it is a binary minus, we pop two and subtract
			{
				float a = operandStack.top();
				operandStack.pop();
				float b = operandStack.top();
				operandStack.pop();
				float result;
				result = b - a;
				operandStack.push(result);
			}
				
			else if (operandStack.size()==1)   // If it is a unary minus, we pop one and negate
			{
				float a = operandStack.top();
				operandStack.pop();
				float result;
				result=-a;
				operandStack.push(result);
			}
		}

		else if(isOperator(elem) and isBinary(elem)) { // Same behaviour for other binary operators
			float a = operandStack.top();
			operandStack.pop();
			float b = operandStack.top();
			operandStack.pop();
			float result;
			if (elem=="+") { result = b + a; }
			else if (elem=="*") { result = b * a; }
			else if (elem=="/") { result = b / a; }
			else if (elem=="%") { result= fmodf(b, a);}
			else if (elem=="//") { result = (b - fmodf(b, a))/a; } 
			operandStack.push(result);
		}
			
		else if (isOperator(elem) and !isBinary(elem)) // And other unary operators
		{
			float a = operandStack.top();
			operandStack.pop();
			float result;
			if (elem=="**") { result = a * a; }
			else if (elem=="--") { result = a - 1; }
			else if (elem=="++") { result = a + 1; }
			operandStack.push(result);
		}
	}

	// We have reached the end of evaluation

	if(operandStack.size() > 1) { // If the stack still has more than one element in it, then there must have been some operator mishap at input which led to a weird result - we warn the user accordingly and make a note not to display these values
		cout << "WARNING: The evaluator could not fully calculate expression " << exp.name << " because of an operator mismatch. Its result, and the results of all other expressions that rely on it, are undefined and won't be displayed." << endl;
		exp.isEval = true;
		exp.ans = NAN; // Very handy - any operation involving a NaN also evaluates to NaN, which takes care of all other expressions that might use the NaN expression as a variable
		expCount--;
		return;
	}
	exp.isEval = true;
	exp.ans = operandStack.top(); // If there were no stack problems, we return the last remaining element as the result
	expCount--;
	return;
}

bool isBinary(string op) // Checks if an operator is binary
{
	if (op=="+" || op=="*" || op=="/" || op=="^" || op=="%" || op=="//")
		return true;
	else if (op=="--"|| op== "**" || op== "++")
		return false;


}