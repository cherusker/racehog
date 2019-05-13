#ifndef STEENX_VM_MEMORY_MEMORYSTORE_H_
#define STEENX_VM_MEMORY_MEMORYSTORE_H_

#include <deque>

#include <boost/core/noncopyable.hpp>

#include "SteenX/VM/Memory/MemoryAtom.h"
#include "Support/Stream/Printable.h"

namespace racehog {
  
  /// MemoryAtom objects that belong together.
  /**
   * Execution is responsible for bookkeeping the connections between MemoryAtom
   * objects and CallContext objects as well as usin the correct
   * MemoryAtom::Level when pushing MemoryAtom objects to specific MemoryStore
   * objects.
   */

  class MemoryStore final
    : public  Printable,
      private boost::noncopyable {

  public:

    /// Id type of MemoryStore objects.

    typedef size_t Id;

    MemoryStore(Id id)
      : id(id) {}

    MemoryStore(Id             id,
                MemoryAtom::Id num_atoms)
      : id(id) {
      // The version of pushAtoms _with_ the level is a bit faster ...
      (void) pushAtoms(num_atoms, 0);
    }

    Id
    getId(void) const {
      return id;
    }

    /// Returns `nullptr` on illegal \p id.

    MemoryAtom*
    operator[](MemoryAtom::Id id) {
      return id >= size() ? nullptr : &atoms[id];
    }

    /// Returns `nullptr` on illegal \p id.

    const MemoryAtom*
    operator[](MemoryAtom::Id id) const {
      return id >= size() ? nullptr : &atoms[id];
    }

    /// Number of MemoryAtom objects in the MemoryStore.

    MemoryAtom::Id
    size(void) const {
      return atoms.size();
    }

    /// Append \p num_atoms MemoryAtom objects, using the last known level.
    /**
     * Appended MemoryAtom objects will have the same level as the last
     * MemoryAtom object in the MemoryStore before calling
     * MemoryStore::pushAtoms.
     * Relative ids of appended MemoryAtom objects will start at the last
     * MemoryAtom object `+1` in the MemoryStore at the time of calling
     * MemoryStore::pushAtoms.
     *
     * If the MemoryStore is empty at the time of calling
     * MemoryStore::pushAtoms, `0` will be assumed for the level as well as the
     * relative starting id.
     */

    void
    pushAtoms(MemoryAtom::Id num_atoms);

    /// Append \p num_atoms MemoryAtom objects with the level \p level.
    /**
     * Relative ids of appended MemoryAtom objects will start at `0`.
     */

    void
    pushAtoms(MemoryAtom::Id    num_atoms,
              MemoryAtom::Level level) {
      (void) pushAtomsImpl(num_atoms, 0, level);
    }

    /// Pop MemoryAtom objects from the stack.

    void
    popAtoms(MemoryAtom::Id num_atoms) {
      (void) atoms.resize(size() - num_atoms);
    }

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;

  private:

    /// `std::deque` is the container of choice.
    /**
     *   - Random access: `O(1)` (only neglectably worse than `std::vector`).
     *
     *   - Back insertion: `O(1)` (slightly but neglectably better than
     *     `std::vector`).
     *
     *   - Back deletion does not happen immeditaly but has to be triggered
     *     manually which helps with memory recycling.  Furthermore, memory is
     *     deleted in chunks, not on an element level.  `std::vector` behaves
     *     the same when it comes to recycling but is worse when deleting
     *     overhead (due to full reallocations).
     *
     *   - Insertion/deletion does not copy/move elements.
     *     Apart from saving the allocation and copy overhead, pointers to
     *     elements within the deque stay valid, even after an insertion (in
     *     contrast to `std::vector`).
     *
     *   - Good spatial locality of atoms within a function call as well as good
     *     spatial locality of global atoms (only neglectably worse than
     *     `std::vector`).
     *
     *   - (The only?) downside of `std::deque` compared to `std::vector` is
     *     that elements are not stored continously in memory which makes the
     *     offset operator (`std::deque::operator[]`) the only way to access
     *     specific elements (instead of raw-pointer-offset magic with
     *     `std::vector`).
     *     The overhead of one additional _addition_ per atom  access (using
     *     something like `atoms[first_atom_id_of_frame + atom_id]`) should be
     *     absorbed by caching the returned pointer wherever feasible.
     */

    typedef std::deque<MemoryAtom> Atoms;

    // TODO:  shrink_to_fit!

    Id    id;
    Atoms atoms;

    void
    pushAtomsImpl(MemoryAtom::Id    num_atoms,
                  MemoryAtom::Id    rel_id,
                  MemoryAtom::Level level);
  };
}

#endif // STEENX_VM_MEMORY_MEMORYSTORE_H_
