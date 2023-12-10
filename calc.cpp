#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <cmath>
#include <sstream>

using namespace std;

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

double applyOp(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return b != 0 ? a / b : throw runtime_error("Division by zero");
    case '^': return pow(a, b);
    }
    return 0;
}

double applyFunction(const string& func, double arg) {
    if (func == "sin") return sin(arg);
    if (func == "cos") return cos(arg);
    if (func == "tan") return tan(arg);
    if (func == "log") return log(arg);
    if (func == "exp") return exp(arg);
    if (func == "sqrt") return sqrt(arg);
    throw runtime_error("Unknown function: " + func);
}

double evaluate(string expression) {
    stack<double> values;
    stack<char> ops;

    for (int i = 0; i < expression.length(); i++) {
        if (isspace(expression[i])) continue;

        if (isdigit(expression[i]) || expression[i] == '.') {
            stringstream ss;
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                ss << expression[i++];
            }
            double val;
            ss >> val;
            values.push(val);
            i--;
        } else if (isalpha(expression[i])) {
            stringstream ss;
            while (i < expression.length() && isalpha(expression[i])) {
                ss << expression[i++];
            }
            string func = ss.str();
            i++; // Skip '('

            stringstream arg;
            while (i < expression.length() && expression[i] != ')') {
                arg << expression[i++];
            }
            i++; // Skip ')'

            double val = evaluate(arg.str());
            values.push(applyFunction(func, val));
        } else if (expression[i] == '(') {
            ops.push(expression[i]);
        } else if (expression[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                double val2 = values.top();
                values.pop();

                double val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOp(val1, val2, op));
            }
            ops.pop();
        } else {
            while (!ops.empty() && precedence(ops.top()) >= precedence(expression[i])) {
                double val2 = values.top();
                values.pop();

                double val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOp(val1, val2, op));
            }
            ops.push(expression[i]);
        }
    }

    while (!ops.empty()) {
        double val2 = values.top();
        values.pop();

        double val1 = values.top();
        values.pop();

        char op = ops.top();
        ops.pop();

        values.push(applyOp(val1, val2, op));
    }

    return values.top();
}

int main() {
    string expression;
    cout << "Enter an expression (or type 'exit' to quit): ";

    while (true) {
        getline(cin, expression);
        if (expression == "exit") break;

        try {
            double result = evaluate(expression);
            cout << "Result: " << result << endl;
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }

        cout << "Enter an expression: ";
    }

    return 0;
}
