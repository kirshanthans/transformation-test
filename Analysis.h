#ifndef ANALYSIS_H_
#define ANALYSIS_H_

#include "Common.h"


class MatchPrinter : public MatchFinder::MatchCallback {
public:
  virtual void run(const MatchFinder::MatchResult &Result);
};

class Tree {
public:
  const CXXRecordDecl *decl;
  const ASTContext *ctx;
  vector<string> recfls;
  Tree(){}
  Tree(const CXXRecordDecl *_decl, const ASTContext *_ctx);
  ~Tree(){}
};

class Expression{
  public:
    Expr * exp;
    string exps;
    Expression(){}
    Expression(Expr * _exp, string _exps): exp(_exp), exps(_exps) {}
    ~Expression(){}
    string to_string(){return exps;}
};

class CallExpression{
  public:
    CallExpr * call;
    string callee;
    vector<Expression> args;
    CallExpression(){}
    CallExpression(CallExpr * _call, string _callee, vector<Expression>_args): call(_call), callee(_callee) {
      for(auto e:_args) args.push_back(e);
    }
    ~CallExpression(){}
    string to_string(){
      string output("");
      output.append(callee).append("(");
      for(uint i = 0; i < args.size(); ++i){
        output.append(args[i].to_string());
        if(i != args.size() - 1) output.append(",");
        else output.append(")");
      }
      return output;
    }
};

class NestedFunction {
public:
  const FunctionDecl *decl;
  const ASTContext *ctx;
  int callcnt = 0;
  int level = 0;
  int size = 0;
  bool loop = false;
  bool lastlevel = false;
  pair<string, string> indvar;
  vector<pair<string, string>> params;
  map<string, Stmt *> body;
  map<int, string> order;
  map<string, int> orderinv;
  string fname;
  Expression guard;
  map<string, CallExpression> selfcall;
  map<string, CallExpression> othercall;
  bool hasComputation = false;
  string computation = "";
  string compLabel = "";
  NestedFunction(){}
  NestedFunction(const FunctionDecl *_decl, const ASTContext *_ctx);
  ~NestedFunction(){}
  string getStringId(Stmt * stmt, int ord);
  Expr* getGuardCond();
  void PrintStmts(bool withLabel);
  void PrintOrder();
  string guardStmt();
  bool isguardlabel(string s)    { return s[0] == 'g'; }
  bool iscalllabel(string s)     { return s[0] == 'r'; }
  bool istransferlabel(string s) { return s[0] == 't'; }
  string prettyPrint(BinaryOperator *binop);
};

#endif