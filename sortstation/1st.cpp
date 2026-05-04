#include <iostream>
#include <stack>
#include <string>
#include <cctype>
using namespace std;
using Valuetype = int;
int operators(char op){
	if (op == '+' || op == '-'){
		return 1;
	}
	if (op == '*'||op == '/'){
		return 2;
	}
	return 0;
}
Valuetype math(Valuetype a, Valuetype b, char op){
	switch (op){
		case '+': return a+b;
		case '-': return a-b;
		case '*': return a*b;
		case '/': return a/b;	
	}
	return 0;
}
Valuetype evaluate(string tokens, string& postfix){
	stack<Valuetype> numbers;
	stack<char> ops;
	for (int i = 0; i<tokens.length(); i++){
		if (tokens[i] == ' ') continue;
		else if (tokens[i] == '('){
			ops.push(tokens[i]);
		}
		else if (isdigit(tokens[i])){
			Valuetype val = 0;
			while (i<tokens.length() && isdigit(tokens[i])){
				val = val*10+(tokens[i]-'0');
				i++;
			}
			i--;
			numbers.push(val);
			postfix += to_string(val) + " ";
		}
		else if (tokens[i] == ')'){
			while (!ops.empty() && ops.top() != '('){
				Valuetype val2 = numbers.top();
				numbers.pop();
				Valuetype val1 = numbers.top();
				numbers.pop();
				char op = ops.top();
				ops.pop();
				postfix += op;
				postfix += " ";
				numbers.push(math(val1, val2, op));
			}
			if (!ops.empty()) ops.pop();
		}
		else{
			while(!ops.empty() && operators(ops.top())>=operators(tokens[i])){
				Valuetype val2 = numbers.top();
				numbers.pop();
				Valuetype val1 = numbers.top();
				numbers.pop();
				char op = ops.top();
				ops.pop();
				postfix += op;
				postfix += " ";
				numbers.push(math(val1, val2, op));
			}
			ops.push(tokens[i]);
		}
	}
	while(!ops.empty()){
		Valuetype val2 = numbers.top();
		numbers.pop();
		Valuetype val1 = numbers.top();
		numbers.pop();
		char op = ops.top();
		ops.pop();
		postfix += op;
		postfix += " ";	
		numbers.push(math(val1, val2, op));
	}
	return numbers.top();
}

int main() {
	string expr = "100 * (5 + 9) / 7";
	string postfix = "";
	Valuetype result = evaluate(expr, postfix);
	cout << "Infix:   " << expr << "\n";
	cout << "Postfix: " << postfix << "\n";
	cout << "Result:  " << result << "\n";
	return 0;
}
