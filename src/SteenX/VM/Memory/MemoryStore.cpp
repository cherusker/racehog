#include "SteenX/VM/Memory/MemoryStore.h"

void
racehog::MemoryStore::
pushAtoms(MemoryAtom::Id num_atoms) {
  MemoryAtom::Id    rel_id = 0;
  MemoryAtom::Level level  = 0;
  auto              s      = size();
  if (s > 0) {
    const auto b = operator[](s - 1);
    rel_id = b->getId();
    level  = b->getLevel();
  }
  (void) pushAtomsImpl(num_atoms, rel_id, level);
}

void
racehog::MemoryStore::
print(UserStream&        stream,
      const std::string& indent) const {
  bool is_first = true;
  for (const auto& atom : atoms) {
    // It's hacky to use "\n" in the indent BUT we know that MemoryAtom::print
    // will always return exactly zero or one line.  Using "\n" here prevents
    // some logic around the already somewhat hacky
    // MemoryAtom::printIfInteresting.
    // This could be cleaned up by interested individuals one day.
    (void) atom.printIfInteresting(
      stream, (is_first == true ? "" :  "\n") + indent);
    if (is_first == true)
      is_first = false;
  }
}

void
racehog::MemoryStore::
pushAtomsImpl(MemoryAtom::Id    num_atoms,
              MemoryAtom::Id    rel_id,
              MemoryAtom::Level level) {
  // TODO:  Is it faster to use deque::resize and set the ids + level
  // afterwards?
  auto atom_id = size();
  const auto new_size = atom_id + num_atoms;
  while (atom_id < new_size)
    (void) atoms.emplace_back(id, atom_id++, rel_id++, level);
}
