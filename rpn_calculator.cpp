#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <cmath>
#include <map>
#include <functional>
#include <iomanip>
#include <algorithm>

class RPNCalculator {
private:
    std::stack<double> stack;
    std::vector<std::string> history;
    
    // 检查字符串是否为数字
    bool isNumber(const std::string& str) {
        if (str.empty()) return false;
        
        // 处理负数
        size_t start = 0;
        if (str[0] == '-') {
            if (str.length() == 1) return false;
            start = 1;
        }
        
        bool hasDecimal = false;
        for (size_t i = start; i < str.length(); i++) {
            if (str[i] == '.') {
                if (hasDecimal) return false; // 多个小数点
                hasDecimal = true;
            } else if (!std::isdigit(str[i])) {
                return false;
            }
        }
        return true;
    }
    
    // 检查是否至少有两个操作数
    bool hasEnoughOperands(int required = 2) {
        if (stack.size() < required) {
            std::cout << "错误: 需要至少 " << required << " 个操作数" << std::endl;
            return false;
        }
        return true;
    }

public:
    RPNCalculator() = default;
    
    // 处理单个token
    bool processToken(const std::string& token) {
        if (isNumber(token)) {
            stack.push(std::stod(token));
            return true;
        }
        
        // 基本运算符
        if (token == "+") {
            if (!hasEnoughOperands()) return false;
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(a + b);
        }
        else if (token == "-") {
            if (!hasEnoughOperands()) return false;
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(a - b);
        }
        else if (token == "*") {
            if (!hasEnoughOperands()) return false;
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(a * b);
        }
        else if (token == "/") {
            if (!hasEnoughOperands()) return false;
            double b = stack.top(); stack.pop();
            if (b == 0) {
                std::cout << "错误: 除零错误" << std::endl;
                return false;
            }
            double a = stack.top(); stack.pop();
            stack.push(a / b);
        }
        // 高级数学运算
        else if (token == "sqrt") {
            if (!hasEnoughOperands(1)) return false;
            double a = stack.top(); stack.pop();
            if (a < 0) {
                std::cout << "错误: 负数不能开平方根" << std::endl;
                return false;
            }
            stack.push(std::sqrt(a));
        }
        else if (token == "pow") {
            if (!hasEnoughOperands()) return false;
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(std::pow(a, b));
        }
        else if (token == "sin") {
            if (!hasEnoughOperands(1)) return false;
            double a = stack.top(); stack.pop();
            stack.push(std::sin(a * M_PI / 180.0)); // 转换为弧度
        }
        else if (token == "cos") {
            if (!hasEnoughOperands(1)) return false;
            double a = stack.top(); stack.pop();
            stack.push(std::cos(a * M_PI / 180.0));
        }
        else if (token == "tan") {
            if (!hasEnoughOperands(1)) return false;
            double a = stack.top(); stack.pop();
            stack.push(std::tan(a * M_PI / 180.0));
        }
        // 斐波那契数列
        else if (token == "fib") {
            if (!hasEnoughOperands(1)) return false;
            int n = static_cast<int>(stack.top()); stack.pop();
            if (n < 0) {
                std::cout << "错误: 斐波那契数列参数必须为非负数" << std::endl;
                return false;
            }
            stack.push(fibonacci(n));
        }
        // 栈操作命令
        else if (token == "clear") {
            clear();
            std::cout << "栈已清空" << std::endl;
        }
        else if (token == "show") {
            displayStack();
        }
        else if (token == "pop") {
            if (stack.empty()) {
                std::cout << "错误: 栈为空" << std::endl;
                return false;
            }
            stack.pop();
        }
        else if (token == "dup") {
            if (stack.empty()) {
                std::cout << "错误: 栈为空" << std::endl;
                return false;
            }
            stack.push(stack.top());
        }
        else if (token == "swap") {
            if (!hasEnoughOperands()) return false;
            double a = stack.top(); stack.pop();
            double b = stack.top(); stack.pop();
            stack.push(a);
            stack.push(b);
        }
        else {
            std::cout << "错误: 未知的操作符或命令 '" << token << "'" << std::endl;
            return false;
        }
        return true;
    }
    
    // 计算斐波那契数列
    double fibonacci(int n) {
        if (n == 0) return 0;
        if (n == 1) return 1;
        
        double a = 0, b = 1;
        for (int i = 2; i <= n; i++) {
            double temp = a + b;
            a = b;
            b = temp;
        }
        return b;
    }
    
    // 清空栈
    void clear() {
        while (!stack.empty()) {
            stack.pop();
        }
    }
    
    // 显示当前栈内容
    void displayStack() const {
        if (stack.empty()) {
            std::cout << "栈为空" << std::endl;
            return;
        }
        
        std::stack<double> temp = stack;
        std::vector<double> elements;
        
        while (!temp.empty()) {
            elements.push_back(temp.top());
            temp.pop();
        }
        
        std::cout << "栈内容 (从顶到底): ";
        for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
    
    // 获取栈顶元素
    double getResult() const {
        if (stack.empty()) {
            throw std::runtime_error("栈为空");
        }
        return stack.top();
    }
    
    // 获取栈大小
    size_t getStackSize() const {
        return stack.size();
    }
    
    // 添加历史记录
    void addToHistory(const std::string& expression, double result) {
        history.push_back(expression + " = " + std::to_string(result));
    }
    
    // 显示历史记录
    void showHistory() const {
        if (history.empty()) {
            std::cout << "没有历史记录" << std::endl;
            return;
        }
        
        std::cout << "计算历史:" << std::endl;
        for (size_t i = 0; i < history.size(); i++) {
            std::cout << i + 1 << ": " << history[i] << std::endl;
        }
    }
    
    // 批量计算表达式
    void batchCalculate(const std::vector<std::string>& expressions) {
        std::cout << "开始批量计算 " << expressions.size() << " 个表达式..." << std::endl;
        
        for (size_t i = 0; i < expressions.size(); i++) {
            std::cout << "\n表达式 " << (i + 1) << ": " << expressions[i] << std::endl;
            
            // 保存当前栈状态
            std::stack<double> backup = stack;
            
            if (evaluateExpression(expressions[i])) {
                std::cout << "结果: " << getResult() << std::endl;
            } else {
                std::cout << "计算失败" << std::endl;
            }
            
            // 恢复栈状态
            stack = backup;
        }
    }
    
    // 计算RPN表达式
    bool evaluateExpression(const std::string& expression) {
        std::istringstream iss(expression);
        std::string token;
        bool success = true;
        
        while (iss >> token && success) {
            success = processToken(token);
        }
        
        return success && !stack.empty();
    }
};

// 显示帮助信息
void showHelp() {
    std::cout << "\n=== RPN计算器使用说明 ===" << std::endl;
    std::cout << "基本用法: 输入数字和操作符，用空格分隔" << std::endl;
    std::cout << "示例: 5 2 + 3 *  => (5+2)*3 = 21" << std::endl;
    std::cout << "\n基本操作符:" << std::endl;
    std::cout << "  +, -, *, /  : 四则运算" << std::endl;
    std::cout << "  sqrt        : 平方根" << std::endl;
    std::cout << "  pow         : 幂运算" << std::endl;
    std::cout << "  sin, cos, tan: 三角函数(角度制)" << std::endl;
    std::cout << "  fib         : 斐波那契数列" << std::endl;
    std::cout << "\n栈操作命令:" << std::endl;
    std::cout << "  show        : 显示栈内容" << std::endl;
    std::cout << "  clear       : 清空栈" << std::endl;
    std::cout << "  pop         : 弹出栈顶元素" << std::endl;
    std::cout << "  dup         : 复制栈顶元素" << std::endl;
    std::cout << "  swap        : 交换栈顶两个元素" << std::endl;
    std::cout << "  history     : 显示计算历史" << std::endl;
    std::cout << "  batch       : 进入批量计算模式" << std::endl;
    std::cout << "  help        : 显示此帮助" << std::endl;
    std::cout << "  quit        : 退出程序" << std::endl;
    std::cout << "========================\n" << std::endl;
}

int main() {
    RPNCalculator calculator;
    std::string input;
    
    std::cout << "欢迎使用RPN计算器！输入 'help' 查看使用说明" << std::endl;
    
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        
        if (input == "quit" || input == "exit") {
            break;
        }
        else if (input == "help") {
            showHelp();
        }
        else if (input == "clear") {
            calculator.clear();
            std::cout << "栈已清空" << std::endl;
        }
        else if (input == "show") {
            calculator.displayStack();
        }
        else if (input == "history") {
            calculator.showHistory();
        }
        else if (input == "batch") {
            std::cout << "进入批量计算模式，输入多个表达式(空行结束):" << std::endl;
            std::vector<std::string> expressions;
            std::string expr;
            
            while (true) {
                std::cout << "expr> ";
                std::getline(std::cin, expr);
                if (expr.empty()) break;
                expressions.push_back(expr);
            }
            
            calculator.batchCalculate(expressions);
        }
        else if (!input.empty()) {
            // 保存当前栈状态用于错误恢复
            std::stack<double> backup = calculator.evaluateExpression(input) ? 
                std::stack<double>() : calculator.evaluateExpression(input) ? 
                std::stack<double>() : std::stack<double>();
            
            // 实际计算
            if (calculator.evaluateExpression(input)) {
                double result = calculator.getResult();
                std::cout << "结果: " << result << std::endl;
                calculator.addToHistory(input, result);
            } else {
                std::cout << "计算失败，请检查输入" << std::endl;
            }
        }
    }
    
    std::cout << "感谢使用RPN计算器！" << std::endl;
    return 0;
}
