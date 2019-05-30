/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

/*
 * LLVMZeroValue.h
 *
 *  Created on: 23.05.2017
 *      Author: philipp
 */

#ifndef PHASAR_PHASARLLVM_IFDSIDE_LLVMZEROVALUE_H_
#define PHASAR_PHASARLLVM_IFDSIDE_LLVMZEROVALUE_H_

#include <memory>
#include <string>

#include <llvm/IR/Constants.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

namespace llvm {
class Value;
}

namespace psr {

// do not touch, its only purpose is to make ZeroValue working
static constexpr char LLVMZeroValueInternalName[] = "zero_value";
static constexpr char LLVMZeroValueInternalModuleName[] = "zero_module";
static const std::unique_ptr<llvm::LLVMContext> LLVMZeroValueCTX(new llvm::LLVMContext);
static const std::unique_ptr<llvm::Module> LLVMZeroValueMod(
    new llvm::Module(LLVMZeroValueInternalModuleName, *LLVMZeroValueCTX));

/**
 * This function can be used to determine if a Value is a ZeroVale.
 * Of course
 *
 * 	llvm::isa<ZeroValue>(V)
 *
 * may be used, but isZeroValue() is much cheaper since it
 * does not have to traverse the class hierarchy to check this.
 */
inline bool isLLVMZeroValue(const llvm::Value *V) {
  if (V && V->hasName()) {
    // checks if V's name start with "zero_value"
    return V->getName().str().find(LLVMZeroValueInternalName) != std::string::npos;
  }
  return false;
}

/**
 * This class may be used to represent the special zero value for IFDS
 * and IDE problems. Instances of this class must be allocated with new!
 *
 * 	LLVMZeroValue *Z = new LLVMZeroValue;
 *
 * The ZeroValue class does the clean-up itself, there are no memory leaks
 * even when a user allocates with new!!! The corresponding LLVMContext and
 * Module will do the clean-up for the user. A user is not allowed to call
 * delete on an allocated LLVMZeroValue - it leads to misery and a double free
 * corruption! A LLVMZeroValue may be dumped using 'dump()' which shall print
 * something similar to
 *
 * 	@zero_value = constant i2 0, align 4
 *
 * It makes much sense to use ZeroValue as a singleton, but one is not
 * restricted to that. Allocating more than one LLVMZeroValue like
 *
 * 	LLVMZeroValue *Z = new LLVMZeroValue;
 *  LLVMZeroValue *X = new LLVMZeroValue;
 *	LLVMZeroValue *Y = new LLVMZeroValue;
 *
 * is allowed. In this case we can find such contents in memory
 *
 * 	@zero_value = constant i2 0, align 4
 *	@zero_value.1 = constant i2 0, align 4
 *	@zero_value.2 = constant i2 0, align 4
 */
class LLVMZeroValue : public llvm::GlobalVariable {
private:
  LLVMZeroValue()
      : llvm::GlobalVariable(*LLVMZeroValueMod,
                             llvm::Type::getIntNTy(*LLVMZeroValueCTX, 2), true,
                             llvm::GlobalValue::LinkageTypes::ExternalLinkage,
                             llvm::ConstantInt::get(*LLVMZeroValueCTX,
                                                    llvm::APInt(/*nbits*/ 2,
                                                                /*value*/ 0,
                                                                /*signed*/ true)),
                             LLVMZeroValueInternalName) {
    setAlignment(4);
  }

public:
  LLVMZeroValue(const LLVMZeroValue &Z) = delete;
  LLVMZeroValue &operator=(const LLVMZeroValue &Z) = delete;
  LLVMZeroValue(LLVMZeroValue &&Z) = delete;
  LLVMZeroValue &operator=(LLVMZeroValue &&Z) = delete;
  // Do not specify a destructor (at all)!
  static LLVMZeroValue *getInstance() {
    static LLVMZeroValue *zv = new LLVMZeroValue;
    return zv;
  }
};
} // namespace psr

#endif
