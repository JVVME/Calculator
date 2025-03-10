//
// Created by zhang on 2025/2/21.
//

#ifndef KERNEL_H
#define KERNEL_H
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <regex>
#include<cmath>

#include "Error.h"
#include "Log.h"
#include "help.h"
#include "Information.h"
#include "LAlgebra.h"

struct Token;
class Variable;
class Function;

void calculator_kernel(const std::string& input);

void preprocess(std::vector<Token>& tokens);

void p_calc(std::vector<std::string> & words);
void p_assign(std::vector<std::string> & words);
void p_show(std::vector<std::string> & words);
void p_save(std::vector<std::string> & words);
void p_mcalc(std::vector<std::string> & words);
void matrix_create(std::vector<std::string> & words);
double calc_function(std::string, std::vector<double> var);

#endif //KERNEL_H
