// User defined header files
#include "Analysis.h"
#include "Common.h"
#include "Transform.h"
#include "Utils.h"

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("my-tool options");
namespace opts {
static cl::opt<bool> mode("transform",
                          cl::desc("run the tool in transform mode"),
                          cl::init(false), cl::Optional,
                          cl::cat(MyToolCategory));
}

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...\n");

int main(int argc, const char **argv) {
  CommonOptionsParser Op(argc, argv, MyToolCategory);
  ClangTool Tool(Op.getCompilations(), Op.getSourcePathList());

  MatchFinder Finder;
  MatchPrinter Printer;

  Finder.addMatcher(FunctionMatcher, &Printer);
  Finder.addMatcher(RecordMatcher, &Printer);

  Tool.run(newFrontendActionFactory(&Finder).get());

  // cout << "Functions" << endl;
  // for (auto t = FunctionData.begin(); t != FunctionData.end(); ++t) {
  //   cout << "Function Level: " << t->first << endl;
  //   cout << "Function Name: " << t->second.fname << endl;
  //   cout << "Guard Cond: " << t->second.guardStmt() << endl;
  //   map<string, CallExpression> calls_self = t->second.selfcall;
  //   cout << "Self Calls" << endl;
  //   for (auto it = calls_self.begin(); it != calls_self.end(); ++it) {
  //     cout << it->first << ": " << it->second.to_string() << endl;
  //   }
  //   cout << "Other Calls" << endl;
  //   map<string, CallExpression> calls_other = t->second.othercall;
  //   for (auto it = calls_other.begin(); it != calls_other.end(); ++it) {
  //     cout << it->first << ": " << it->second.to_string() << endl;
  //   }
  // }
  if (!opts::mode) {
    for (auto t = FunctionData.begin(); t != FunctionData.end(); ++t) {
      cout << "Function Level: " << t->first << endl;
      t->second.PrintStmts(true);
    }
  }

  if (opts::mode) {
    map<int, string> ord;
    ord[0] = "g2";
    ord[1] = "s2";
    ord[2] = "r2l";
    ord[3] = "r2r";

    code_motion(2, ord);
    interchange(1);

    cout << "\nTransformation" << endl;
    for (auto t = FunctionData.begin(); t != FunctionData.end(); ++t) {
      cout << "Function Level: " << t->first << endl;
      t->second.PrintStmts(false);
    }
  }

  return 0;
}