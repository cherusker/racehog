#include "SteenX/VM/Memory/MemoryAtom.h"

#include <algorithm> // std::copy
#include <utility>   // std::swap

#include "Support/Macros.h"

bool
racehog::MemoryAtom::
equalise(MemoryAtom* other) {
  if (other == nullptr) {
    LOG(ERROR) << "MemoryAtom is nullptr.";
    return false;
  }
  // TODO:  Are cross assignments possible and legal?
  //   -> Assigning pointers to threads to functions?
  return (equalisePointees(other)  == true &&
          equaliseThreads(other)   == true &&
          equaliseFunctions(other) == true &&
          equaliseLocks(other)     == true) ? true : false;
}

bool
racehog::MemoryAtom::
equalisePointees(MemoryAtom* other) {
  auto p1 = this->foldPointees();
  auto p2 = other->foldPointees();
  if (p1 == p2)
    // Could be the most common case (`nullptr == nullptr`).
    return true;
  if (p1 == nullptr) {
    (void) this->addPointee(p2);
    return true;
  }
  if (p2 == nullptr) {
    (void) other->addPointee(p1);
    return true;
  }
  return p1->unify(p2);
}

bool
racehog::MemoryAtom::
equaliseFunctions(MemoryAtom* other) {
  auto f1 = this->getFunction();
  auto f2 = other->getFunction();
  if (f1 == f2)
    // Most common case (`nullptr == nullptr`).
    return true;
  if (f1 == nullptr) {
    (void) this->setFunction(f2);
    return true;
  }
  if (f2 == nullptr) {
    (void) other->setFunction(f1);
    return true;
  }
  LOG(ERROR) << "Unifying function pointers required.";
  return false;
}

bool
racehog::MemoryAtom::
equaliseThreads(MemoryAtom* other) {
  auto t1 = this->getThread();
  auto t2 = other->getThread();
  if (t1 == t2)
    // Most common case (`nullptr == nullptr`).
    return true;
  if (t1 == nullptr) {
    (void) this->setThread(t2);
    return true;
  }
  if (t2 == nullptr) {
    (void) other->setThread(t1);
    return true;
  }
  LOG(ERROR) << "Unifying thread pointers required.";
  return false;
}

bool
racehog::MemoryAtom::
equaliseLocks(MemoryAtom* other) {
  auto l1 = this->getLockId();
  auto l2 = other->getLockId();
  if (l1 == l2)
    return true;
  if (l1 == 0) {
    (void) this->setLockId(l2);
    return true;
  }
  if (l2 == 0) {
    (void) other->setLockId(l1);
    return true;
  }
  LOG(ERROR) << "Unifying locks required.";
  return false;
}

bool
racehog::MemoryAtom::
unify(MemoryAtom* other) {
  if (other == nullptr) {
    LOG(ERROR) << "MemoryAtom is nullptr.";
    return false;
  }
  auto lhs = getRepresentative();
  auto rhs = other->getRepresentative();
  if (lhs == rhs)
    return true; // Trivial;  already the same.
  // Make sure the two atoms are the same (recursively follow all pointers):
  if (lhs->equalise(rhs) == false) {
    LOG(ERROR) << "MemoryAtom::assign failed.";
    return false;
  }
  if (lhs->DisjointSet::unify(rhs) == false) {
    LOG(ERROR) << "DisjointSet::unify failed.";
    return false;
  }
  // MemoryAtom::assign has already matched all data between `lhs` and `rhs`.
  // The last thing left to do is copy the accesses and be done with the
  // unification.
  auto* from = &lhs->accesses;
  auto* to   = &rhs->accesses;
  if (lhs == rhs->getRepresentative())
    (void) std::swap(from, to);
  (void) std::copy(from->begin(), from->end(), std::back_inserter(*to));
  return true;
}

racehog::MemoryAtom*
racehog::MemoryAtom::
foldPointees(void) {
  auto& ps  = getRepresentative()->pointees;
  auto  it  = ps.begin();
  auto  end = ps.end();
  if (it == end) // No pointees.
    return nullptr;
  // Post-increment is fine here since `it` is just a regular pointer.
  MemoryAtom* rep = *it++; // First Pointee will be future representative.
  if (it == end) // Exactly one pointee.
    return rep;
  while (it != end) // Two or more pointees.
    (void) rep->unify(*it++);
  // Make `rep` the only pointee henceforth:
  (void) ps.clear();
  (void) ps.push_back(rep);
  (void) ps.shrink_to_fit();
  return rep;
}

void
racehog::MemoryAtom::
print(UserStream&        stream,
      const std::string& indent) const {
  stream << indent;
  (void) printId(stream);
  const auto parent = getParent(); // parent is const, representative is not.
  if (parent != this) {
    stream << " aka ";
    (void) parent->printId(stream);
    return;
  }
  if (pointees.empty() == false) {
    bool is_first_p = true;
    for (const auto p : pointees) {
      if (is_first_p == true) {
        stream << " -> ";
        is_first_p = false;
      } else {
        stream << ", ";
      }
      (void) p->printId(stream);
    }
  }
  if (func != nullptr) {
    stream << " -> F";
    const auto& meta = func->getMetadata();
    if (meta.isSet() == true)
      // TODO:  Having the id here would be nice.
      stream << ":" << *meta->getName();
  }
  if (thread != nullptr) {
    stream << " -> ";
    (void) thread->print(stream);
  }
  if (lock_id != 0)
    stream << " -> "
           << stream.setFormat(UserStream::Format::Variable)
           << "M" << lock_id
           << stream.resetFormat();
  bool is_first_a = true;
  for (const auto& access : accesses) {
    if (is_first_a == true) {
      is_first_a = false;
      stream << ": ";
    } else {
      stream << ", ";
    }
    (void) access.print(stream);
  }
}

void
racehog::MemoryAtom::
printIfInteresting(UserStream&        stream,
                   const std::string& indent) const {
  if (getParent()      != this    ||
      pointees.empty() == false   ||
      func             != nullptr ||
      thread           != nullptr ||
      lock_id          != 0       ||
      accesses.empty() == false)
    (void) print(stream, indent);
}
