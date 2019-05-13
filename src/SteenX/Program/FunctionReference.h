#ifndef STEENX_PROGRAM_FUNCTIONREFERENCE_H_
#define STEENX_PROGRAM_FUNCTIONREFERENCE_H_

#include <boost/core/noncopyable.hpp>

#include "Support/Stream/Serialisable.h"
#include "Support/TrackingReference.h"

namespace racehog {

  class Function;

  /// TrackingReference, to connect Function and Program.
  /**
   * **Very important**:
   * FunctionReference is an integral part of the _ONLY_ back edge in the whole
   * system:
   * Function > Instruction > FunctionOperand > FunctionReference > Function.
   * This cannot be avoided in the current setup as Instruction has to know the
   * exact size of every Operand, including FunctionOperand.
   *
   * A possible solution could be to have all TrackingReference objects store
   * `void` pointers that will only be cast back to their original form when
   * accessed.
   * This remains future work however and, until then, we use a forward
   * declaration of Function in FunctionReference (the _ONLY_ forward reference
   * in the whole system).
   */

  class FunctionReference final
    : public Serialisable,
      public TrackingReference<Stream::FunctionId, Function> {

    typedef Stream::FunctionId IdT;
    typedef Function           ReferenceT;

    typedef TrackingReference<IdT, ReferenceT> Super;

  public:

    FunctionReference(void) = default;

    FunctionReference(IdT         id,
                      ReferenceT* ref)
      : Super(id, ref) {}

    bool
    readFromStream(InputStream& stream) override {
      (void) setId(stream.readFunctionId());
      return stream.allReadsGood();
    }

    bool
    writeToStream(OutputStream& stream) const override {
      return stream.writeFunctionId(getId());
    }
  };
}

#endif // STEENX_PROGRAM_FUNCTIONREFERENCE_H_
