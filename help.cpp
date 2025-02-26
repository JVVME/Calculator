//
// Created by zhang on 2025/2/23.
//

#include "include/help.h"

#include <iostream>
#include <fstream>

void chinese_doc() {
    std::ofstream outFile;
    outFile.open("中文文档.md");
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return ;
    }

    outFile << "# Calculator中文文档\n\n";
    outFile << "## 版本号与退出方式\n\n";
    outFile << "使用version命令查看版本号\n\n";
    outFile << "使用exit命令退出程序\n\n";
    outFile << "```\n";
    outFile << "(cal)>>exit\n";
    outFile << " Program exit success.\n";
    outFile << "```\n\n";
    outFile << "## 计算\n\n";
    outFile << "计算需要利用 calc指令，在calc后输入表达式，可以返回表达式的值，如:\n\n";
    outFile << "```\n";
    outFile << " (cal)>>calc 1 + 1\n";
    outFile << " 2\n";
    outFile << "```\n\n";
    outFile << "同时，程序支持自定义变量，在终端中直接声明变量名并赋值即可，如:\n\n";
    outFile << "```\n";
    outFile << "(cal)>>a = 5\n";
    outFile << "```\n\n";
    outFile << "程序同样支持函数声明，定义方式如下:\n\n";
    outFile << "```\n";
    outFile << "(cal)>>f(x) = x + 1\n";
    outFile << "(cal)>>g(x, y) = 2 * x * y\n";
    outFile << "```\n\n";
    outFile << "并且在calc中可以使用自定义的函数和变量，如：\n\n";
    outFile << "```\n";
    outFile << "(cal)>>calc 1+f(g(1,1)*a)\n";
    outFile << " 12\n";
    outFile << "```\n\n";
    outFile << "---\n\n";
    outFile << "## 记录\n\n";
    outFile << "程序支持使用以下命令查询自定义变量和函数以及历史记录\n\n";
    outFile << "show fun: 输出自定义的函数\n\n";
    outFile << "```\n";
    outFile << "(cal)>>show fun\n";
    outFile << " Name      Variable  Expression\n";
    outFile << "f         x         x+1\n";
    outFile << "g         x y       2*x*y\n";
    outFile << "```\n\n";
    outFile << "show var: 输出自定义变量\n\n";
    outFile << "```\n";
    outFile << "(cal)>>show var\n";
    outFile << " Name      Value\n";
    outFile << "a         5\n";
    outFile << "```\n\n";
    outFile << "show log: 展示历史记录\n\n";
    outFile << "```\n";
    outFile << "(cal)>>show log\n";
    outFile << " Call                Expression             Result\n";
    outFile << "Calculation         1+1                    2.000000\n";
    outFile << "Assign Variable     a=5                    Success\n";
    outFile << "Assign Function     f(x)=x+1               Success\n";
    outFile << "Assign Function     g(x,y)=2*x*y           Success\n";
    outFile << "Calculation         1+f(g(1,1)*a)          12.000000\n";
    outFile << "```\n\n";
    outFile << "save: 保存当前历史记录\n\n";
    outFile << "```save [filename]```\n\n";
    outFile << "会保存记录到 [filename].txt 中\n\n";
    outFile << "```save```\n\n";
    outFile << "会保存文件到以当前时间为名字的txt文本文件中\n\n";
    outFile << "---\n\n";
    outFile << "## 自带函数\n\n";
    outFile << "- sin\n";
    outFile << "- cos\n";
    outFile << "- tan\n";
    outFile << "- arcsin\n";
    outFile << "- arccos\n";
    outFile << "- arctan\n";
    outFile << "- abs\n";
    outFile << "- exp  (e的n次方)\n";
    outFile << "- exp2  (2的n次方)\n";
    outFile << "- ln\n";
    outFile << "- log10  (以10为底的对数)\n";
    outFile << "- log2     (以2为底的对数)\n";
    outFile << "- pow      (pow(a, b) 计算a的b次方)\n";
    outFile << "- sqrt\n\n";
    outFile << "## 自带常量\n\n";
    outFile << "- pi   π\n\n";
    outFile << "- e    自然对数\n\n";


    outFile.close();
    std::cout << "Content written to file successfully!" << std::endl;
}

void document(std::string language) {
    if (language == "zh-cn") {
        chinese_doc();
    }
}
