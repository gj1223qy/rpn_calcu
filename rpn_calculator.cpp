#include "rpn_calculator.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>

#define M_PI 3.14159265358979323846

RPNCalculator::RPNCalculator() {
    initializeOperations();
}

void RPNCalculator::initializeOperations() {
    // Basic arithmetic operations
    operations["+"] = [this]() { add(); };
    operations["-"] = [this]() { subtract(); };
    operations["*"] = [this]() { multiply(); };
    operations["/"] = [this]() { divide(); };

    // Advanced math operations
    operations["sqrt"] = [this]() { squareRoot(); };
    operations["^"] = [this]() { power(); };
    operations["sin"] = [this]() { sine(); };
    operations["cos"] = [this]() { cosine(); };
    operations["tan"] = [this]() { tangent(); };

    // Special operations
    operations["fib"] = [this]() { fibonacci(); };
    operations["pascal"] = [this]() { pascalTriangle(); };
}

// Stack operation implementations
void RPNCalculator::push(double value) {
    stack.push(value);
}

double RPNCalculator::pop() {
    if (stack.empty()) {
        throw StackUnderflowException();
    }
    double value = stack.top();
    stack.pop();
    return value;
}

void RPNCalculator::clear() {
    while (!stack.empty()) {
        stack.pop();
    }
}

void RPNCalculator::displayStack() const {
    if (stack.empty()) {
        std::cout << "Stack is empty" << std::endl;
        return;
    }

    std::stack<double> temp = stack;
    std::vector<double> elements;

    while (!temp.empty()) {
        elements.push_back(temp.top());
        temp.pop();
    }

    std::cout << "Current stack (top to bottom): ";
    for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

// Basic operation implementations
void RPNCalculator::add() {
    ensureStackSize(2);
    double b = pop();
    double a = pop();
    push(a + b);
}

void RPNCalculator::subtract() {
    ensureStackSize(2);
    double b = pop();
    double a = pop();
    push(a - b);
}

void RPNCalculator::multiply() {
    ensureStackSize(2);
    double b = pop();
    double a = pop();
    push(a * b);
}

void RPNCalculator::divide() {
    ensureStackSize(2);
    double b = pop();
    double a = pop();

    if (b == 0) {
        push(a); // Restore dividend
        push(b); // Restore divisor
        throw DivisionByZeroException();
    }

    push(a / b);
}

// Advanced operation implementations
void RPNCalculator::squareRoot() {
    ensureStackSize(1);
    double a = pop();
    if (a < 0) {
        push(a);
        throw RPNException("Error: Cannot calculate square root of negative number");
    }
    push(std::sqrt(a));
}

void RPNCalculator::power() {
    ensureStackSize(2);
    double exponent = pop();
    double base = pop();
    push(std::pow(base, exponent));
}

void RPNCalculator::sine() {
    ensureStackSize(1);
    double a = pop();
    push(std::sin(a * M_PI / 180.0)); // Convert to radians
}

void RPNCalculator::cosine() {
    ensureStackSize(1);
    double a = pop();
    push(std::cos(a * M_PI / 180.0));
}

void RPNCalculator::tangent() {
    ensureStackSize(1);
    double a = pop();
    double radians = a * M_PI / 180.0;
    if (std::cos(radians) == 0) {
        push(a);
        throw RPNException("Error: Tangent function undefined at angle " + std::to_string(a));
    }
    push(std::tan(radians));
}

void RPNCalculator::fibonacci() {
    ensureStackSize(1);
    double n = pop();
    if (n < 0 || n != static_cast<int>(n)) {
        push(n);
        throw RPNException("Error: Fibonacci sequence requires non-negative integer");
    }
    push(calculateFibonacci(static_cast<int>(n)));
}

void RPNCalculator::pascalTriangle() {
    ensureStackSize(2);
    double col = pop();
    double row = pop();

    if (row < 0 || col < 0 || col > row ||
        row != static_cast<int>(row) || col != static_cast<int>(col)) {
        push(row);
        push(col);
        throw RPNException("Error: Invalid Pascal's triangle parameters");
    }

    push(calculatePascal(static_cast<int>(row), static_cast<int>(col)));
}

// Utility function implementations
bool RPNCalculator::isNumber(const std::string& str) {
    std::istringstream iss(str);
    double d;
    iss >> d;
    return !iss.fail() && iss.eof();
}

void RPNCalculator::ensureStackSize(int required) {
    if (stack.size() < required) {
        throw StackUnderflowException();
    }
}

double RPNCalculator::calculateFibonacci(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    double a = 0, b = 1, c;
    for (int i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

double RPNCalculator::calculatePascal(int row, int col) {
    if (col == 0 || col == row) return 1;

    double result = 1;
    for (int i = 1; i <= col; i++) {
        result = result * (row - i + 1) / i;
    }
    return result;
}

// Calculation operation implementations
void RPNCalculator::calculate(const std::string& operation) {
    auto it = operations.find(operation);
    if (it != operations.end()) {
        it->second();
    }
    else {
        throw InvalidInputException();
    }
}

double RPNCalculator::evaluateExpression(const std::string& expression) {
    std::istringstream iss(expression);
    std::string token;

    while (iss >> token) {
        if (isNumber(token)) {
            push(std::stod(token));
        }
        else {
            calculate(token);
        }
    }

    if (stack.size() != 1) {
        throw RPNException("Error: Incomplete expression or format error");
    }

    double result = pop();
    addToHistory(expression, result);
    return result;
}

// Batch calculation implementation
std::vector<double> RPNCalculator::evaluateBatch(const std::vector<std::string>& expressions) {
    std::vector<double> results;

    for (const auto& expr : expressions) {
        try {
            clear(); // Clear stack for each expression
            double result = evaluateExpression(expr);
            results.push_back(result);
        }
        catch (const RPNException& e) {
            results.push_back(std::nan("")); // Use NaN to represent error
            std::cerr << "Expression '" << expr << "' calculation error: " << e.what() << std::endl;
        }
    }

    return results;
}

// History implementation
void RPNCalculator::addToHistory(const std::string& expression, double result) {
    std::string entry = expression + " = " + std::to_string(result);
    history.push_back(entry);
}

void RPNCalculator::displayHistory() const {
    if (history.empty()) {
        std::cout << "History is empty" << std::endl;
        return;
    }

    std::cout << "Calculation History:" << std::endl;
    for (size_t i = 0; i < history.size(); i++) {
        std::cout << i + 1 << ". " << history[i] << std::endl;
    }
}

void RPNCalculator::clearHistory() {
    history.clear();
}

// Status query implementations
bool RPNCalculator::isEmpty() const {
    return stack.empty();
}

size_t RPNCalculator::size() const {
    return stack.size();
}