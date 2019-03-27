#ifndef COMMON_H_
#define COMMON_H_

#include "clang/AST/Decl.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/Type.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

using namespace std;
using namespace llvm;
using namespace clang;
using namespace tooling;
using namespace ast_matchers;

extern DeclarationMatcher FunctionMatcher; 
extern DeclarationMatcher RecordMatcher;

class NestedFunction;
class Tree;

extern map<int, NestedFunction> FunctionData;
extern map<string, Tree> RecordData;

#endif