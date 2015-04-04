/*
 * Copyright (c) 2014, Columbia University
 * All rights reserved.
 *
 * This software was developed by (alphabetically)
 * Kangkook Jee <jikk@cs.columbia.edu>
 * Theofilos Petsios <theofilos@cs.columbia.edu>
 * Marios Pomonis <mpomonis@cs.columbia.edu>
 * at Columbia University, New York, NY, USA, in September 2014.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Columbia University nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef INFOAPP_H_
#define INFOAPP_H_

#include "llvm/Pass.h"
#include "llvm/PassManager.h"
#include "llvm/Module.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include "Infoflow.h"

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#define WHITE_LIST	"/tmp/whitelist.files"
#define MODE_FILE	"/tmp/mode"
#define WHITELISTING	1
#define BLACKLISTING	2
#define SENSITIVE		3
#define	BLACK_SENSITIVE	4
#define MODE_MAX_NUM	4

using namespace llvm;
using namespace deps;

namespace  {

class IntflowPass : public ModulePass {
 public:
  IntflowPass() : ModulePass(ID) {}
  static char ID;
  bool runOnModule(Module &M);

  virtual void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequired<Infoflow>();
    AU.setPreservesAll();
  }

 private:
  Infoflow* infoflow;
  uint64_t unique_id;
  std::string *iocIdName;
  DenseMap<const Value*, bool> xformMap;
  std::set<StringRef> whiteSet;
  std::set<StringRef> blackSet;
  unsigned char mode;

  virtual void doInitializationAndRun(Module &M);
  virtual void doFinalization();

  void runOnModuleWhitelisting(Module &M);
  void runOnModuleBlacklisting(Module &M);
  void runOnModuleSensitive(Module &M);
  void populateMapsSensitive(Module &M);
  void createArraysAndSensChecks(Module &M);
  void insertIOCChecks(Module &M);

  void insertIntFlowFunction(Module &M,
                             std::string name,
                             Instruction *ci,
                             BasicBlock::iterator ii,
                             GlobalVariable *g,
                             uint64_t totalIOC);

  GlobalVariable *createGlobalArray(Module &M,
                                    uint64_t size,
                                    std::string sinkKind);

  GlobalVariable *getGlobalArray(Module &M, std::string sinkKind);
  void addFunctions(Module &M, GlobalVariable * gl);
  /// Traverse instructions from the module(M) and identify tainted
  /// instructions.
  /// if it returns true: tag it to replace it with dummy
  ///       returns false: do not change

  bool trackSoln(Module &M,
                 InfoflowSolution* soln,
                 CallInst* sinkCI,
                 std::string& kinds);
  bool trackSolnInst(CallInst *i,
                     Module &M,
                     CallInst *ci,
                     InfoflowSolution* soln,
                     std::string& s);

  void backSensitiveArithm(Module &M,
                           CallInst *ci,
                           std::string std,
                           InfoflowSolution* soln);

  void backSensitiveInst(Function &F,
                         Module &M,
                         Instruction &i,
                         std::string std,
                         InfoflowSolution* soln);

  void searchSensFromArithm(Function &F,
                            Module &M,
                            std::string iocKind,
                            CallInst *ci);

  void searchSensFromInst(Function &F,
                          Module &M,
                          std::string iocKind,
                          Instruction &i);

  void handleStrictShift(std::string iocKind,
                         std::string sinkKind,
                         Function &F,
                         Module &M);

  void backwardSlicingBlacklisting(Module &M,
                                   InfoflowSolution* fsoln,
                                   CallInst* srcCI);

  void taintForward(std::string s,
                    CallInst *ci,
                    const CallTaintEntry *entry);

  void taintBackwards(std::string s,
                      CallInst *ci,
                      const CallTaintEntry *entry);

  InfoflowSolution *forwardSlicingBlacklisting(CallInst *ci,
                                               const CallTaintEntry *entry,
                                               uint64_t *id);

  InfoflowSolution *getForwardSolFromEntry(std::string s,
                                           CallInst *ci,
                                           const CallTaintEntry *entry);
  InfoflowSolution *getBackwardsSolFromEntry(std::string s,
                                             CallInst *ci,
                                             const CallTaintEntry *entry);
  InfoflowSolution *getForwardSol(std::string s, CallInst *ci);
  InfoflowSolution *getBackwardsSol(std::string s, CallInst *ci);
  InfoflowSolution *getBackSolArithm(std::string s, CallInst *ci);
  InfoflowSolution *getForwSolArithm(std::string s, CallInst *ci);
  InfoflowSolution *getForwSolConv(std::string s, CallInst *ci);
  InfoflowSolution *getBackSolConv(std::string s, CallInst *ci);

  void removeBenignChecks(Module &M);
  void checkfTainted(Module &M, InfoflowSolution *f);
  void setWrapper(CallInst *ci, Module &M, Function *f);

  bool ioc_report_all_but_conv(std::string s);
  bool ioc_report_all(std::string s);
  bool ioc_report_arithm(std::string s);
  bool ioc_report_shl(std::string s);
  bool llvm_arithm(std::string s);

  bool checkBackwardTainted(Value &V,
                            InfoflowSolution* soln,
                            bool direct=true);
  bool checkForwardTainted(Value &V,
                           InfoflowSolution* soln,
                           bool direct=true);
  bool checkForwardTaintedAny(CallInst *ci, InfoflowSolution* soln);

  bool isConstAssign(const std::set<const Value *> vMap);

  void removeChecksForFunction(Function& F, Module& M);
  void removeChecksInst(CallInst *i, unsigned int m, Module &M);
  void format_ioc_report_func(const Value* val, raw_string_ostream& rs);

  void getStringFromVal(Value* val, std::string& output);
  void getMode();
  uint64_t getIntFromVal(Value* val);
  uint64_t getColFromVal(Value* val);
  std::string getKindId(std::string name, uint64_t *id);
  std::string getKindCall(Module &M, Function &F, CallInst *ci);
  std::string getKindInst(Module &M, Function &F, Instruction &i);

};  //class

typedef  struct {
  char* func;
  char* fname;
  bool conversion;
  bool overflow;
  bool shift;
} rmChecks;

/* All IOC checks related with a sens Sink */
typedef std::vector <std::string> iocPointVector; //vector of ioc_checks
typedef std::map <std::string, iocPointVector> iocPointsForSens;

/* All sens sinks related with an IOC check */
typedef std::vector <std::string> sensPointVector; //vector of sens_sinks
typedef std::map <std::string, sensPointVector> sensPointsForIOC;

iocPointsForSens iocPoints;
sensPointsForIOC sensPoints;

void dbg_err(std::string s);
void dbg_msg(std::string s, std::string b);
}  // namespace

#endif
