//===--- ConstCorrectnessCheck.h - clang-tidy--------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CPPCOREGUIDELINES_CONSTCORRECTNESSCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CPPCOREGUIDELINES_CONSTCORRECTNESSCHECK_H

#include "../ClangTidy.h"
#include "clang/Analysis/Analyses/ExprMutationAnalyzer.h"

namespace clang {
namespace tidy {

namespace cppcoreguidelines {

/// This check warns on variables which could be declared const but are not.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/cppcoreguidelines-const.html
class ConstCorrectnessCheck : public ClangTidyCheck {
public:
  ConstCorrectnessCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context),
        AnalyzeValues(Options.get("AnalyzeValues", 1)),
        AnalyzeReferences(Options.get("AnalyzeReferences", 1)),
        WarnPointersAsValues(Options.get("WarnPointersAsValues", 0)) {}

  void storeOptions(ClangTidyOptions::OptionMap &Opts) override;
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  void registerScope(const CompoundStmt *LocalScope, ASTContext *Context);

  using MutationAnalyzer = std::unique_ptr<ExprMutationAnalyzer>;
  llvm::DenseMap<const CompoundStmt *, MutationAnalyzer> ScopesCache;

  const bool AnalyzeValues;
  const bool AnalyzeReferences;
  const bool WarnPointersAsValues;
};

} // namespace cppcoreguidelines
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CPPCOREGUIDELINES_CONSTCORRECTNESSCHECK_H
