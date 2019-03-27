#include "Analysis.h"
#include "Annotations.h"
#include "Utils.h"

DeclarationMatcher FunctionMatcher = functionDecl().bind("function");
DeclarationMatcher RecordMatcher = cxxRecordDecl().bind("tree");
map<int, NestedFunction> FunctionData;
map<string, Tree> RecordData;

Tree::Tree(const CXXRecordDecl *_decl, const ASTContext *_ctx) {
  decl = _decl;
  ctx = _ctx;
  for (auto t = decl->field_begin(); t != decl->field_end(); ++t) {
    if (hasRecursiveFLAnnoatation(*t))
      recfls.push_back(t->getNameAsString());
  }
}

NestedFunction::NestedFunction(const FunctionDecl *_decl,
                               const ASTContext *_ctx) {
  decl = _decl;
  ctx = _ctx;
  fname = decl->getNameAsString();
  // Dealing with some annotations
  level = getNestedLevel(decl);
  loop = isLoop(decl);
  // Dealing with the parameters
  for(auto t = decl->param_begin(); t != decl->param_end(); ++t){
      string name = (*t)->getNameAsString();
      string type = (*t)->getOriginalType().getAsString();
      if(hasIndVarAnnotation(*t)) indvar = make_pair(type, name);
      params.push_back(make_pair(type, name));
  }
  //Dealing with the body of the function
  Stmt *bd = decl->getBody();
  int ord = 0;
  for(auto t = bd->child_begin(); t != bd->child_end(); ++t){
      string sid = getStringId(*t, ord);
      body[sid]  = *t;
      order[ord] = sid;
      orderinv[sid] = ord;
      ord++;
      if (isa<CallExpr>(*t)){
        CallExpr *cc = cast<CallExpr>(*t);
        string cname = cc->getDirectCallee()->getNameAsString();
        vector<Expression> args_exp;
        for(auto it = cc->arg_begin(); it != cc->arg_end(); ++it){
          Expr * e = *it;
          string es = exprTostr(e, ctx->getSourceManager(), ctx->getLangOpts());
          Expression ex = Expression(e, es);
          args_exp.push_back(ex);
        }
        CallExpression cx = CallExpression(cc, cname, args_exp);
        if (fname == cname) selfcall[sid] = cx;
        else othercall[sid] = cx;
      }
      if (othercall.empty()) lastlevel = true;
  }
  size = ord;
  Expr * gd_ = getGuardCond();
  if(gd_ != NULL) {
    string gs_ = exprTostr(gd_, ctx->getSourceManager(), ctx->getLangOpts());
    guard = Expression(gd_, gs_);
  }
}

string NestedFunction::getStringId(Stmt * stmt, int ord) {
    string s("");
    if (isa<IfStmt>(stmt)){
        IfStmt *guard = cast<IfStmt>(stmt);
        if(isa<ReturnStmt>(guard->getThen())){
            s.append("g").append(to_string(level));
            return s;
        }
    } else if(isa<CallExpr>(stmt)){
        CallExpr *call = cast<CallExpr>(stmt);
        string callee_name = call->getDirectCallee()->getNameAsString();
        string caller_name = decl->getNameAsString();
        if(callee_name != caller_name) {
            s.append("t").append(to_string(level));
            return s;
        }
        s.append("r").append(to_string(level));
        if (!loop){ //later has to be changed for proper analysis of rec field
          callcnt++;
          if(callcnt == 1) s.append("l");
          else s.append("r");
        }
        return s;
    }
     else {
        s.append("s").append(to_string(level));
        hasComputation = true;
        computation = stmtTostr(stmt, ctx->getSourceManager(), ctx->getLangOpts());
        compLabel = s;
        return s;
    }
    return s;
}

Expr* NestedFunction::getGuardCond() {
  string gid = "g";
  gid.append(to_string(level));
  if(isa<IfStmt>(body[gid])){
    IfStmt *guard = cast<IfStmt>(body[gid]);
    return guard->getCond();
  }
  return NULL;
}

void NestedFunction::PrintStmts(bool withLabel) {
    //for(auto t = body.begin(); t != body.end(); ++t){
    //    cout << stmtTostr(t->second, ctx->getSourceManager(), ctx->getLangOpts()) << endl;
    //}
    for(int i = 0; i < size; i++){
      if(withLabel) cout<< order[i] << ": ";
      if(isguardlabel(order[i])) cout << guardStmt() << endl;
      else if(iscalllabel(order[i])) cout << selfcall[order[i]].to_string() << ";" << endl;
      else if(istransferlabel(order[i])) cout << othercall[order[i]].to_string() << ";" << endl;
      else cout << computation << ";" << endl;

    }
}

void NestedFunction::PrintOrder() {
    for(auto t = order.begin(); t != order.end(); ++t){
        cout << t->second << endl;
    }

}

string NestedFunction::guardStmt() {
  string s("if ( ");
  s.append(guard.to_string());
  s.append(" ) return;");
  return s;
}

string NestedFunction::prettyPrint(BinaryOperator *binop) {
  // string op = binop->getOpcodeStr();
  // auto LHS  = binop->getLHS()->IgnoreImplicit();
  // auto RHS  = binop->getRHS()->IgnoreImplicit();

  // string lhs_, rhs_;

  // if (isa<BinaryOperator>(LHS)){
  //   BinaryOperator *binop_lhs = cast<BinaryOperator>(LHS);
  //   lhs_ = prettyPrint(binop_lhs);
  // } else if (isa<MemberExpr>(LHS)){
  //   MemberExpr *mem_lhs = cast<MemberExpr>(LHS);
  // } else if(isa<IntegerLiteral>(LHS)){
  //   IntegerLiteral *intlit = cast<IntergerLiteral>(LHS);
  // }
  return "";


}

void MatchPrinter::run(const MatchFinder::MatchResult &Result) {
  if (const FunctionDecl *FD =
          Result.Nodes.getNodeAs<FunctionDecl>("function")) {
    if (FD->isThisDeclarationADefinition() && !FD->isMain() &&
        hasNestedAnnotation(FD)) {
      string fName = FD->getNameAsString();
      int level = getNestedLevel(FD);
      if(FunctionData.find(level) == FunctionData.end()){
        FunctionData[level] = NestedFunction(FD, Result.Context);
      //if (FunctionData.find(fName) == FunctionData.end()) {
        //FunctionData[fName] = NestedFunction(FD, Result.Context);
        //FunctionData[fName].PrintStmts();
      }
    }
  } else if (const CXXRecordDecl *DS =
                 Result.Nodes.getNodeAs<CXXRecordDecl>("tree")) {
    if (hasRecursiveDSAnnotation(DS)) {
      string dName = DS->getNameAsString();
      if (RecordData.find(dName) == RecordData.end()) {
        RecordData[dName] = Tree(DS, Result.Context);
      }
    }
  }
}