#ifndef STEENX_PROGRAM_STRINGPOOL_H_
#define STEENX_PROGRAM_STRINGPOOL_H_

#include <deque>

#include <boost/core/noncopyable.hpp>

#include "SteenX/Program/StringReference.h"
#include "Support/Stream/Printable.h"
#include "Support/Stream/Serialisable.h"

namespace racehog {
  
  /// A simple, serialisable collection of `std::string`.

  class StringPool final
    : public  Printable,
      public  Serialisable,
      private boost::noncopyable {

  public:

    typedef Stream::StringId StringId;

    /// No bounds check;  know what you request!

    StringReference
    operator[](Stream::StringId id) {
      return StringReference(id, &strs[id]);
    }

    bool
    readFromStream(InputStream& stream) override;

    bool
    writeToStream(OutputStream& stream) const override;

    bool
    empty(void) const {
      return strs.empty();
    }

    StringId
    size(void) const {
      return strs.size();
    }

    /// Naive insert at the back.
    /**
     * There is no check if the inserted string exists in this StringPool
     * already.
     * It could make sense, of course, to apply some compression and/or reusage
     * of string bits to keep file sizes small and memory usage to a minimum.
     * Such clever mechanisms have to be applyied from the outside however.
     * StringPool naively stores everything that is being thrown at it.
     */

    StringReference
    insert(const std::string& str) {
      // We simply assume that everything goes right ...
      (void) strs.push_back(str);
      return StringReference(strs.size() - 1, &strs.back());
    }

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;

  private:
    
    /**
     * `std::deque` is the choice here since we don't want any reallocations of
     * `std::string`.
     */

    typedef std::deque<std::string> Strings;

    Strings strs;
  };
}

#endif // STEENX_PROGRAM_STRINGPOOL_H_
