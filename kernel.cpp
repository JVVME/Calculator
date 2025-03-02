//
// Created by zhang on 2025/2/21.
//

#include <utility>

#include "include/kernel.h"

#include <chrono>
#include <complex>
#include <fstream>
#include <iomanip>

#define IS_NEG (tokens[i].type == TK_SUB && (i == 0 || tokens[i - 1].type == TK_ADD || tokens[i - 1].type == TK_SUB \
 || tokens[i - 1].type == TK_MUL || tokens[i - 1].type == TK_DIV || tokens[i - 1].type == TK_LBR))
#define IS_FLOAT (i > 0 && tokens[i].type == TK_DOT && tokens[i - 1].type == TK_NUM && tokens[i + 1].type == TK_NUM)
#define IS_FUNCTION (tokens[i].type == TK_VAR && tokens[i + 1].type == TK_LBR)

std::vector<Variable> var_list;
std::vector<Function *> fun_list;
std::vector<std::pair<std::string, Matrix>> matrix_list;

M_Log logger;

enum TokenType {
    TK_ADD,
    TK_SUB,
    TK_MUL,
    TK_DIV,
    TK_LBR,
    TK_RBR,
    TK_NUM,
    TK_DOT,
    TK_VAR,
    TK_CMA,
    TK_EQL,
    TK_LSB,
    TK_RSB,
    TK_FUN = 256,
    TK_MAT,
};

struct Token {
    TokenType type;
    std::string str;
};

typedef struct Node {
    Token* token;
    double value = 0;
    std::vector<Node *> children;

}Node;

std::unordered_map<std::string, std::function<void(std::vector<std::string> &)>> functionMap= {
    {"calc", p_calc},
    {"show", p_show},
    {"save", p_save},
    {"matrix", matrix_create},
    {"mcalc",p_mcalc},
};

std::unordered_map<std::string, double> constants = {
    {"pi", std::numbers::pi},
    {"e", std::numbers::e},
};

class Expression {
public:
    std::string expr;
    std::vector<Token> tokens;

    size_t pos;
    std::vector<std::regex> patterns;

    Node *root = new Node();

    explicit Expression(std::string _expr): expr(_expr), pos(0){
        patterns = {
            std::regex("\\+"), // 加号
            std::regex("\\-"), // 减号
            std::regex("\\*"), // 乘号
            std::regex("\\/"), // 除号
            std::regex("\\("), // 左括号
            std::regex("\\)"), // 右括号
            std::regex("[0-9]+"), // 数字
            std::regex("\\."),
            std::regex("[a-zA-Z]+[0-9]*"),
            std::regex("\\,"),
            std::regex("\\="),
            std::regex("\\["),
            std::regex("\\]"),
        };}
    static bool check_parentheses(int p, int q, std::vector<Token>& tks){
        if(tks[p].type != TK_LBR || tks[q].type != TK_RBR){
            return false;
        }
        int count = 0;
        for(int i = p + 1; i <= q - 1; i++){
            if(tks[i].type == TK_LBR){
                count++;
            }
            else if(tks[i].type == TK_RBR){
                count--;
            }
            if(count < 0){
                return false;
            }
        }
        return (count == 0);
    }
    void create_tree(int p, int q, Node* node) {
        // std::cout << p << " " << q << std::endl;
        if (p > q) return;
        if (p == q) {
            // std::cout << "write " << p << std::endl;
            node->token = &tokens[p]; // 叶子节点
            return;
        }

        // 处理括号
        if (check_parentheses(p, q, tokens)) {
            p++;
            q--;
        }

        // 处理函数的情况
        if (tokens[p].type == TK_FUN && check_parentheses(p + 1, q, tokens)) {
            node->token = &tokens[p];
            int cnt = 1;
            int begin = p + 2;
            int end = p + 3;
            while (end <= q) {
                if (tokens[end].type == TK_RBR) cnt--;
                else if (tokens[end].type == TK_LBR) cnt++;
                else if (tokens[end].type == TK_CMA && cnt == 1) {
                    Node *node1 = new Node();
                    node->children.emplace_back(node1);
                    create_tree(begin, end - 1, node1);
                    begin = end + 1;
                }
                end++;
            }
            Node *node1 = new Node();
            node->children.emplace_back(node1);
            create_tree(begin, end - 2, node1);
        }

        // 根据操作符进行分割
        int cnt = 0;
        for (int i = p; i <= q; i++) {
            if (tokens[i].type == TK_LBR) cnt++;
            else if (tokens[i].type == TK_RBR) cnt--;
            if (cnt == 0 && (tokens[i].type == TK_ADD || tokens[i].type == TK_SUB || tokens[i].type == TK_MUL || tokens[i].type == TK_DIV)) {
                node->token = &tokens[i];
                Node *node1 = new Node();
                Node *node2 = new Node();
                node->children.emplace_back(node1);
                node->children.emplace_back(node2);
                // std::cout << p << " " << i << " " << q << std::endl;
                create_tree(p, i - 1, node1); // 递归左子树
                create_tree(i + 1, q, node2); // 递归右子树
                break;
            }
        }
    }
    bool parse() {
        while (pos < expr.length()) {
            bool matched = false;
            for (size_t i = 0; i < patterns.size(); ++i) {
                std::smatch match;
                // 使用正则表达式匹配
                if (std::regex_search(expr.cbegin() + pos, expr.cend(), match, patterns[i], std::regex_constants::match_continuous)) {
                    Token token;
                    token.str = match.str(0);

                    // 根据正则表达式的顺序分配类型
                    if (i == TK_ADD) token.type = TK_ADD;     // 加号
                    else if (i == TK_SUB) token.type = TK_SUB; // 减号
                    else if (i == TK_MUL) token.type = TK_MUL; // 乘号
                    else if (i == TK_DIV) token.type = TK_DIV; // 除号
                    else if (i == TK_LBR) token.type = TK_LBR; // 左括号
                    else if (i == TK_RBR) token.type = TK_RBR; // 右括号
                    else if (i == TK_NUM) token.type = TK_NUM; // 数字
                    else if (i == TK_DOT) token.type = TK_DOT;
                    else if (i == TK_VAR) token.type = TK_VAR;
                    else if (i == TK_CMA) token.type = TK_CMA;
                    else if (i == TK_EQL) token.type = TK_EQL;
                    else if (i == TK_LSB) token.type = TK_LSB;
                    else if (i == TK_RSB) token.type = TK_RSB;
                    else {
                        break;
                    }

                    tokens.push_back(token);
                    pos += match.length();  // 更新 pos，继续从新的位置查找
                    matched = true;
                    break;
                }
            }

            if (!matched) {
                std::cerr << "Unable to parse the expression at position " << pos << ": " << expr.substr(pos) << std::endl;
                return false;
            }
        }

        return true;
    }
    void printTokens() const {
        for (const auto& token : tokens) {
            std::cout << "Token: " << token.str << ", Type: " << token.type << std::endl;
        }
    }
    static void to_dot(const Node* node, std::ostream& os, int& counter) {
        if (node == nullptr) return;

        // 为当前节点生成唯一的标识符
        std::string node_name = "node" + std::to_string(counter);  // 每次递增

        // 如果当前节点有token，添加label
        os << "  " << node_name << " [label=\"" << (node->token ? node->token->str : "NULL") << "\"];\n";

        // 递归遍历子节点并生成边
        for (auto& child : node->children) {
            counter++;
            std::string child_name = "node" + std::to_string(counter);  // 保证唯一性
            os << "  " << node_name << " -> " << child_name << ";\n";  // 添加边
            to_dot(child, os, counter);  // 递归遍历子节点
        }
    }
    std::string to_dot() const {
        std::ostringstream oss;
        oss << "digraph G {\n";  // 开始图的定义
        int counter = 0;  // counter 从 0 开始
        to_dot(root, oss, counter);  // 从根节点开始生成 DOT 格式
        oss << "}\n";  // 结束图的定义
        return oss.str();
    }
};

class Variable {
public:
    std::string name;
    double value;
    Variable(std::string _name, double _val) : name(std::move(_name)), value(_val){}
};

class Function : public Expression{
public:
    std::string name;
    std::vector<std::string> f_var;
    explicit Function(std::string _name, std::string _expr) : Expression(_expr), name(_name) {
    }

    double function_val(std::vector<double>& var) {
        // std::cout << "Hello1" << std::endl;
        calc_val(root, var);
        return root->value;
    }

private:
    void calc_val(Node * node, std::vector<double>& tmp_var) {
        // std::cout << "Hello2" << std::endl;
        if (node->children.empty()) {
            if (node->token->type == TK_NUM) {
                node->value = std::stod(node->token->str);
            }
            else if (node->token->type == TK_VAR) {
                // std::cout << "check var" <<std::endl;
                for (int i = 0; i < f_var.size(); i++) {
                    if (f_var[i] == node->token->str) {
                        // std::cout << "set to " << tmp_var[i] << std::endl;
                        node->value = tmp_var[i];
                    }
                }
                if (constants.contains(node->token->str)) {
                    node->value = constants[node->token->str];
                }
                for (auto& var: var_list) {
                    if (var.name == node->token->str) {
                        node->value = var.value;
                    }
                }
            }
            return ;
        }

        for (auto child: node->children) {
            calc_val(child, tmp_var);
        }

        if (node->token->type == TK_FUN) {
            std::vector<double> var;
            for (auto& v : node->children) {
                var.push_back(v->value);
            }
            node->value = calc_function(node->token->str, var);
            return ;
        }

        else {
            if (node->token->type == TK_ADD) {
                node->value = node->children[0]->value + node->children[1]->value;
            }
            else if (node->token->type == TK_SUB) {
                node->value = node->children[0]->value - node->children[1]->value;
            }
            else if (node->token->type == TK_MUL) {
                node->value = node->children[0]->value * node->children[1]->value;
            }
            else if (node->token->type == TK_DIV) {
                if (node->children[1]->value == 0) {
                    std::cout << "Dividend divided by zero, your expression will cause system crash "
                                 "replace the divisor with 1, and your result is wrong." << std::endl;
                    node->children[1]->value = 1;
                }
                node->value = node->children[0]->value / node->children[1]->value;
            }
            return ;
        }
    }
};

class ExpressionParser : public Expression{
public:
    double expr_val;

    explicit ExpressionParser(std::string  expr) : Expression(expr),expr_val(0) {
        // 初始化正则表达式模式，按优先级顺序

    }

    static void calc_tree(Node * node) {
        if (node->children.empty()) {
            if (node->token->type == TK_NUM) {
                node->value = std::stod(node->token->str);
            }
            else if (node->token->type == TK_VAR) {
                if (constants.contains(node->token->str)) {
                    node->value = constants[node->token->str];
                }
                for (auto& var: var_list) {
                    if (var.name == node->token->str) {
                        node->value = var.value;
                    }
                }
            }
            return ;
        }

        for (auto child: node->children) {
            calc_tree(child);
        }

        if (node->token->type == TK_FUN) {
            std::vector<double> var;
            for (auto& v : node->children) {
                var.push_back(v->value);
            }
            node->value = calc_function(node->token->str, var);
            return ;
        }

        else {
            if (node->token->type == TK_ADD) {
                node->value = node->children[0]->value + node->children[1]->value;
            }
            else if (node->token->type == TK_SUB) {
                node->value = node->children[0]->value - node->children[1]->value;
            }
            else if (node->token->type == TK_MUL) {
                node->value = node->children[0]->value * node->children[1]->value;
            }
            else if (node->token->type == TK_DIV) {
                if (node->children[1]->value == 0) {
                    std::cout << "Dividend divided by zero, your expression will cause system crash "
                                 "replace the divisor with 1, and your result is wrong." << std::endl;
                    node->children[1]->value = 1;

                }
                node->value = node->children[0]->value / node->children[1]->value;
            }
            return ;
        }

    }

    static void print_tree(const Node& node, int depth = 0) {
        // 打印当前节点的 token
        for (int i = 0; i < depth; i++) std::cout << "  ";  // 打印空格来表示层级
        std::cout << "Token: " << node.token->type << std::endl;

        // 递归打印子节点
        for (const auto& child : node.children) {
            print_tree(*child, depth + 1);
        }
    }

};


void matrix_create(std::vector<std::string> &words) {
    std::vector<std::vector<double>> init;

    words.erase(words.begin());
    std::string expr;
    for (auto & word : words) {
        expr += word;
    }
    ExpressionParser parser(expr);

    parser.parse();
    // parser.printTokens();

    for (int i = 3; i < parser.tokens.size() - 1; i++) {
        if (parser.tokens[i].type == TK_LSB) {
            std::vector<double> line;
            while (parser.tokens[i].type != TK_RSB) {
                if (parser.tokens[i].type == TK_NUM) {
                    line.push_back(std::stod(parser.tokens[i].str));
                }
                i++;
            }
            init.push_back(line);
        }
    }

    Matrix matrix(init.size(), init[0].size(), init);

    matrix_list.emplace_back(parser.tokens[0].str, matrix);
    // matrix.print();

    std::vector<std::string> log;
    log.emplace_back("Matrix create");
    std::string info = "Store matrix " + parser.tokens[0].str;
    log.push_back(info);
    std::string res = "Success. Size: " + std::to_string(matrix.getRow()) + "x" +std::to_string(matrix.getColumn());
    log.push_back(res);
    logger.log_add(log);


}

std::vector<std::string> split_string_by_space(const std::string& str) {
    std::vector<std::string> result;
    std::istringstream stream(str);
    std::string word;

    while (stream >> word) {
        result.push_back(word);
    }

    return result;
}

void calculator_kernel(const std::string& input) {
    std::vector<std::string> words = split_string_by_space(input);

    if (words.empty()) {
        return ;
    }

    const std::string& command = words[0];

    if (command == "help") {
        if (words.size() > 1) {
            document(words[1]);
        }
        else {
            document("zh-cn");
        }
    }
    else if (command == "version") {
        std::cout << "Calculator: v";
        std::cout << VERSION << std::endl;
    }
    else if (!functionMap.contains(command)) {
        p_assign(words);
    }
    else {
        functionMap[command](words);
    }


}

void preprocess(std::vector<Token>& tokens) {
    for (int i = 0; i < tokens.size(); i++) {
        if (IS_NEG) {
            tokens[i + 1].str = "-" + tokens[i + 1].str;
            tokens.erase(tokens.begin() + i);
        }
        else if (IS_FLOAT) {
            tokens[i - 1].str += ".";
            tokens[i - 1].str += tokens[i + 1].str;
            tokens.erase(tokens.begin() + i);
            tokens.erase(tokens.begin() + i);
            i--;
        }
        else if (IS_FUNCTION) {
            tokens[i].type = TK_FUN;
        }
    }


}

double calc_function(std::string fname, std::vector<double> var) {
    if (fname == "sin") {
        return std::sin(var[0]);
    }
    if (fname == "cos") {
        return std::cos(var[0]);
    }
    if (fname == "tan") {
        return std::tan(var[0]);
    }
    if (fname == "arcsin") {
        return std::asin(var[0]);
    }
    if (fname == "arccos") {
        return std::acos(var[0]);
    }
    if (fname == "arctan") {
        return std::atan(var[0]);
    }
    if (fname == "abs") {
        return std::abs(var[0]);
    }
    if (fname == "exp") {
        return std::exp(var[0]);
    }
    if (fname == "exp2") {
        return std::exp2(var[0]);
    }
    if (fname == "ln") {
        return std::log(var[0]);
    }
    if (fname == "log10") {
        return std::log10(var[0]);
    }
    if (fname == "log2") {
        return std::log2(var[0]);
    }
    if (fname == "pow") {
        return std::pow(var[0], var[1]);
    }
    if (fname == "sqrt") {
        return std::sqrt(var[0]);
    }

    for (auto f : fun_list) {
        if (f->name == fname) {
            return f->function_val(var);
        }
    }

    return 0;
}

void p_mcalc(std::vector<std::string> &words) {
    words.erase(words.begin());
    std::string expr;
    for (auto & word : words) {
        expr += word;
    }
    ExpressionParser parser(expr);
    parser.parse();

    // parser.printTokens();
    std::vector<Token> & tokens = parser.tokens;

    std::vector<std::string> log;
    log.push_back("Matrix Calculation");
    log.push_back(expr);

    if (tokens.size() == 4) {
        if (tokens[0].str == "det") {
            double res = 0;
            for (auto &[fst, snd]: matrix_list) {
                if (fst == tokens[2].str) {
                    res = snd.determinant();
                }
            }
            std::cout << res << std::endl;
            log.push_back(std::to_string(res));
        }
        else if (tokens[0].str == "T") {

            for (auto &[fst, snd]: matrix_list) {
                if (fst == tokens[2].str) {
                    Matrix m = snd.T();
                    std::string content = fst + "_Transpose";
                    matrix_list.emplace_back(content, m);
                    m.print();
                    log.push_back("Saved as " + content);
                }
            }
        }
        else if (tokens[0].str == "inverse") {

            for (auto &[fst, snd]: matrix_list) {
                if (fst == tokens[2].str) {
                    Matrix m = snd.inverse();
                    std::string content = fst + "_Inverse";
                    matrix_list.emplace_back(content, m);
                    m.print();
                    log.push_back("Saved as " + content);
                }
            }
        }

    }
    else if (tokens.size() == 3) {
        if (tokens[1].type == TK_ADD) {
            Matrix *x1 = nullptr;
            Matrix *x2 = nullptr;
            std::string res;
            for (auto & matrix: matrix_list) {
                if (matrix.first == tokens[0].str) {
                    x1 = &matrix.second;
                    res = matrix.first + "+";
                }
            }
            for (auto &[fst, snd]: matrix_list) {
                if (fst == tokens[2].str) {
                    x2 = &snd;
                    res += fst;
                }
            }
            if (x1 == nullptr || x2 == nullptr) {
                std::cout << "Invalid expression." << std::endl;
                return ;
            }
            Matrix m = *x1 + *x2;

            m.print();
            matrix_list.emplace_back(res,m);
            log.push_back("Saved as " + res);
        }
        else if (tokens[1].type == TK_SUB) {
            Matrix *x1 = nullptr;
            Matrix *x2 = nullptr;
            std::string res;
            for (auto &[fst, snd]: matrix_list) {
                if (fst == tokens[0].str) {
                    x1 = &snd;
                    res += fst;
                    res += "-";
                }
            }
            for (auto &[fst, snd]: matrix_list) {
                if (fst == tokens[2].str) {
                    x2 = &snd;
                    res += fst;
                }
            }
            if (x1 == nullptr || x2 == nullptr) {
                std::cout << "Invalid expression." << std::endl;
                return ;
            }
            Matrix m = *x1 - *x2;

            m.print();
            matrix_list.emplace_back(res,m);
            log.push_back("Saved as " + res);
        }
        else if (tokens[1].type == TK_MUL) {
            std::string res;
            if (tokens[0].type == TK_NUM) {
                Matrix *x1 = nullptr;
                res += tokens[0].str;
                res += "*";
                for (auto & matrix: matrix_list) {
                    if (matrix.first == tokens[2].str) {
                        x1 = &matrix.second;
                        res += matrix.first;
                    }
                }
                if (x1 == nullptr) {
                    std::cout << "Invalid expression." << std::endl;
                    return ;
                }
                Matrix m = *x1 * std::stod(tokens[0].str);

                m.print();
                matrix_list.emplace_back(res,m);
                log.push_back("Saved as " + res);

            }
            else if (tokens[2].type == TK_NUM) {
                Matrix *x1 = nullptr;
                res += tokens[2].str;
                res += "*";
                for (auto & matrix: matrix_list) {
                    if (matrix.first == tokens[0].str) {
                        x1 = &matrix.second;
                        res += matrix.first;
                    }
                }
                if (x1 == nullptr) {
                    std::cout << "Invalid expression." << std::endl;
                    return ;
                }
                Matrix m = *x1 * std::stod(tokens[2].str);

                m.print();
                matrix_list.emplace_back(res,m);
                log.push_back("Saved as " + res);

            }
            else {
                Matrix *x1 = nullptr;
                Matrix *x2 = nullptr;
                for (auto &[fst, snd]: matrix_list) {
                    if (fst == tokens[0].str) {
                        x1 = &snd;
                        res += fst;
                        res += "*";
                    }
                }
                for (auto &[fst, snd]: matrix_list) {
                    if (fst == tokens[2].str) {
                        x2 = &snd;
                        res += fst;
                    }
                }
                if (x1 == nullptr || x2 == nullptr) {
                    std::cout << "Invalid expression." << std::endl;
                    return ;
                }
                Matrix m = *x1 * *x2;

                m.print();
                matrix_list.emplace_back(res,m);
                log.push_back("Saved as " + res);

            }
        }
        else if (tokens[1].type == TK_DIV) {
            std::cout << "Suggest not using division, use multiply instead (/2 can be x0.5)" << std::endl;
            log.push_back("Suggest multiply");
        }
    }

    logger.log_add(log);

    // Matrix *x1, *x2;
    // for (auto & m : matrix_list) {
    //     if (m.first == words[1]) {
    //
    //     }
    // }

}

void p_calc(std::vector<std::string> & words) {
    // std::cout << "calc called" << std::endl;
    words.erase(words.begin());
    std::string expr;
    for (auto & word : words) {
        expr += word;
    }
    // std::cout << expr << std::endl;

    ExpressionParser parser(expr);

    if ( parser.parse()) {
        // parser.printTokens();
    } else {
        std::cerr << "Failed to parse the expression." << std::endl;
        return ;
    }

    std::vector<Token>& tokens = parser.tokens;
    preprocess(tokens);

    // parser.printTokens();

    parser.create_tree(0, tokens.size() - 1, parser.root);

    // std::string dot_output = parser.to_dot();

    // 如果要将输出写入文件
    // std::ofstream dot_file("../tree.dot");
    // std::cout << "output" <<std::endl;
    // dot_file << dot_output;
    // dot_file.close();

    // parser.print_tree(*parser.root);
    // std::cout << parser.expr << std::endl;

    parser.calc_tree(parser.root);

    std::cout << parser.root->value << std::endl;

    std::vector<std::string> res;
    res.push_back("Calculation");
    res.push_back(parser.expr);
    res.push_back(std::to_string(parser.root->value));

    logger.log_add(res);

}

void p_assign(std::vector<std::string> & words) {
    // std::cout << "assign called" << std::endl;
    std::string expr;
    for (auto & word : words) {
        expr += word;
    }
    ExpressionParser parser(expr);

    if ( parser.parse()) {
        // parser.printTokens();
        preprocess(parser.tokens);
    } else {
        std::cout << "Failed to parse the expression" << std::endl;
        return ;
    }

    if (parser.tokens[0].type == TK_VAR) {
        Variable var(parser.tokens[0].str, std::stod(parser.tokens[2].str));
        var_list.push_back(var);
        // std::cout << "Success saved variable  " << var.name << ": " << var.value <<std::endl;
        std::vector<std::string> res;
        res.push_back("Assign Variable");
        res.push_back(parser.expr);
        res.push_back("Success");

        logger.log_add(res);
    }

    if (parser.tokens[0].type == TK_FUN) {
        int cnt = 1, i = 2;
        std::string f_expr = "";
        bool eq_r = false;
        for (int p = 0; p < parser.tokens.size(); p++) {
            if (parser.tokens[p].type == TK_EQL) {
                eq_r = true;
                continue;
            }
            if (eq_r) {
                f_expr += parser.tokens[p].str;
            }
        }

        Function *fc = new Function(parser.tokens[0].str, f_expr);

        while (cnt != 0) {
            if (parser.tokens[i].type == TK_LBR) cnt++;
            else if (parser.tokens[i].type == TK_RBR) cnt--;
            else if (parser.tokens[i].type == TK_VAR) fc->f_var.push_back(parser.tokens[i].str);
            i++;
        }


        for (int j = i + 1; j < parser.tokens.size(); j++) {
            fc->tokens.push_back(parser.tokens[j]);
        }

        fc->create_tree(0, fc->tokens.size() - 1, fc->root);

        fun_list.push_back(fc);

        std::vector<std::string> res;
        res.push_back("Assign Function");
        res.push_back(parser.expr);
        res.push_back("Success");

        logger.log_add(res);

        // std::cout << fc->expr << std::endl;

        // std::string dot_output = parser.to_dot();
        // std::ofstream dot_file("../tree.dot");
        // std::cout << "output" <<std::endl;
        // dot_file << dot_output;
        // dot_file.close();


    }

}

void show_function() {
    std::cout << std::left << std::setw(10) << "Name";
    std::cout << std::setw(10) << "Variable";
    std::cout << "Expression" << std::endl;

    for (auto f: fun_list) {
        std::cout << std::left << std::setw(10) << f->name;
        std::string v = "";
        for (auto var: f->f_var) {
            v += var;
            v += " ";
        }
        std::cout << std::setw(10) << v;
        std::cout << f->expr << std::endl;
    }

}

void show_variable() {
    std::cout << std::left << std::setw(10) << "Name";
    std::cout << "Value" << std::endl;
    for (auto v: var_list) {
        std::cout << std::left << std::setw(10) << v.name;
        std::cout << v.value<<std::endl;
    }
}

void show_matrix() {
    for (auto & m : matrix_list) {
        std::cout << m.first << " ";
        m.second.print();
        std::cout << std::endl;;
    }
}

void p_show(std::vector<std::string> &words) {
    if (words[1] == "fun") {
        show_function();
    }
    else if (words[1] == "var") {
        show_variable();
    }
    else if (words[1] == "matrix") {
        show_matrix();
    }
    else if (words[1] == "log") {
        logger.show_log();
    }
}

std::string getCurrentDateTimeString() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf{};

    // 使用线程安全的localtime_r替代localtime
#if defined(_WIN32)
    localtime_s(&tm_buf, &now_time);
#else
    localtime_r(&now_time, &tm_buf);
#endif

    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", &tm_buf);
    return buffer;
}

void p_save(std::vector<std::string> &words) {
    std::string file_name;
    if (words.size() == 1) {
       file_name = getCurrentDateTimeString();
    }
    else {
        file_name = words[1];
    }
    file_name += ".txt";

    logger.save2file(file_name);
}
