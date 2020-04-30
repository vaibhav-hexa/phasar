/******************************************************************************
 * Copyright (c) 2020 Fabian Schiebel.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Fabian Schiebel and others
 *****************************************************************************/

#ifndef PHASAR_PHASARLLVM_IFDSIDE_PROBLEMS_IDEGENERALIZEDLCA_IDENTITYEDGEFUNCTION_H_
#define PHASAR_PHASARLLVM_IFDSIDE_PROBLEMS_IDEGENERALIZEDLCA_IDENTITYEDGEFUNCTION_H_

#include "phasar/PhasarLLVM/DataFlowSolver/IfdsIde/EdgeFunction.h"
#include "phasar/PhasarLLVM/DataFlowSolver/IfdsIde/EdgeFunctions/EdgeIdentity.h"
#include "phasar/PhasarLLVM/DataFlowSolver/IfdsIde/Problems/IDEGeneralizedLCA/IDEGeneralizedLCA.h"

namespace psr {

class IdentityEdgeFunction
    : public EdgeFunction<IDEGeneralizedLCA::l_t>,
      public std::enable_shared_from_this<IdentityEdgeFunction> {
  size_t maxSize;

public:
  IdentityEdgeFunction(size_t maxSize);
  IDEGeneralizedLCA::l_t computeTarget(IDEGeneralizedLCA::l_t source) override;

  std::shared_ptr<EdgeFunction<IDEGeneralizedLCA::l_t>> composeWith(
      std::shared_ptr<EdgeFunction<IDEGeneralizedLCA::l_t>> secondFunction)
      override;

  std::shared_ptr<EdgeFunction<IDEGeneralizedLCA::l_t>>
  joinWith(std::shared_ptr<EdgeFunction<IDEGeneralizedLCA::l_t>> otherFunction)
      override;

  bool equal_to(std::shared_ptr<EdgeFunction<IDEGeneralizedLCA::l_t>> other)
      const override;
  void print(std::ostream &OS, bool isForDebug = false) const override;
  static std::shared_ptr<IdentityEdgeFunction> getInstance(size_t maxSize);
};

// typedef EdgeIdentity<IDEGeneralizedLCA::l_t>
//   IdentityEdgeFunction;

} // namespace psr

#endif
