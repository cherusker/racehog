#ifndef STEENX_LLVM_STRINGTABLE_H_
#define STEENX_LLVM_STRINGTABLE_H_

#include <unordered_map>

#include <boost/core/noncopyable.hpp>

#include "SteenX/Program/StringPool.h"
#include "SteenX/Program/StringReference.h"

namespace racehog {
  
  /// Responsibly watches over StringPool.
  /**
   * The main task of StringTable is bookkeeping:  no `std::string` will be
   * inserted twice into StringPool.
   */

  class StringTable final
    : private boost::noncopyable {

  public:

    StringTable(StringPool& pool)
      : pool(pool) {}

    StringReference
    insert(const std::string& str) {
      // std::pair<std::iterator<std::string, StringPool::StringId>, bool>
      auto val = lookup.insert(std::make_pair(str, lookup.size()));
      if (val.second == true)
        return pool.insert(str);
      return pool[val.first->second];
    }

  private:
    
    typedef std::unordered_map<
      std::string,
      StringPool::StringId> LookupContainer;

    StringPool&     pool;
    LookupContainer lookup;
  };
}

#endif // STEENX_LLVM_STRINGTABLE_H_
