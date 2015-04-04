//===------- SourceSinkAnalysis.h -- Identify taint sources and sinks ----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares a pass to identify source and sink values in programs.
//
//===----------------------------------------------------------------------===//

#ifndef SOURCESINKANALYSIS_H_
#define SOURCESINKANALYSIS_H_

#include "llvm/Pass.h"
#include "llvm/Support/CallSite.h"
#include "FlowRecord.h"

#include <string>
#include <set>

using namespace llvm;

namespace deps {

class SourceSinkAnalysis : public ModulePass {
public:
  static char ID;

  SourceSinkAnalysis() : ModulePass(ID) {}

  void visitCallInst(CallInst &CI);

  virtual bool runOnModule(Module &M);

  virtual void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.setPreservesAll();
  }

  // Moved over from MISO so we can get at it from Infoflow...
  const FlowRecord & getSourcesAndSinks() const;

  // Accessors for infoflow to get at sinks quickly
  bool valueIsSink(const Value &) const;
  bool vargIsSink(const Function &) const;
  bool directPtrIsSink(const Value &) const;
  bool reachPtrIsSink(const Value &) const;

  //
  // Determine the source taint information for an external function call.
  //
  // Inputs
  //  CS is the call site to analyze.
  //  TaintedValues will be filled with all values that should be considered
  //   taint sources after the call occurs.
  //  TaintedDirectPointers will be filled with all pointers only whose
  //   directly reachable memory locations should be considered taint sources
  //   after the call occurs.
  //  TaintedRootPointers will be filled with all pointers whose reachable
  //   memory locations should be considered taint sources after the call
  //   occurs.
  //
  void identifySourcesForCallSite(
    const CallSite &CS,
    std::set<const Value *> &TaintedValues,
    std::set<const Value *> &TaintedDirectPointers,
    std::set<const Value *> &TaintedRootPointers
  );

  //
  // Determine the sink taint information for an external function call.
  //
  // Inputs
  //  CS is the call site to analyze.
  //  TaintedValues will be filled with all values that should be considered
  //   taint sinks during the call.
  //  TaintedDirectPointers will be filled with all pointers only whose
  //   directly reachable memory locations should be considered taint sinks
  //   during the call.
  //  TaintedRootPointers will be filled with all pointers whose reachable
  //   memory locations should be considered taint sinks during the call.
  //
  void identifySinksForCallSite(
    const CallSite &CS,
    std::set<const Value *> &TaintedValues,
    std::set<const Value *> &TaintedDirectPointers,
    std::set<const Value *> &TaintedRootPointers
  );

  //
  // Determine the source taint information for an internal function.
  //
  // NOTE: Currently only adds taint information for main
  //
  // Inputs
  //  F is the function to analyze.
  //  TaintedValues will be filled with all arguments that should be
  //   considered tainted sources during a call to the function.
  //  TaintedDirectPointers will be filled with all arguments that are
  //   pointers and only whose directly reachable memory locations should be
  //   considered a taint source during a call to the function.
  //  TaintedRootPointers will be filled with all arguments that are pointers
  //   and whose reachable memory should be considered a taint source during
  //   a call to the function.
  //
  void identifySourcesForFunction(
    const Function &F,
    std::set<const Value *> &TaintedValues,
    std::set<const Value *> &TaintedDirectPointers,
    std::set<const Value *> &TaintedRootPointers
  );

  private:
  FlowRecord sourcesAndSinks;
};

  //
  // Structure which records which arguments should be marked tainted.
  //
  typedef struct CallTaintSummary {
    static const unsigned NumArguments = 10;
    bool TaintsReturnValue;
    bool TaintsArgument[NumArguments];
    bool TaintsVarargArguments;
  } CallTaintSummary;
  
  //
  // Holds an entry for a single function, summarizing which arguments are
  // tainted values and which point to tainted memory.
  //
  typedef struct CallTaintEntry {
    const char *Name;
    CallTaintSummary ValueSummary;
    CallTaintSummary DirectPointerSummary;
    CallTaintSummary RootPointerSummary;
  } CallTaintEntry;
  
#define TAINTS_NOTHING \
{ false, { }, false }
#define TAINTS_ALL_ARGS { \
false, \
{ true, true, true, true, true, true, true, true, true, true }, \
true \
}
#define TAINTS_VARARGS \
{ false, { }, true }
#define TAINTS_RETURN_VAL \
{ true, { }, false }
  
#define TAINTS_ARG_1 \
{ false, { true }, false }
#define TAINTS_ARG_2 \
{ false, { false, true }, false }
#define TAINTS_ARG_3 \
{ false, { false, false, true }, false }
#define TAINTS_ARG_4 \
{ false, { false, false, false, true }, false }
  
#define TAINTS_ARG_1_2 \
{ false, { true, true, false }, false }
#define TAINTS_ARG_1_3 \
{ false, { true, false, true }, false }
#define TAINTS_ARG_1_4 \
{ false, { true, false, false, true }, false }
  
#define TAINTS_ARG_2_3 \
{ false, { false, true, true, false }, false }
#define TAINTS_ARG_3_4 \
{ false, { false, false, true, true }, false }
  
#define TAINTS_ARG_1_2_3 \
{ false, { true, true, true }, false }
  
#define TAINTS_ARG_1_AND_VARARGS \
{ false, { true }, true }
#define TAINTS_ARG_3_AND_RETURN_VAL \
{ true, { false, false, true }, false }

}

#endif
