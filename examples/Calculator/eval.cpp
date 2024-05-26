#include <iostream>
#include <sstream>
#include <stack>
#include <queue>
#include <string>
#include <cctype>
#include <cmath>
#include <stdexcept>

static bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

static bool isParenthesis(char c) {
    return c == '(' || c == ')';
}

static bool isNumber(const std::string &s) {
    std::istringstream iss(s);
    double d;
    char c;
    return iss >> d && !(iss >> c);
}

static int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

static double applyOperator(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) throw std::runtime_error("Division by zero");
            return a / b;
        case '^': return pow(a, b);
        default: throw std::runtime_error("Invalid operator");
    }
}

static std::queue<std::string> infixToPostfix(const std::string &expression) {
    std::stack<char> operators;
    std::queue<std::string> output;
    std::istringstream tokens(expression);
    std::string token;

    while (tokens >> token) {
        if (isNumber(token)) {
            output.push(token);
        } else if (isOperator(token[0])) {
            while (!operators.empty() && precedence(operators.top()) >= precedence(token[0])) {
                output.push(std::string(1, operators.top()));
                operators.pop();
            }
            operators.push(token[0]);
        } else if (token[0] == '(') {
            operators.push('(');
        } else if (token[0] == ')') {
            while (!operators.empty() && operators.top() != '(') {
                output.push(std::string(1, operators.top()));
                operators.pop();
            }
            operators.pop();
        } else {
            throw std::runtime_error("Invalid character in expression");
        }
    }

    while (!operators.empty()) {
        output.push(std::string(1, operators.top()));
        operators.pop();
    }

    return output;
}

static double evaluatePostfix(std::queue<std::string> &postfix) {
    std::stack<double> values;

    while (!postfix.empty()) {
        std::string token = postfix.front();
        postfix.pop();

        if (isNumber(token)) {
            values.push(std::stod(token));
        } else if (isOperator(token[0])) {
            double b = values.top(); values.pop();
            double a = values.top(); values.pop();
            double result = applyOperator(a, b, token[0]);
            values.push(result);
        }
    }

    return values.top();
}

static std::string preprocessExpression(const std::string &expression) {
    std::ostringstream output;
    bool lastCharWasOperator = true;

    for (size_t i = 0; i < expression.length(); ++i) {
        char c = expression[i];
        if (isspace(c)) continue;

        if (isdigit(c) || c == '.') {
            output << c;
            lastCharWasOperator = false;
        } else if (isOperator(c) || isParenthesis(c)) {
            if (lastCharWasOperator && c == '-') {
                output << "0 "; // Handle unary minus
            }
            output << ' ' << c << ' ';
            lastCharWasOperator = true;
        } else {
            throw std::runtime_error("Invalid character in expression");
        }
    }

    return output.str();
}

double evaluate(const std::string &expression) {
    std::string preprocessed = preprocessExpression(expression);
    std::queue<std::string> postfix = infixToPostfix(preprocessed);
    return evaluatePostfix(postfix);
}
