#ifndef ANNOTATIONS_H_
#define ANNOTATIONS_H_

#include "Common.h"

bool hasNestedAnnotation(const FunctionDecl *FD); 
int getNestedLevel(const FunctionDecl *FD);
bool isLoop(const FunctionDecl *FD); 
bool hasRecursiveDSAnnotation(const CXXRecordDecl *DS); 
bool hasRecursiveFLAnnoatation(const FieldDecl *FL); 
bool hasIndVarAnnotation(const ParmVarDecl *P); 

#endif
