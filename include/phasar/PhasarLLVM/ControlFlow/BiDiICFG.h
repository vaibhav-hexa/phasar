/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

/*
 * BiDiICFG.h
 *
 *  Created on: 15.09.2016
 *      Author: pdschbrt
 */

#ifndef ANALYSIS_IFDS_IDE_BIDIICFG_H_
#define ANALYSIS_IFDS_IDE_BIDIICFG_H_

#include "ICFG.h"
#include <vector>

using namespace std;
namespace psr {

template <typename N, typename M> class BiDiICFG : public ICFG<N, M> {
public:
  virtual ~BiDiICFG() = default;

  virtual vector<N> getPredsOf(N u) = 0;

  virtual set<N> getEndPointsOf(M m) = 0;

  virtual vector<N> getPredsOfCallAt(N u) = 0;

  virtual set<N> allNonCallEndNodes() = 0;

  // also exposed to some clients who need it
  // virtual DirectedGraph<N> getOrCreateUnitGraph(M body) = 0;

  virtual vector<N> getParameterRefs(M m) = 0;

  /**
   * Gets whether the given statement is a return site of at least one call
   * @param n The statement to check
   * @return True if the given statement is a return site, otherwise false
   */
  virtual bool isReturnSite(N n) = 0;

  /**
   * Checks whether the given statement is reachable from the entry point
   * @param u The statement to check
   * @return True if there is a control flow path from the entry point of the
   * program to the given statement, otherwise false
   */
  virtual bool isReachable(N u) = 0;
};

} // namespace psr

#endif /* ANALYSIS_IFDS_IDE_BIDIICFG_HH_ */
