#include "Utils.h"
#include "Annotations.h"

// A simple statement printer
string stmtTostr(const Stmt *stmt, const SourceManager &SM, const LangOptions &LO) {
  SourceLocation b(stmt->getBeginLoc()), _e(stmt->getEndLoc());
  SourceLocation e(Lexer::getLocForEndOfToken(_e, 0, SM, LO));
  string Output = string(SM.getCharacterData(b), SM.getCharacterData(e)-SM.getCharacterData(b));
  return Output;
}
string exprTostr(const Expr *expr, const SourceManager &SM, const LangOptions &LO) {
  SourceLocation b(expr->getBeginLoc()), _e(expr->getEndLoc());
  SourceLocation e(Lexer::getLocForEndOfToken(_e, 0, SM, LO));
  string Output = string(SM.getCharacterData(b), SM.getCharacterData(e)-SM.getCharacterData(b));
  return Output;
}

// Printing necessary details of a function
//void funcToStr(const FunctionDecl *FD, const SourceManager *SM) {
//  cout << "Nested Recursive Function Name " << FD->getNameAsString() << endl;
//  cout << "Function Parameters" << endl;
//  int p = 0;
//  for (auto t = FD->param_begin(); t != FD->param_end(); ++t) {
//    cout << "\tParameter " << p << " Name: " << (*t)->getNameAsString()
//         << " Type: " << (*t)->getOriginalType().getAsString();
//    if (hasIndVarAnnotation(*t))
//      cout << " Induction Variable";
//    cout << endl;
//    p++;
//  }
//  cout << "Nested recursive Function Body " << endl;
//  Stmt *body = FD->getBody();
//  int m = 0;
//  for (auto t = body->child_begin(); t != body->child_end(); ++t) {
//    cout << "\tStatement " << m << ": " << stmtTostr(*t, *SM) << endl;
//    m++;
//  }
//}
//
//// Printing necessary details of a record
//void recordToStr(const CXXRecordDecl *DS, const SourceManager *SM) {
//  cout << "Recursive Data Strucutre Name: " << DS->getNameAsString() << endl;
//  int f = 0;
//  for (auto t = DS->field_begin(); t != DS->field_end(); ++t) {
//    cout << "\tMember " << f << ": " << t->getNameAsString() << " ";
//    if (hasRecursiveFLAnnoatation(*t))
//      cout << "Recursive Field";
//    cout << endl;
//    f++;
//  }
//}