/*
 * Copyright (c) 1996-2004 Barton P. Miller
 * 
 * We provide the Paradyn Parallel Performance Tools (below
 * described as "Paradyn") on an AS IS basis, and do not warrant its
 * validity or performance.  We reserve the right to update, modify,
 * or discontinue this software at any time.  We shall have no
 * obligation to supply such updates or modifications or any other
 * form of support to you.
 * 
 * This license is for research uses.  For such uses, there is no
 * charge. We define "research use" to mean you may freely use it
 * inside your organization for whatever purposes you see fit. But you
 * may not re-distribute Paradyn or parts of Paradyn, in any form
 * source or binary (including derivatives), electronic or otherwise,
 * to any other organization or entity without our permission.
 * 
 * (for other uses, please contact us at paradyn@cs.wisc.edu)
 * 
 * All warranties, including without limitation, any warranty of
 * merchantability or fitness for a particular purpose, are hereby
 * excluded.
 * 
 * By your use of Paradyn, you understand and agree that we (or any
 * other person or entity with proprietary rights in Paradyn) are
 * under no obligation to provide either maintenance services,
 * update services, notices of latent defects, or correction of
 * defects for Paradyn.
 * 
 * Even if advised of the possibility of such damages, under no
 * circumstances shall we (or any other person or entity with
 * proprietary rights in the software licensed hereunder) be liable
 * to you or any third party for direct, indirect, or consequential
 * damages of any character regardless of type of action, including,
 * without limitation, loss of profits, loss of use, loss of good
 * will, or computer failure or malfunction.  You agree to indemnify
 * us (and any other person or entity with proprietary rights in the
 * software licensed hereunder) for any and all liability it may
 * incur to third parties resulting from your use of Paradyn.
 */

// $Id: instPoint-mips.h,v 1.20 2005/02/02 17:27:26 bernat Exp $
// MIPS-specific definition of class instPoint

#ifndef _INST_POINT_MIPS_H_
#define _INST_POINT_MIPS_H_

#include <stdio.h>
#include "common/h/Types.h" // Address
#include "arch-mips.h"    // instruction
#include "function.h"
#include "symtab.h"

class process;
class image;

#ifdef BPATCH_LIBRARY
class BPatch_point;
#endif

typedef Address Offset;

/* instPoint flag bits */
#define IP_Relocated       (0x00000001)
#define IP_Overlap         (0x00000002)
#define IP_RecursiveBranch (0x80000000)

class instPoint : public instPointBase {
 public:
  instPoint(int_function *fn, Offset offset, instPointType ipt, unsigned info) :
    instPointBase(ipt, (fn->getAddress(0) + offset), fn),
    flags(info),
    vectorId(-1),
    hint_got_(0),
    owner_(fn->pdmod()->exec()),
    offset_(offset), 
    size_(2*INSN_SIZE),
    origOffset_(offset)
    {
      origInsn_.raw = owner_->get_instruction(pointAddr()); 
      delayInsn_.raw = owner_->get_instruction(pointAddr()+INSN_SIZE); 
    }
  ~instPoint() { /* TODO */ }

  int size() const { return size_; }
  unsigned code() const { return origInsn_.raw; }
  Offset offset() const { return offset_; }

  void print(FILE *stream = stderr, char *pre = NULL) 
    {
      if (pre) fprintf(stream, pre);
      switch(getPointType()) {
      case functionEntry: fprintf(stream, "entry "); break;
      case functionExit: fprintf(stream, "exit "); break;
      case callSite: fprintf(stream, "call "); break;
      case otherPoint: fprintf(stream, "other "); break;
      default: fprintf(stream, "unknown "); break;
      }
      fprintf(stream, "point of \"%s\"\n", pointFunc()->prettyName().c_str());
    }

 public:
  unsigned       flags;
  int            vectorId;
  // protected:
  Address        hint_got_;  // callee hint (relative vaddr of GOT entry) 
  image         *owner_;     // image containing "func_"
  Offset         offset_;    // offset of instPoint from function start
  int            size_;      // instPoint footprint
  instruction    origInsn_;  // instruction being instrumented

  instruction    delayInsn_;
  Offset         origOffset_;
};

class entryPoint : public instPoint {
 public:
  entryPoint(int_function *func, Offset off, unsigned info = 0) 
    : instPoint(func, off, functionEntry, info) {}
};

class exitPoint : public instPoint {
 public:
  exitPoint(int_function *func, Offset off, unsigned info = 0) 
    : instPoint(func, off, functionExit, info) {}
};

class callPoint : public instPoint {
 public:
  callPoint(int_function *func, Offset off, unsigned info = 0) 
    : instPoint(func, off, callSite, info) {}
};


#endif /* _INST_POINT_MIPS_H_ */
