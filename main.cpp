#include <iostream>
#include <string>
#include <vector>
#include "rpn_calculator.h"
#include"rpn_calculator.cpp"

void printWelcome() {
    std::cout << "==========================================" << std::endl;
    std::cout << "        C++ RPN Calculator" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Basic Operations: +, -, *, /" << std::endl;
    std::cout << "Advanced Operations: sqrt, ^, sin, cos, tan, fib, pascal" << std::endl;
    std::cout << "Commands: c(clear), s(stack), h(history), q(quit)" << std::endl;
    std::cout << "Example: '5 5 + 3 *' or '10 fib'" << std::endl;
    std::cout << "==========================================" << std::endl;
}

void printHelp() {
    std::cout << "\nAvailable Commands:" << std::endl;
    std::cout << "  <expression>  - Enter RPN expression" << std::endl;
    std::cout << "  c         - Clear stack" << std::endl;
    std::cout << "  s         - Display stack" << std::endl;
    std::cout << "  h         - Show calculation history" << std::endl;
    std::cout << "  clearh    - Clear history records" << std::endl;
    std::cout << "  batch     - Batch calculation mode" << std::endl;
    std::cout << "  help      - Show help information" << std::endl;
    std::cout << "  q         - Exit program" << std::endl;
}

void batchMode(RPNCalculator& calc) {
    std::cout << "\nBatch Calculation Mode (press Enter to finish):" << std::endl;
    std::vector<std::string> expressions;
    std::string line;

    int count = 1;
    while (true) {
        std::cout << "Expression " << count << ": ";
        std::getline(std::cin, line);

        if (line.empty()) {
            break;
        }

        expressions.push_back(line);
        count++;
    }

    if (!expressions.empty()) {
        std::cout << "\nCalculation Results:" << std::endl;
        std::vector<double> results = calc.evaluateBatch(expressions);

        for (size_t i = 0; i < results.size(); i++) {
            std::cout << i + 1 << ". " << expressions[i] << " = ";
            if (std::isnan(results[i])) {
                std::cout << "Error" << std::endl;
            }
            else {
                std::cout << results[i] << std::endl;
            }
        }
    }
}

int main() {
    RPNCalculator calculator;
    std::string input;

    printWelcome();

    while (true) {
        std::cout << "\n> ";
        std::getline(std::cin, input);

        if (input == "q" || input == "quit") {
            std::cout << "Goodbye!" << std::endl;
            break;
        }
        else if (input == "c" || input == "clear") {
            calculator.clear();
            std::cout << "Stack cleared" << std::endl;
        }
        else if (input == "s" || input == "stack") {
            calculator.displayStack();
        }
        else if (input == "h" || input == "history") {
            calculator.displayHistory();
        }
        else if (input == "clearh") {
            calculator.clearHistory();
            std::cout << "History records cleared" << std::endl;
        }
        else if (input == "batch") {
            batchMode(calculator);
        }
        else if (input == "help") {
            printHelp();
        }
        else if (!input.empty()) {
            try {
                double result = calculator.evaluateExpression(input);
                std::cout << "Result: " << result << std::endl;
            }
            catch (const RPNException& e) {
                std::cerr << e.what() << std::endl;
            }
            catch (const std::exception& e) {
                std::cerr << "Unknown error: " << e.what() << std::endl;
            }
        }
    }

    return 0;
}