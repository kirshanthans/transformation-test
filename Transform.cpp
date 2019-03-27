#include "Transform.h"
#include "Analysis.h"

void code_motion(int level, map<int, string> ord) {
    if (FunctionData.find(level) == FunctionData.end()){
        cout << "Level Error" << endl;
        return;
    }
    NestedFunction& F = FunctionData[level];
    F.order = ord;
    for(auto it = ord.begin(); it != ord.end(); ++it){
        F.orderinv[it->second] = it->first;
    }

}

void interchange(int level){

    NestedFunction F1 = FunctionData[level];
    NestedFunction F2 = FunctionData[level+1];
    auto name_l1 = F1.fname;
    auto name_l2 = F2.fname;
    for(auto it = F2.selfcall.begin(); it != F2.selfcall.end(); ++it) {
        auto& call = it->second;
        call.callee = name_l1;
    }
    
    for(auto it = F1.selfcall.begin(); it != F1.selfcall.end(); ++it) {
        auto& call = it->second;
        call.callee = name_l2;
    }

    if (F2.hasComputation){
        F2.hasComputation = false;
        string compute = F2.computation;
        string complabel2 = F2.compLabel;
        int ord_comp = F2.orderinv[complabel2];
        F2.order[ord_comp] = "t"+to_string(level);
        F2.othercall["t"+to_string(level)] = F1.othercall["t"+to_string(level)];
        F1.hasComputation = true;
        F1.computation = compute;
        F1.compLabel = "s" + to_string(level+1);
        int ord_transf = F1.orderinv["t"+to_string(level)];
        F1.order[ord_transf] = F1.compLabel;

    }
    
    FunctionData[level] = F2;
    FunctionData[level+1] = F1;

}