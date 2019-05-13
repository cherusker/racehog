#ifndef STEENX_VM_MEMORY_MEMORYATOM_H_
#define STEENX_VM_MEMORY_MEMORYATOM_H_

#include <deque>
#include <vector>

#include <boost/core/noncopyable.hpp>

#include "RaceHog/DataRace/DataRaceQueue.h"
#include "SteenX/Program/Function.h"
#include "SteenX/VM/Memory/MemoryAccess.h"
#include "SteenX/VM/Thread.h"
#include "Support/DisjointSet.h"
#include "Support/Logging.h"
#include "Support/range_iterator.h"
#include "Support/Stream/Printable.h"
#include "Support/Stream/Stream.h"

namespace racehog {

  /// Represents a memory location within MemoryStack or MemoryHeap.
  /**
   * MemoryAtom has a few tasks:
   *
   *   - Keep track of alias/unified relations to other MemoryAtom objects.
   *   - Store pointers to MemoryAtom*, Function*, Thread*, ...
   *   - Store all accesses of a memory location.
   *
   * The first task (alias relations) is mostly handled by DisjointSet which is
   * used by MemoryAtom in blackbox manner.
   *
   * The second and third tasks are handled directly within MemoryAtom; it keeps
   * track of pointees (if it identifies as pointer) and the accesses.
   */

  class MemoryAtom final
    : public  DisjointSet<MemoryAtom>,
      public  Printable,
      private boost::noncopyable {

  public:

    typedef size_t                         StoreId;
    typedef Stream::MemoryAtomId           Id;
    typedef DisjointSet<MemoryAtom>::Level Level;

    typedef std::vector<MemoryAtom*>                 Pointees;
    typedef range_iterator<Pointees::const_iterator> ConstPointeesRange;

    MemoryAtom(StoreId sid    = 0, ///< Id of the parent MemoryStore.
               Id      id     = 0, ///< Position within the store.
               Id      rel_id = 0, ///< Used for reporting/priting.
               Level   level  = 0) ///< Forwarded to DisjointSet::DisjointSet.
      : DisjointSet(level),
        sid(sid),
        id(id),
        rel_id(rel_id) {}

    StoreId
    getStoreId(void) {
      return getRepresentative()->sid;
    }
    
    /// Get unique identifier of representative MemoryAtom.
    
    Id
    getId(void) {
      return getRepresentative()->id;
    }

    /// Get pointees of representative MemoryAtom.
    
    ConstPointeesRange
    getPointees(void) {
      return ConstPointeesRange(getRepresentative()->pointees);
    }

    /// Add pointee to representative MemoryAtom.
    
    void
    addPointee(MemoryAtom* atom) {
      // TODO:  Only add _new_ pointers; i.e.
      // atom1->getRepresentative() != atom2->getRepresentative().
      (void) getRepresentative()->pointees.push_back(atom);
    }

    /// Get function pointer of representative MemoryAtom.

    const Function*
    getFunction(void) {
      return getRepresentative()->func;
    }

    /// Set function pointer of representative MemoryAtom.

    void
    setFunction(const Function* func) {
      getRepresentative()->func = func;
    }

    /// Get thread pointer of representative MemoryAtom.

    Thread*
    getThread(void) {
      return getRepresentative()->thread;
    }

    /// Set thread pointer of representative MemoryAtom.

    void
    setThread(Thread* thread) {
      getRepresentative()->thread = thread;
    }

    Thread::LockId
    getLockId(void) const {
      return lock_id;
    }

    void
    setLockId(Thread::LockId id) {
      lock_id = id;
    }

    /// Add a MemoryAccess to the representative MemoryAtom ...
    /**
     * ... test for duplicates based on the time and also test for data races by
     * pairwise calling MemoryAccess::racesWith with all MemoryAccess objects
     * that were already part of that MemoryAtom before.
     * DataRace objects will be pushed to DataRaceQueue.
     */

    template<class... Args>
    void
    appendAccess(DataRaceQueue& races,
                 Args&&...      args) {
      auto& as = getRepresentative()->accesses;
      (void) as.emplace_back(std::forward<Args>(args)...);
      auto it  = as.begin();
      auto end = as.end() - 1; // We know that last item ... ;)
      // The source of duplicates are unified memory atoms in the presence of
      // separate pointers.
      // The time of the last access is enough to quickly find duplicates as
      // accesses are added sequentially.
      // `it != end` is a quick check if `as` has at least one access prior to
      // the latest one.
      if (it != end &&
          (end - 1)->getTime() == end->getTime()) {
        (void) as.pop_back(); // Discard the duplicate access.
        return;
      }
      // Data race check:
      const auto& endlocks = end->getLocks();
      for (; it != end; ++it) {
        // Make sure to check EVERY item (but the last).
        if (it->racesWith(*end) == true) {
          auto race = std::make_unique<DataRace>(*it, *end);
          // TODO:  Improve performance by iterating the smaller set
          // (it->getLocks() or end->getLocks()) as locks::find() is generally
          // in `O(1)`.
          for (const auto& left : it->getLocks()) {
            auto right = endlocks.find(left.first);
            if (right != endlocks.end())
              (void) race->appendLockPair(left.second, right->second);
          }
          (void) races.push(std::move(race));
        }
      }
    }

    /// Equalise two MemoryAtom operands.
    /**
     * MemoryAtom::equalise makes sure that two MemoryAtom objects are exactly
     * alike, while still being two distinct objects that can be manipulated
     * separately from each other (if that was the case before).
     *
     * MemoryAtom::equalise calls all four subfunctions, one after another:
     *
     *  - MemoryAtom::equalisePointees
     *  - MemoryAtom::equaliseFunctions
     *  - MemoryAtom::equaliseThreads
     *  - MemoryAtom::equaliseLocks
     *
     * If any of these functions return `false`, MemoryAtom::equalise will
     * return `false` and `true` otherwise.
     *
     * Each subfunction works exactly the same, with the exception of their
     * preconditions (read carefully!).
     * Subfunctions compare values in the following order:
     *
     *   -# If both values are the same, nothing happens.
     *
     *   -# If one value is `nullptr` (or `0`), the value will be set using the
     *      other value (which cannot be `nullptr` or `0`).
     *
     *   -# If both values are different but not `nullptr` (or `0`), the value
     *      have to be unified.
     *      Currently, unification only works for MemoryAtom::equalisePointees.
     *
     * Note that MemoryAtom::equalise is commutative:
     * `a->equalise(b)` yields the same result as `b->equalise(a)`.
     */

    bool
    equalise(MemoryAtom* other);

    /// Compare MemoryAtom::foldPointees.
    /**
     * If both pointees are not `nullptr`, the pointees will be unified via
     * MemoryAtom::unify.
     *
     * More details at MemoryAtom::equalise.
     */

    bool
    equalisePointees(MemoryAtom* other);

    /// Compare MemoryAtom::getFunction.
    /**
     * Either both functions are the same, or at least one function has to be
     * `nullptr`.
     *
     * More details at MemoryAtom::equalise.
     */

    bool
    equaliseFunctions(MemoryAtom* other);

    /// Compare MemoryAtom::getThread.
    /**
     * Either both threads are the same, or at least one thread has to be
     * `nullptr`.
     *
     * More details at MemoryAtom::equalise.
     */

    bool
    equaliseThreads(MemoryAtom* other);

    /// Compare MemoryAtom::getLockId.
    /**
     * Either both lock ids are the same, or at least one lock id has to be `0`.
     *
     * More details at MemoryAtom::equalise.
     */

    bool
    equaliseLocks(MemoryAtom* other);

    /// Unify two MemoryAtom objects.
    /**
     * MemoryAtom::unify is commutative;  the order of who-calls-whom does not
     * matter, the outcome will always be the same.
     * Internally, MemoryAtom::unify calls MemoryAtom::equalise and
     * DisjointSet::unify.
     * In addition, all stored MemoryAccess objects will be copied where
     * necessary to ensure a sound transformation.
     */

    bool
    unify(MemoryAtom* other);

    /// _Unify_ (not just equalise!) all pointees (forever).
    /**
     * \return The representative MemoryAtom of all pointees.
     */

    MemoryAtom*
    foldPointees(void);

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;

    /// Calls MemoryAtom::print IFF MemoryAtom is considered "interesting".
    /**
     * There are three different reasons which can classify a MemoryAtom
     * "interesting":
     *
     *   - The MemoryAtom is unified with another MemoryAtom.
     *   - MemoryAtom points to something (MemoryAtom, Function, Thread, ...)
     *   - The MemoryAtom has at least one registered MemoryAccess.
     */

    void
    printIfInteresting(UserStream&        stream,
                       const std::string& indent = "") const;

  private:

    // TODO:  Could be converted to vector maybe as we never really have
    // many accesses in first place.   Plus:  is memory allocated when none is
    // needed?  -> We do have MANY atoms without a single access ...

    typedef std::deque<MemoryAccess> Accesses;

    StoreId sid;
    Id      id;
    Id      rel_id; ///< "Relative" id.

    Pointees        pointees;         ///< Used as regular pointer.
    const Function* func   = nullptr; ///< Used as function pointer.
    Thread*         thread = nullptr; ///< Used as thread pointer.

    Thread::LockId lock_id = 0; ///< Used as lock.

    Accesses accesses;

    /// Print the atom's identifier: `level:local_id`.

    void
    printId(UserStream&        stream,
            const std::string& indent = "") const {
      stream << indent
             << stream.setFormat(UserStream::Format::Keyword)
             << sid << ":" << getLevel() << ":" << rel_id
             << stream.resetFormat();
    }
  };
}

#endif // STEENX_VM_MEMORY_MEMORYATOM_H_
