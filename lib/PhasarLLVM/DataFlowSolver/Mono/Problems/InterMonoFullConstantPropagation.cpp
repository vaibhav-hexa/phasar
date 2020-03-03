/******************************************************************************
 * Copyright (c) 2020 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert, Linus Jungemann and others
 *****************************************************************************/

#include <algorithm>
#include <llvm/Support/Casting.h>
#include <ostream>

#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>

#include <phasar/DB/ProjectIRDB.h>
#include <phasar/PhasarLLVM/ControlFlow/LLVMBasedICFG.h>
#include <phasar/PhasarLLVM/DataFlowSolver/Mono/Problems/InterMonoFullConstantPropagation.h>
#include <phasar/PhasarLLVM/Pointer/LLVMPointsToInfo.h>
#include <phasar/PhasarLLVM/TypeHierarchy/LLVMTypeHierarchy.h>
#include <phasar/Utils/BitVectorSet.h>
#include <phasar/Utils/LLVMShorthands.h>
#include <unordered_map>

using namespace std;
using namespace psr;

namespace psr {

InterMonoFullConstantPropagation::InterMonoFullConstantPropagation(
    const ProjectIRDB *IRDB, const LLVMTypeHierarchy *TH,
    const LLVMBasedICFG *ICF, const LLVMPointsToInfo *PT,
    std::set<std::string> EntryPoints)
    : InterMonoProblem<InterMonoFullConstantPropagation::n_t,
                       InterMonoFullConstantPropagation::d_t,
                       InterMonoFullConstantPropagation::f_t,
                       InterMonoFullConstantPropagation::t_t,
                       InterMonoFullConstantPropagation::v_t,
                       InterMonoFullConstantPropagation::i_t>(IRDB, TH, ICF, PT,
                                                              EntryPoints) {}

BitVectorSet<InterMonoFullConstantPropagation::d_t>
InterMonoFullConstantPropagation::join(
    const BitVectorSet<InterMonoFullConstantPropagation::d_t> &Lhs,
    const BitVectorSet<InterMonoFullConstantPropagation::d_t> &Rhs) {
  return Lhs.setIntersect(Rhs);
}

bool InterMonoFullConstantPropagation::sqSubSetEqual(
    const BitVectorSet<InterMonoFullConstantPropagation::d_t> &Lhs,
    const BitVectorSet<InterMonoFullConstantPropagation::d_t> &Rhs) {
  return Lhs.includes(Rhs);
}

std::unordered_map<InterMonoFullConstantPropagation::n_t,
                   BitVectorSet<InterMonoFullConstantPropagation::d_t>>
InterMonoFullConstantPropagation::initialSeeds() {
  std::unordered_map<InterMonoFullConstantPropagation::n_t,
                     BitVectorSet<InterMonoFullConstantPropagation::d_t>>
      Seeds;
  for (auto &EntryPoint : EntryPoints) {
    if (auto Fun = IRDB->getFunctionDefinition(EntryPoint)) {
      auto Is = ICF->getStartPointsOf(Fun);
      for (auto I : Is) {
        Seeds[I] = {};
      }
    }
  }
  return Seeds;
}

BitVectorSet<InterMonoFullConstantPropagation::d_t>
InterMonoFullConstantPropagation::normalFlow(
    InterMonoFullConstantPropagation::n_t S,
    const BitVectorSet<InterMonoFullConstantPropagation::d_t> &In) {
  // TODO finish implementation
  auto Out = In;
  if (auto Alloc = llvm::dyn_cast<llvm::AllocaInst>(S)) {
    if (Alloc->getAllocatedType()->isIntegerTy()) {
      Out.insert({Alloc, Top{}});
    }
  }
  if (auto Store = llvm::dyn_cast<llvm::StoreInst>(S)) {
    auto ValueOp = Store->getValueOperand();
    // Case I: Integer literal
    if (auto val = llvm::dyn_cast<llvm::ConstantInt>(ValueOp)) {
      for (auto elem : In.getAsSet()) {
        if (elem.first == Store) {
          if (std::holds_alternative<Bottom>(elem.second)) {
            break;
          }
          Out.erase(elem);
          Out.insert({Store, val->getSExtValue()});
          break;
        }
      }
    }
    // Case II: Storing an integer typed value
    if (ValueOp->getType()->isIntegerTy()) {
      LatticeDomain<int64_t> latticeVal;
      for (auto elem : In.getAsSet()) {
        if (elem.first == ValueOp) {
          latticeVal = elem.second;
          break;
        }
      }
      if (!std::holds_alternative<Top>(latticeVal)){
        for (auto elem : In.getAsSet()) {
        if (elem.first == Store) {
          if (std::holds_alternative<Bottom>(elem.second)) {
            break;
          }
          Out.erase(elem);
          Out.insert({Store, latticeVal});
          break;
        }
      }
      }
    }
  }
  for (auto x : Out.getAsSet()){
    printDataFlowFact(std::cout,x);
  }
  return Out;
}

BitVectorSet<InterMonoFullConstantPropagation::d_t>
InterMonoFullConstantPropagation::callFlow(
    InterMonoFullConstantPropagation::n_t CallSite,
    InterMonoFullConstantPropagation::f_t Callee,
    const BitVectorSet<InterMonoFullConstantPropagation::d_t> &In) {
  // TODO implement
  return In;
}

BitVectorSet<InterMonoFullConstantPropagation::d_t>
InterMonoFullConstantPropagation::returnFlow(
    InterMonoFullConstantPropagation::n_t CallSite,
    InterMonoFullConstantPropagation::f_t Callee,
    InterMonoFullConstantPropagation::n_t ExitStmt,
    InterMonoFullConstantPropagation::n_t RetSite,
    const BitVectorSet<InterMonoFullConstantPropagation::d_t> &In) {
  // TODO implement
  return In;
}

BitVectorSet<InterMonoFullConstantPropagation::d_t>
InterMonoFullConstantPropagation::callToRetFlow(
    InterMonoFullConstantPropagation::n_t CallSite,
    InterMonoFullConstantPropagation::n_t RetSite,
    std::set<InterMonoFullConstantPropagation::f_t> Callees,
    const BitVectorSet<InterMonoFullConstantPropagation::d_t> &In) {
  // TODO implement
  return In;
}

void InterMonoFullConstantPropagation::printNode(
    std::ostream &os, InterMonoFullConstantPropagation::n_t n) const {
  os << llvmIRToString(n);
}

void InterMonoFullConstantPropagation::printDataFlowFact(
    std::ostream &os, InterMonoFullConstantPropagation::d_t d) const {
  os << "< " + llvmIRToString(d.first) << ", ";
  if (std::holds_alternative<Top>(d.second)) {
    os << std::get<Top>(d.second);
  }
  if (std::holds_alternative<Bottom>(d.second)) {
    os << std::get<Bottom>(d.second);
  }
  if (std::holds_alternative<InterMonoFullConstantPropagation::plain_d_t>(
          d.second)) {
    os << std::get<InterMonoFullConstantPropagation::plain_d_t>(d.second);
  }
  os << " >\n";
}

void InterMonoFullConstantPropagation::printFunction(
    std::ostream &os, InterMonoFullConstantPropagation::f_t f) const {
  os << f->getName().str();
}

} // namespace psr