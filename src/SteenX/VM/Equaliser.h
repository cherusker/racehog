#ifndef STEENX_VM_EQUALISER_H_
#define STEENX_VM_EQUALISER_H_

#include <boost/core/noncopyable.hpp>

#include "SteenX/Program/Function.h"
#include "SteenX/Program/Operand/Operand.h"
#include "SteenX/VM/CallContext.h"
#include "SteenX/VM/Environment.h"
#include "SteenX/VM/Memory/MemoryAtom.h"
#include "SteenX/VM/Thread.h"
#include "Support/turbovector.h"

namespace racehog {
  
  /// Equalising all sorts of MemoryAtom and Operand objects.

  class Equaliser final
    : private boost::noncopyable {

  public:

    Equaliser(Environment& env)
      : env(env) {}

    bool
    equalise(MemoryAtom* lhs,
             MemoryAtom* rhs);

    bool
    equalise(MemoryAtom*    lhs,
             const Operand& rhs_op) {
      return equalise(lhs, env.getActiveCallContext(), rhs_op);
    }

    bool
    equalise(MemoryAtom*    lhs,
             CallContext&   rhs_context,
             const Operand& rhs_op);

    bool
    equalise(const Function* lhs,
             MemoryAtom*     rhs);

    bool
    equalise(const Function* lhs,
             const Operand&  rhs_op) {
      return equalise(lhs, env.getActiveCallContext(), rhs_op);
    }

    bool
    equalise(const Function* lhs,
             CallContext&    rhs_context,
             const Operand&  rhs_op);

    bool
    equalise(const Operand& lhs_op,
             const Operand& rhs_op) {
      return equalise(
        env.getActiveCallContext(),
        lhs_op,
        env.getActiveCallContext(),
        rhs_op);
    }

    bool
    equalise(CallContext&   lhs_context,
             const Operand& lhs_op,
             CallContext&   rhs_context,
             const Operand& rhs_op);

  private:

    Environment& env;

    /// Shared some common code when resolving MemoryAtomOperand objects (RHS).
    /**
     * \tparam T Function or MemoryAtom.
     */

    template<class T>
    bool
    forwardMemoryAtomOperand(T*                       lhs,
                             CallContext&             rhs_context,
                             const MemoryAtomOperand* rhs_maop) {
      // It is very important to go via Environment as we could deal with a
      //   GlobalMemoryOperand.
      auto rhs = env.getMemoryAtom(rhs_context, *rhs_maop);
      // `nullptr` check by Equaliser::equalise(MemoryAtom*, MemoryAtom*).
      if (rhs_maop->isPointer() == false)
        return equalise(lhs, rhs);
      for (auto rhs_p : rhs->getPointees()) {
        if (equalise(lhs, rhs_p) == false)
          return false;
      }
      return true;
    }
  };
}

#endif // STEENX_VM_EQUALISER_H_
