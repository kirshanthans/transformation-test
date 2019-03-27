#include "Annotations.h"

bool hasNestedAnnotation(const FunctionDecl *FD) {
  if (FD->hasAttr<AnnotateAttr>()){
         stringstream ss(FD->getAttr<AnnotateAttr>()->getAnnotation().str());
         vector<string> splits;
         string item;
         while(getline(ss, item, '|')){
                splits.push_back(item);
         }
         return "nested" == splits[0];
  }
  return false;
}

int getNestedLevel(const FunctionDecl *FD) {
  if (FD->hasAttr<AnnotateAttr>()){
         stringstream ss(FD->getAttr<AnnotateAttr>()->getAnnotation().str());
         vector<string> splits;
         string item;
         while(getline(ss, item, '|')){
                splits.push_back(item);
         }
         if ("nested" == splits[0]){
                return stoi(splits[1]);
         }
  }
  return -1;
}

bool isLoop(const FunctionDecl *FD) {
  if (FD->hasAttr<AnnotateAttr>()){
         stringstream ss(FD->getAttr<AnnotateAttr>()->getAnnotation().str());
         vector<string> splits;
         string item;
         while(getline(ss, item, '|')){
                splits.push_back(item);
         }
         if ("nested" == splits[0]){
                return "lp" == splits[2];
         }
  }
  return false;
}

bool hasRecursiveDSAnnotation(const CXXRecordDecl *DS) {
  return DS->hasAttr<AnnotateAttr>() &&
         DS->getAttr<AnnotateAttr>()->getAnnotation()
         .str()
         .compare("recursive_ds") == 0;
}

bool hasRecursiveFLAnnoatation(const FieldDecl *FL) {
  return FL->hasAttr<AnnotateAttr>() &&
         FL->getAttr<AnnotateAttr>()->getAnnotation()
         .str()
         .compare("recursive_fl") == 0;
}

bool hasIndVarAnnotation(const ParmVarDecl *P) {
    return P->hasAttr<AnnotateAttr>() && 
           P->getAttr<AnnotateAttr>()->getAnnotation()
           .str()
           .compare("ind_var") == 0;
}