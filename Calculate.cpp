#include "Calculate.h"

uint Calculate::priority(const std::string &symbol){
    if(symbol == "(") return 1;
    if(symbol == "+" || symbol == "-") return 2;
    if(symbol == "*" || symbol == "/") return 3;
}

/*!
 Разбивает арифметическое выражение на токены
*/
void Calculate::get_token(const std::string &expr){
    if(expr.empty()){
        std::cerr<<"expression not found";
    }

    std::string last_token;
    uint n_brackets = 0;

    size_t i = 0;
    while(i < expr.length()){
        switch (expr[i]){
            case '(':
                if(i == 0 || storage.back() == "(" || last_token == "sign"){
                    n_brackets++;
                    storage.push_back("(");
                }else{
                    std::cerr<<"incorrect bracket location";
                }
                break;
            case ')':
                if(storage.back() == ")" || last_token == "digit"){
                    n_brackets--;
                    storage.push_back(")");
                }
                break;
            case '-':
                if(storage.back() == "(" || storage.empty()){
                    last_token = "unary_minus";
                }else if(storage.back() != ")" || last_token == "digit"){
                    storage.push_back("-");
                    last_token = "sign";

                }else{
                    std::cout << expr[i] << " incorrect character positioning\n";
                }
                break;
            case '*':
            case '/':
            case '+':
                if(storage.back() != ")" || last_token == "digit"){
                    std::string s;
                    s = expr[i] + '\0';
                    storage.push_back(s);
                    last_token = "sign";
                }
                break;
            default:
                std::string num;

                if(storage.empty() || storage.back() == "(" || last_token == "sign"){
                    size_t k = i;
                    while(isdigit(expr[k]) || (expr[k] == '.' && std::isdigit(num.back()))){
                        num += expr[k];
                        ++k;
                    }

                    if(std::isalpha(expr[i]) && !std::isalpha(num.back())){
                        num = expr[i];
                    }

                    if(last_token == "unary_minus"){
                        num = "-" + num;
                    }

                    if(!num.empty()){
                        storage.push_back(std::move(num));
                        i = k - 1;
                        k = 0;
                        last_token = "digit";
                    }
                }
                break;
        }
        ++i;
    }

    if(n_brackets != 0){
        std::cout<<"invalid num of brackets"<<std::endl;
        storage.clear();
    }

//    for (size_t i = 0 ; i < storage.size(); i++){
//        std::cout << "[" << storage[i] << "] ";
//    }
//    std::cout<<"\n";
}
/*!
  Преобразуется инфиксную запись в постфиксную
*/
void Calculate::postfix()
{
    for(size_t i = 0; i < storage.size(); i++){
            if(storage.at(i) == "("){
                stack_.push(storage[i]);
            }else if(storage.at(i) == ")"){
                while( stack_.top() != "(" && !stack_.empty()){
                    queue_.push(stack_.top());
                    stack_.pop();
                }
                if(!stack_.empty()){
                    stack_.pop();
                }else{
                    std::cerr<<"no matching '('";
                }
            }else if(is_operation(storage.at(i))){
                if(stack_.empty()){
                    stack_.push(storage[i]);
                }else{
                    if(priority(storage[i]) <= priority(stack_.top())){
                        while(!stack_.empty() && priority(storage[i]) <= priority(stack_.top())){
                            queue_.push(stack_.top());
                            stack_.pop();
                        }
                    }
                    stack_.push(storage[i]);
                }
            }else{
                queue_.push(storage[i]);
            }
        }

    while(!stack_.empty()){
        queue_.push(stack_.top());
        stack_.pop();
    }
}

/*!
 Непосредственное выполнение операций
*/
double Calculate::do_operation(std::string &sign){
    double l = numbers.top();
    numbers.pop();
    double r = numbers.top();
    numbers.pop();

    if(sign == "+"){
        numbers.push(l + r);
    }else if (sign == "-"){
        numbers.push(r - l);
    }else if (sign == "*"){
        numbers.push(l * r);
    }else if (sign == "/"){
        numbers.push(r / l);
    }else{
        std::cout<<"incorrect symbol\n";
    }
}
/*!
 Подсчитывает значение арифметического выпажения в постфиксной записи
*/
double Calculate::calculate(const std::string &argv){
    get_token(argv);
    postfix();

    std::string sym;
    while(!queue_.empty()){
        sym = queue_.front();
        if(is_operation(sym)){
            do_operation(sym);
        }else{
            double number = std::stod(sym.c_str());
            numbers.push(number);
        }
        queue_.pop();
    }
        //std::cout<<numbers.top();
    return numbers.top();
}
