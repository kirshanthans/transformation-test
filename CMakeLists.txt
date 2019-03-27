set(LLVM_LINK_COMPONENTS
  Support
  )

add_clang_executable(test-tool
  TestTool.cpp
  Utils.cpp
  Annotations.cpp
  Analysis.cpp
  Transform.cpp
  Codegen.cpp
  )

target_link_libraries(test-tool
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangTooling
  clangToolingRefactor
  )
