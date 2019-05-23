//===--- ExceptionAnalyzer.cpp - clang-tidy -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ExceptionAnalyzer.h"

namespace clang {
namespace tidy {
namespace utils {

void ExceptionAnalyzer::ExceptionInfo::registerException(
    const Type *ExceptionType) {
  assert(ExceptionType != nullptr && "Only valid types are accepted");
  Behaviour = State::Throwing;
  ThrownExceptions.insert(ExceptionType);
}

void ExceptionAnalyzer::ExceptionInfo::registerExceptions(
    const Throwables &Exceptions) {
  if (Exceptions.size() == 0)
    return;
  Behaviour = State::Throwing;
  ThrownExceptions.insert(Exceptions.begin(), Exceptions.end());
}

ExceptionAnalyzer::ExceptionInfo &ExceptionAnalyzer::ExceptionInfo::merge(
    const ExceptionAnalyzer::ExceptionInfo &Other) {
  // Only the following two cases require an update to the local
  // 'Behaviour'. If the local entity is already throwing there will be no
  // change and if the other entity is throwing the merged entity will throw
  // as well.
  // If one of both entities is 'Unknown' and the other one does not throw
  // the merged entity is 'Unknown' as well.
  if (Other.Behaviour == State::Throwing)
    Behaviour = State::Throwing;
  else if (Other.Behaviour == State::Unknown && Behaviour == State::NotThrowing)
    Behaviour = State::Unknown;

  ContainsUnknown = ContainsUnknown || Other.ContainsUnknown;
  ThrownExceptions.insert(Other.ThrownExceptions.begin(),
                          Other.ThrownExceptions.end());
  return *this;
}

static bool isBaseOf(const Type *DerivedType, const Type *BaseType) {
  const auto *DerivedClass = DerivedType->getAsCXXRecordDecl();
  const auto *BaseClass = BaseType->getAsCXXRecordDecl();
  if (!DerivedClass || !BaseClass)
    return false;

  return !DerivedClass->forallBases(
      [BaseClass](const CXXRecordDecl *Cur) { return Cur != BaseClass; });
}

bool ExceptionAnalyzer::ExceptionInfo::filterByCatch(const Type *BaseClass) {
  llvm::SmallVector<const Type *, 8> TypesToDelete;
  for (const Type *T : ThrownExceptions) {
    if (T == BaseClass || isBaseOf(T, BaseClass))
      TypesToDelete.push_back(T);
  }

  for (const Type *T : TypesToDelete)
    ThrownExceptions.erase(T);

  reevaluateBehaviour();
  return TypesToDelete.size() > 0;
}

ExceptionAnalyzer::ExceptionInfo &
ExceptionAnalyzer::ExceptionInfo::filterIgnoredExceptions(
    const llvm::StringSet<> &IgnoredTypes, bool IgnoreBadAlloc) {
  llvm::SmallVector<const Type *, 8> TypesToDelete;
  // Note: Using a 'SmallSet' with 'llvm::remove_if()' is not possible.
  // Therefore this slightly hacky implementation is required.
  for (const Type *T : ThrownExceptions) {
    if (const auto *TD = T->getAsTagDecl()) {
      if (TD->getDeclName().isIdentifier()) {
        if ((IgnoreBadAlloc &&
             (TD->getName() == "bad_alloc" && TD->isInStdNamespace())) ||
            (IgnoredTypes.count(TD->getName()) > 0))
          TypesToDelete.push_back(T);
      }
    }
  }
  for (const Type *T : TypesToDelete)
    ThrownExceptions.erase(T);

  reevaluateBehaviour();
  return *this;
}

void ExceptionAnalyzer::ExceptionInfo::clear() {
  Behaviour = State::NotThrowing;
  ContainsUnknown = false;
  ThrownExceptions.clear();
}

void ExceptionAnalyzer::ExceptionInfo::reevaluateBehaviour() {
  if (ThrownExceptions.size() == 0)
    if (ContainsUnknown)
      Behaviour = State::Unknown;
    else
      Behaviour = State::NotThrowing;
  else
    Behaviour = State::Throwing;
}

ExceptionAnalyzer::ExceptionInfo ExceptionAnalyzer::throwsException(
    const FunctionDecl *Func,
    llvm::SmallSet<const FunctionDecl *, 32> &CallStack) {
  // This function has already been analyzed. Therefore the neutral element
  // is returned.
  if (CallStack.count(Func) != 0)
    return ExceptionInfo::createNotThrowing();

  // The function body is available in the source-code and is analyzed directly.
  if (const Stmt *Body = Func->getBody()) {
    CallStack.insert(Func);
    ExceptionInfo Result =
        throwsException(Body, ExceptionInfo::Throwables(), CallStack);
    CallStack.erase(Func);
    return Result;
  }

  // The exception behaviour must be infered by the declaration of the function.
  auto Result = ExceptionInfo::createNotThrowing();
  if (const auto *FPT = Func->getType()->getAs<FunctionProtoType>()) {
    // Dynamic exception specifier states the exceptions that can be thrown.
    for (const QualType Ex : FPT->exceptions())
      Result.registerException(Ex.getTypePtr());

    switch (FPT->getExceptionSpecType()) {
    // The function declaration does not give any information about the
    // exception behaviour and must be considered as 'Unknown'.
    case EST_None:
      Result.signalUnknown();
      break;
    // The 'FunctionProtoType' signals that throwing is expected.
    case EST_NoexceptFalse:
    case EST_MSAny:
      Result.signalThrowing();
      llvm::dbgs() << "Throwing expected!\n";
      llvm::dbgs() << Func->getName() << "\n";
      break;
      // The potential exception list in 'throw()' is handled above.
      // All other possibilities are not considered here.
    default:
      break;
    }
    // What's left here is that the function does state to not throw
    // (e.g. 'void foo() noexcept') which is covered with the initial value of
    // 'Result'.
  }
  return Result;
}

/// Analyzes a single statment on it's throwing behaviour. This is in principle
/// possible except some 'Unknown' functions are called.
ExceptionAnalyzer::ExceptionInfo ExceptionAnalyzer::throwsException(
    const Stmt *St, const ExceptionInfo::Throwables &Caught,
    llvm::SmallSet<const FunctionDecl *, 32> &CallStack) {
  auto Results = ExceptionInfo::createNotThrowing();
  if (!St)
    return Results;

  if (const auto *Throw = dyn_cast<CXXThrowExpr>(St)) {
    if (const auto *ThrownExpr = Throw->getSubExpr()) {
      const auto *ThrownType =
          ThrownExpr->getType()->getUnqualifiedDesugaredType();
      if (ThrownType->isReferenceType())
        ThrownType = ThrownType->castAs<ReferenceType>()
                         ->getPointeeType()
                         ->getUnqualifiedDesugaredType();
      Results.registerException(
          ThrownExpr->getType()->getUnqualifiedDesugaredType());
    } else
      // A rethrow of a caught exception happens which makes it possible
      // to throw all exception that are caught in the 'catch' clause of
      // the parent try-catch block.
      Results.registerExceptions(Caught);
  } else if (const auto *Try = dyn_cast<CXXTryStmt>(St)) {
    ExceptionInfo Uncaught =
        throwsException(Try->getTryBlock(), Caught, CallStack);
    for (unsigned i = 0; i < Try->getNumHandlers(); ++i) {
      const CXXCatchStmt *Catch = Try->getHandler(i);

      // Everything is catched through 'catch(...)'.
      if (!Catch->getExceptionDecl()) {
        ExceptionInfo Rethrown = throwsException(
            Catch->getHandlerBlock(), Uncaught.getExceptionTypes(), CallStack);
        Results.merge(Rethrown);
        Uncaught.clear();
      } else {
        const auto *CaughtType =
            Catch->getCaughtType()->getUnqualifiedDesugaredType();
        if (CaughtType->isReferenceType())
          CaughtType = CaughtType->castAs<ReferenceType>()
                           ->getPointeeType()
                           ->getUnqualifiedDesugaredType();

        // If the caught exception will catch multiple previously potential
        // thrown types (because it's sensitive to inheritance) the throwing
        // situation changes. First of all filter the exception types and
        // analyze if the baseclass-exception is rethrown.
        if (Uncaught.filterByCatch(CaughtType)) {
          ExceptionInfo::Throwables CaughtExceptions;
          CaughtExceptions.insert(CaughtType);
          ExceptionInfo Rethrown = throwsException(Catch->getHandlerBlock(),
                                                   CaughtExceptions, CallStack);
          Results.merge(Rethrown);
        }
      }
    }
    Results.merge(Uncaught);
  } else if (const auto *Call = dyn_cast<CallExpr>(St)) {
    if (const FunctionDecl *Func = Call->getDirectCallee()) {
      ExceptionInfo Excs = throwsException(Func, CallStack);
      Results.merge(Excs);
    }
  } else {
    for (const Stmt *Child : St->children()) {
      ExceptionInfo Excs = throwsException(Child, Caught, CallStack);
      Results.merge(Excs);
    }
  }
  return Results;
}

ExceptionAnalyzer::ExceptionInfo
ExceptionAnalyzer::analyzeImpl(const FunctionDecl *Func) {
  ExceptionInfo ExceptionList;
  // Check if the function has already been analyzed and reuse that result.
  if (FunctionCache.count(Func) == 0) {
    llvm::SmallSet<const FunctionDecl *, 32> CallStack;
    ExceptionList = throwsException(Func, CallStack);

    // Cache the result of the analysis. This is done prior to filtering
    // because it is best to keep as much information as possible.
    // The results here might be relevant to different analysis passes
    // with different needs as well.
    FunctionCache.insert(std::make_pair(Func, ExceptionList));
  } else
    ExceptionList = FunctionCache[Func];

  return ExceptionList;
}

ExceptionAnalyzer::ExceptionInfo
ExceptionAnalyzer::analyzeImpl(const Stmt *Stmt) {
  llvm::SmallSet<const FunctionDecl *, 32> CallStack;
  return throwsException(Stmt, ExceptionInfo::Throwables(), CallStack);
}

template <typename T>
ExceptionAnalyzer::ExceptionInfo
ExceptionAnalyzer::analyzeDispatch(const T *Node) {
  ExceptionInfo ExceptionList = analyzeImpl(Node);

  if (ExceptionList.getBehaviour() == State::NotThrowing ||
      ExceptionList.getBehaviour() == State::Unknown)
    return ExceptionList;

  // Remove all ignored exceptions from the list of exceptions that can be
  // thrown.
  ExceptionList.filterIgnoredExceptions(IgnoredExceptions, IgnoreBadAlloc);

  return ExceptionList;
}

ExceptionAnalyzer::ExceptionInfo
ExceptionAnalyzer::analyze(const FunctionDecl *Func) {
  return analyzeDispatch(Func);
}

ExceptionAnalyzer::ExceptionInfo
ExceptionAnalyzer::analyze(const Stmt *Stmt) {
  return analyzeDispatch(Stmt);
}

} // namespace utils
} // namespace tidy

} // namespace clang
