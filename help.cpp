//
// Created by zhang on 2025/2/23.
//

#include "include/help.h"

#include <iostream>
#include <fstream>

void chinese_doc() {
    std::ofstream outFile;
    outFile.open("�����ĵ�.md");
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return ;
    }

    outFile << "# Calculator�����ĵ�\n\n";
    outFile << "## �汾�����˳���ʽ\n\n";
    outFile << "ʹ��version����鿴�汾��\n\n";
    outFile << "ʹ��exit�����˳�����\n\n";
    outFile << "```\n";
    outFile << "(cal)>>exit\n";
    outFile << " Program exit success.\n";
    outFile << "```\n\n";
    outFile << "## ����\n\n";
    outFile << "������Ҫ���� calcָ���calc��������ʽ�����Է��ر��ʽ��ֵ����:\n\n";
    outFile << "```\n";
    outFile << " (cal)>>calc 1 + 1\n";
    outFile << " 2\n";
    outFile << "```\n\n";
    outFile << "ͬʱ������֧���Զ�����������ն���ֱ����������������ֵ���ɣ���:\n\n";
    outFile << "```\n";
    outFile << "(cal)>>a = 5\n";
    outFile << "```\n\n";
    outFile << "����ͬ��֧�ֺ������������巽ʽ����:\n\n";
    outFile << "```\n";
    outFile << "(cal)>>f(x) = x + 1\n";
    outFile << "(cal)>>g(x, y) = 2 * x * y\n";
    outFile << "```\n\n";
    outFile << "������calc�п���ʹ���Զ���ĺ����ͱ������磺\n\n";
    outFile << "```\n";
    outFile << "(cal)>>calc 1+f(g(1,1)*a)\n";
    outFile << " 12\n";
    outFile << "```\n\n";
    outFile << "---\n\n";
    outFile << "## ��¼\n\n";
    outFile << "����֧��ʹ�����������ѯ�Զ�������ͺ����Լ���ʷ��¼\n\n";
    outFile << "show fun: ����Զ���ĺ���\n\n";
    outFile << "```\n";
    outFile << "(cal)>>show fun\n";
    outFile << " Name      Variable  Expression\n";
    outFile << "f         x         x+1\n";
    outFile << "g         x y       2*x*y\n";
    outFile << "```\n\n";
    outFile << "show var: ����Զ������\n\n";
    outFile << "```\n";
    outFile << "(cal)>>show var\n";
    outFile << " Name      Value\n";
    outFile << "a         5\n";
    outFile << "```\n\n";
    outFile << "show log: չʾ��ʷ��¼\n\n";
    outFile << "```\n";
    outFile << "(cal)>>show log\n";
    outFile << " Call                Expression             Result\n";
    outFile << "Calculation         1+1                    2.000000\n";
    outFile << "Assign Variable     a=5                    Success\n";
    outFile << "Assign Function     f(x)=x+1               Success\n";
    outFile << "Assign Function     g(x,y)=2*x*y           Success\n";
    outFile << "Calculation         1+f(g(1,1)*a)          12.000000\n";
    outFile << "```\n\n";
    outFile << "save: ���浱ǰ��ʷ��¼\n\n";
    outFile << "```save [filename]```\n\n";
    outFile << "�ᱣ���¼�� [filename].txt ��\n\n";
    outFile << "```save```\n\n";
    outFile << "�ᱣ���ļ����Ե�ǰʱ��Ϊ���ֵ�txt�ı��ļ���\n\n";
    outFile << "---\n\n";
    outFile << "## �Դ�����\n\n";
    outFile << "- sin\n";
    outFile << "- cos\n";
    outFile << "- tan\n";
    outFile << "- arcsin\n";
    outFile << "- arccos\n";
    outFile << "- arctan\n";
    outFile << "- abs\n";
    outFile << "- exp  (e��n�η�)\n";
    outFile << "- exp2  (2��n�η�)\n";
    outFile << "- ln\n";
    outFile << "- log10  (��10Ϊ�׵Ķ���)\n";
    outFile << "- log2     (��2Ϊ�׵Ķ���)\n";
    outFile << "- pow      (pow(a, b) ����a��b�η�)\n";
    outFile << "- sqrt\n\n";
    outFile << "## �Դ�����\n\n";
    outFile << "- pi   ��\n\n";
    outFile << "- e    ��Ȼ����\n\n";


    outFile.close();
    std::cout << "Content written to file successfully!" << std::endl;
}

void document(std::string language) {
    if (language == "zh-cn") {
        chinese_doc();
    }
}
