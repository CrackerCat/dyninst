/*
 * Copyright (c) 1996-2011 Barton P. Miller
 * 
 * We provide the Paradyn Parallel Performance Tools (below
 * described as "Paradyn") on an AS IS basis, and do not warrant its
 * validity or performance.  We reserve the right to update, modify,
 * or discontinue this software at any time.  We shall have no
 * obligation to supply such updates or modifications or any other
 * form of support to you.
 * 
 * By your use of Paradyn, you understand and agree that we (or any
 * other person or entity with proprietary rights in Paradyn) are
 * under no obligation to provide either maintenance services,
 * update services, notices of latent defects, or correction of
 * defects for Paradyn.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef _FCUseDominator_h_
#define _FCUseDominator_h_

#include <FunctionCoverage.h>

/** class to represent a function in the executable
  * whose source code line information is available.
  * This class is introduced for instrumentation of the function 
  * using the dominator information of its control flow graph
  * for code coverage.
  */
class FCUseDominator : public FunctionCoverage {
private:
	/** private constructor of the class */
	FCUseDominator();

	/** method that fill the dominator information
	  * of the control flow graph it contains
	  */
	void fillDominatorInfo();

	/** method to decide which basic block will
	  * be instrumented when the function is instrumented
	  * for code coverage. A basic block is validated
	  * if it is a leaf in dominator tree or has an 
	  * outgoing edge to a basic block it does not dominate.
	  * @param bb basic block pointer
	  */
	bool validateBasicBlock(BPatch_basicBlock* bb);

	/** if interval deletion is given to be >= 0
	  * then in fixed intervals the mutatee is stopped
	  * and the execution counts of the basic blocks are
	  * updated for all blocks. This method
	  * updates the execution count of the basic block
	  * with the given count using the dominator information.
	  * @param bb basic block whose execution count will be
	  *        updated
	  * @param ec number of executions to add
	  */
	int updateExecutionCounts(BPatch_basicBlock* bb,int ec);

public:

	/** constructor of the class
	  * @param f function to instrument later
	  * @param t thread object of the mutatee
	  * @param i image that the function belongs to
	  * @param funcN the name of the function
	  * @param fileN the name of the source file the function is in
	  */
	FCUseDominator(BPatch_function* f,BPatch_thread* t,BPatch_image* i,
		       const char* funcN);

	/** destructor of the class */
	~FCUseDominator();
};

#endif /* _FCUseDominator_h_ */
