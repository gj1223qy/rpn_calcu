#pragma once
#ifndef RPN_CALCULATOR_H
#define RPN_CALCULATOR_H

#include <vector>
#include <string>
#include <stack>
#include <map>
#include <functional>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <sstream>

class RPNCalculator {
private:
    std::stack<double> stack;
    std::vector<std::string> history;

    // Mathematical operation container
    std::map<std::string, std::function<void()>> operations;

    void initializeOperations();

    // Basic operations
    void add();
    void subtract();
    void multiply();
    void divide();

    // Advanced operations
    void squareRoot();
    void power();
    void sine();
    void cosine();
    void tangent();
    void fibonacci();
    void pascalTriangle();

    // Utility functions
    bool isNumber(const std::string& str);
    void ensureStackSize(int required);
    double calculateFibonacci(int n);
    double calculatePascal(int row, int col);

public:
    RPNCalculator();

    // Stack operations
    void push(double value);
    double pop();
    void clear();
    void displayStack() const;

    // Calculation operations
    void calculate(const std::string& operation);
    double evaluateExpression(const std::string& expression);

    // Batch calculation
    std::vector<double> evaluateBatch(const std::vector<std::string>& expressions);

    // History management
    void addToHistory(const std::string& expression, double result);
    void displayHistory() const;
    void clearHistory();

    // Status queries
    bool isEmpty() const;
    size_t size() const;
};

// Exception classes
class RPNException : public std::runtime_error {
public:
    explicit RPNException(const std::string& message) : std::runtime_error(message) {}
};

class DivisionByZeroException : public RPNException {
public:
    DivisionByZeroException() : RPNException("Error: Division by zero") {}
};

class StackUnderflowException : public RPNException {
public:
    StackUnderflowException() : RPNException("Error: Stack underflow") {}
};

class InvalidInputException : public RPNException {
public:
    InvalidInputException() : RPNException("Error: Invalid input format") {}
};

#endif