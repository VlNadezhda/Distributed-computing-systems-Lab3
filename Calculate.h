#ifndef INC_3_CALCULATE_H
#define INC_3_CALCULATE_H

#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include<cctype>
#include<vector>
#include<cstring>

class Calculate
{
private:
    std::vector<std::string> storage;
    std::queue <std::string> queue_;
    std::stack <std::string> stack_;
    std::stack<double> numbers;

    bool is_operation(std::string c){
        return c == "+" || c == "-" || c == "*" || c == "/";
    }
    void postfix();
    void get_token(const std::string &expr);
    uint priority(const std::string &symbol);
    double do_operation(std::string &sign);

public:
    Calculate(){};
    double calculate(const std::string &argv);
};

#endif //INC_3_CALCULATE_H
