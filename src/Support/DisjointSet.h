#ifndef RACEHOG_SUPPORT_DISJOINTSET_H_
#define RACEHOG_SUPPORT_DISJOINTSET_H_

#include <cstdlib> // std::size_t
#include <utility> // std::swap

#include "Support/Logging.h"

namespace racehog {
  
  /// An almost standard disjoint set.
  /**
   * DisjointSet encapsulates all logic around disjoint sets (unification,
   * parents, and representative items).
   * To use DisjointSet, inherit from it and call its protected members as
   * needed.
   *
   * DisjointSet has an additional `level` parameter which sets it apart from 
   * regular disjoint sets.
   * `level` impacts the unification to avoid dangling references when unifying
   * MemoryAtom objects across different CallContext objects (stack frames):
   *
   *   -# if `x.level == y.level`:
   *      size-based unification as expected of disjoint sets in `O(n)`
   *
   *   -# if `x.level < y.level`:
   *      `y` will always point towards `x` in `O(n^2)`
   *
   * `level` helps preserving the idea of actual stack frames where no item can
   * ever point to to a frame that is "higher" up on the stack than the item's 
   * own frame.
   * This consideration is necessary to avoid dangling references _for free_
   * when returning to the caller of a function (and removing the callee's
   * frame / information).
   * 
   * TL;DR the original idea of size-based unification of disjoint sets can only
   * be applied to items that belong to the same frame/level.
   * 
   * The global level ("globals") should be represented by the lowest possible
   * level (`0`) while regular stack frames will start at level `1`.
   *
   * \tparam DisjointSetImpl Has to be the type of the deriving class.
   *         DisjointSet will use it to statically downcast itself where
   *         necessary for convenient access.
   *         In the bigger picture, this design helps avoiding virtual function
   *         calls and downcasts within deriving classes.
   */

  template<class DisjointSetImpl>
  class DisjointSet {

  public:

    typedef std::size_t Level;

    Level
    getLevel(void) const {
      return level;
    }

  protected:

    DisjointSet(Level level)
      : level(level) {}

    DisjointSet(const DisjointSet& other)
      : parent(other.parent),
        level(other.level),
        size(other.size) {
      LOG(INFO) << "Copying DisjointSet.";
    }

    DisjointSetImpl*
    getParent(void) const {
      return static_cast<DisjointSetImpl*>(parent);
    }

    void
    setParent(DisjointSet* parent) {
      this->parent = parent;
    }

    /**
     * When performing actions on DisjointSet objects make sure to always
     * perform them on the representative item.
     *
     * Path splitting is performed while finding the representative which
     * will alter all connected sets (`x->getParent()->getParent()->...`).
     */

    DisjointSetImpl*
    getRepresentative(void) {
      auto rep    = this;
      auto parent = rep->getParent();
      while (rep != parent) {
        (void) rep->setParent(parent->getParent());
        rep = parent;
        parent = rep->getParent();
      }
      return static_cast<DisjointSetImpl*>(rep);
    }

    /// Level- and size-based unification.
    /**
     * See DisjointSet class description for details.
     */

    bool
    unify(DisjointSet* other) {
      if (other == nullptr) {
        LOG(ERROR) << "DisjointSet is nullptr.";
        return false;
      }
      auto lhs = getRepresentative();
      auto rhs = other->getRepresentative();
      if (lhs == rhs)
        return true;
      // swap if:
      //   1) lhs.level >  rhs.level
      //   2) lhs.level == rhs.level && lhs.size < rhs.size
      // 1: make sure to always point from high to low
      // 2: make sure to always attach smaller to larger sets on the same level
      if ( lhs->getLevel() >  rhs->getLevel()  ||
          (lhs->getLevel() == rhs->getLevel()  &&
           lhs->getSize()  <  rhs->getSize()))
        (void) std::swap(lhs, rhs);
      (void) rhs->setParent(lhs);
      (void) lhs->addSize(rhs->getSize());
      return true;
    }

  private:

    typedef std::size_t SetSize;

    DisjointSet* parent = this;
    Level        level;
    SetSize      size   = 1;

    void
    addSize(SetSize size) {
      this->size += size;
    }

    SetSize
    getSize(void) const {
      return size;
    }
  };
}

#endif // RACEHOG_SUPPORT_DISJOINTSET_H_
