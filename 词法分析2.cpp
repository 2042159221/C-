#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// 定义单词类型枚举
enum TokenType {
    ident, // 标识符
    number, // 数字
    keyword,// 关键字
    symbol  //符号

};
const char* type_names[] = { //定义数组
    "ident", // 标识符
    "number", // 数字
    "keyword" ,// 关键字
    "symbol"  //符号

};

// 定义符号类型枚举
enum Symbol {

    Plus, // "+"
    Minus, // "-"
    times, // "*"
    slash, // "/"
    eql, // "="
    lss, // "<"
    gtr, // ">"
    lparen, // "("
    rparen, // ")"
    comma, // ","
    semicolon, // ";"
    colon, // ":"
    period, // "."
    becomes, // ":="
    leq, // "<="
    geq, // ">="
    neq,// "#"
    error//无效字符

};
const char* symbol_names[] = {

    "Plus", // "+"
    "Minus", // "-"
    "times", // "*"
    "slash", // "/"
    "eql", // "="
    "lss", // "<"
    "gtr", // ">"
    "lparen", // "("
    "rparen", // ")"
    "comma", // ","
    "semicolon", // ";"
    "colon", // ":"
    "period", // "."
    "becomes", // ":="
    "leq", // "<="
    "geq", // ">="
    "neq" ,// "#"
    "error"//无效字符

};

// 定义单词结构体
struct Token {
    TokenType type; // 单词类型
    Symbol symSignal;//符号类型
    string value; // 单词值
};

// 定义关键字
vector<string> keywords = { "const", "var", "procedure", "begin", "end", "if", "then", "while", "do", "call", "odd", "write", "read" };

// 判断一个字符串是否是关键字
bool isKeyword(string str) {
    for (string keyword : keywords) {
        if (str == keyword) {
            return true;
        }
    }
    return false;
}

// 判断一个字符是否是有意义的符号
bool isSymbol(char c) {
    string symbols = "+-*/=<>(),.;:";
    return symbols.find(c) != string::npos;
}

// 获取下一个单词
Token getNextToken(ifstream& ifs) {
    Token token;
    char c;

    // 跳过空格和注释
    while (ifs.get(c)) {
        if (isspace(c)) {
            continue;
        }
        else if (c == '/') {
            if (ifs.get(c) && c == '*') {
                // 跳过注释
                while (ifs.get(c)) {
                    if (c == '*' && ifs.get(c) && c == '/') {
                        break;
                    }
                }
            }
            else {
                ifs.putback(c);
                break;
            }
        }
        else {
            break;
        }
    }

    // 判断标识符、关键字等类型
    if (isalpha(c)) {
        token.type = ident;
        token.value += c;
        while (ifs.get(c) && isalnum(c)) {
            token.value += c;
        }
        ifs.putback(c);
        if (isKeyword(token.value)) {
            token.type = keyword;
        }
    }

    // 判断数字类型
    else if (isdigit(c)) {
        token.type = number;
        token.value += c;
        while (ifs.get(c) && isdigit(c)) {
            token.value += c;
        }
        ifs.putback(c);
    }

    // 判断符号类型
    else if (isSymbol(c)) {
        token.type = symbol;
        token.value += c;
        switch (c) {
        case '+':  token.symSignal = Plus; token.value = '+'; break;
        case '-': token.symSignal = Minus; token.value = '-'; break;
        case '*': token.symSignal = times; token.value = '*'; break;
        case '/': token.symSignal = slash; token.value = '/'; break;
        case '=': token.symSignal = eql; token.value = '='; break;
        case '#': token.symSignal = neq; token.value = '#'; break;
        case '<': {
            if (ifs.get(c) && c == '=') {
                token.symSignal = leq;
                token.value = "<=";
            }

            else {
                ifs.putback(c);
                token.symSignal = lss;
                token.value = "<";
            }
            break;
        }
        case '>': {
            if (ifs.get(c) && c == '=') {
                token.symSignal = geq;
                token.value = ">=";
            }
            else {
                ifs.putback(c);
                token.symSignal = gtr;
                token.value = ">";
            }
            break;
        }
        case '(': token.symSignal = lparen; token.value = '('; break;
        case ')': token.symSignal = rparen; token.value = ')'; break;
        case ',': token.symSignal = comma; token.value = ','; break;
        case ';': token.symSignal = semicolon; token.value = ';'; break;
        case ':': {
            if (ifs.get(c) && c == '=') {
                token.symSignal = becomes;
                token.value = ":=";
            }
            else {
                ifs.putback(c);
                token.symSignal = colon;
                token.value = ":";
            }
            break;
        }
        case '.': token.symSignal = period; token.value = "."; break;
        }
    }

    // 无效字符处理
    else {
        token.symSignal = error;
        token.value += c;
    }

    return token;
}
void printFileContents(const std::string& filename) {  //在控制端打印输入文件数据
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::cout << "source program: " << line << std::endl;
    }
}

int main() {
    // 打开输入，并输出在控制端
    ifstream ifs("input.txt");
    std::string filename = "input.txt";
    printFileContents(filename);
    //打开输出
    ofstream ofs("output.txt");
    // 逐个单词写入到输出文件中 
    Token token;
    while ((token = getNextToken(ifs)).type != symbol || token.value != ".")
    {
        if (token.type == keyword)
        {
            ofs << "(" << token.value << ")" << endl;
        }
        else if (token.type == symbol) {
            ofs << "(" << symbol_names[token.symSignal] << ", " << token.value << ")" << endl;
        }
        else {
            ofs << "(" << type_names[token.type] << ", " << token.value << ")" << endl;
        }
    }
    ofs << "(" << symbol_names[token.symSignal] << ", " << token.value << ")" << endl;
    // 关闭输入和输出文件
    ifs.close();
    ofs.close();
    return 0;
}