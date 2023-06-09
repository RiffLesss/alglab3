﻿#include <iostream>
#include <string>
#include <sstream>
#include "alglab1.h"
#include "stack.h"


class PostfixCalculator {
public:
    PostfixCalculator() {
        stack = new Stack();
    }

    ~PostfixCalculator() {
        delete stack;
    }

    int calculate(const std::string& input) {
        std::istringstream iss(input);
        std::string token;
        while (iss >> token) {
            try {
                int value = std::stoi(token);
                stack->push(value);
            }
            catch (std::exception&) {
                if (is_operator(token)) {
                    stack->push(calculate_operator(token));
                }
                else {
                    std::cout << "Invalid input" << std::endl;
                    throw std::exception();
                }
            }
        }

        if (stack->empty()) {
            std::cout << "Invalid input" << std::endl;
            return 0;
        }

        int result = stack->pop();

        if (!stack->empty()) {
            std::cout << "Invalid input" << std::endl;
            return 0;
        }

        return result;
    }
private:
    Stack* stack;

    bool is_operator(const std::string& op) {
        return op == "+" || op == "-" || op == "*" || op == "/";
    }

    int calculate_operator(const std::string& op) {
        int a = stack->pop();
        int b = stack->pop();
        if (op == "+") {
            return b + a;
        }
        if (op == "-") {
            return b - a;
        }
        if (op == "*") {
            return b * a;
        }
        if (op == "/") {
            if (a == 0) {
                std::cout << "Error: division by zero" << std::endl;
                throw std::exception();
            }
            return b / a;
        }
        return 0;
    }


};

// Функция, которая определяет приоритет оператора
int priority(char op) {
    if (op == '+' || op == '-') {
        return 1;
    }
    else if (op == '*' || op == '/') {
        return 2;
    }
    else {
        return 0;
    }
}

// Функция, которая переводит инфиксное выражение в постфиксное
string infixToPostfix(string expression) {
    stack<char> s;
    string postfix = "";

    for (int i = 0; i < expression.length(); i++) {
        char c = expression[i];

        // Если символ является операндом, добавляем его в постфиксную строку
        if (isalnum(c)) {
            postfix += c;
        }
        // Если символ является открывающей скобкой, помещаем его в стек
        else if (c == '(') {
            s.push(c);
        }
        // Если символ является закрывающей скобкой, выталкиваем все символы из стека 
        // и добавляем их в постфиксную строку, пока не достигнем открывающей скобки
        else if (c == ')') {
            while (!s.empty() && s.top() != '(') {
                postfix += s.top();
                s.pop();
            }
            s.pop();
        }
        // Если символ является оператором, выталкиваем из стека все операторы с 
        // более высоким приоритетом и добавляем их в постфиксную строку
        else {
            while (!s.empty() && priority(c) <= priority(s.top())) {
                postfix += s.top();
                s.pop();
            }
            s.push(c);
        }
    }

    // Выталкиваем оставшиеся элементы из стека и добавляем их в постфиксную строку
    while (!s.empty()) {
        postfix += s.top();
        s.pop();
    }

    return postfix;
}

int main() {
    PostfixCalculator calculator;
    std::string expression;
    while (true) {
        std::cout << "Please enter an expression in postfix notation: ";
        getline(std::cin, expression);
        if (expression == "q") {
            break;
        }
        string postfix = infixToPostfix(expression);
        std::cout << calculator.calculate(expression) << std::endl;
    }
    return 0;
}