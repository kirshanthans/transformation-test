#ifndef UTILS_H_
#define UTILS_H_

#include "Common.h"

string stmtTostr(const Stmt *stmt, const SourceManager &SM, const LangOptions &LO);
string exprTostr(const Expr *expr, const SourceManager &SM, const LangOptions &LO);

#endif
