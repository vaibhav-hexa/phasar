/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

/*
 * KillAll.h
 *
 *  Created on: 04.08.2016
 *      Author: pdschbrt
 */

#ifndef ANALYSIS_IFDS_IDE_FLOW_FUNC_KILLALL_H_
#define ANALYSIS_IFDS_IDE_FLOW_FUNC_KILLALL_H_

#include <memory>
#include <phasar/PhasarLLVM/IfdsIde/FlowFunction.h>
#include <set>

using namespace std;
namespace psr {

template <typename D> class KillAll : public FlowFunction<D> {
private:
  KillAll() = default;

public:
  virtual ~KillAll() = default;
  KillAll(const KillAll &k) = delete;
  KillAll &operator=(const KillAll &k) = delete;
  set<D> computeTargets(D source) override { return set<D>(); }
  static shared_ptr<KillAll<D>> getInstance() {
    static shared_ptr<KillAll> instance = shared_ptr<KillAll>(new KillAll);
    return instance;
  }
};
} // namespace psr

#endif /* ANALYSIS_IFDS_IDE_FLOW_FUNC_KILLALL_HH_ */
