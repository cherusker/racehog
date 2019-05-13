#include "SteenX/VM/Environment.h"

// Boo C++!  Boo!  -> ODR.

constexpr racehog::Environment::ThreadStack::size_type
racehog::Environment::num_init_threads;

racehog::Environment::
Environment(const Program& prog)
  : tstack(num_init_threads) {
  // Initialising the common program stack and thread return values:
  (void) mstores.emplace_back(0, prog.getNumGlobalMemoryAtoms());
  (void) mstores.emplace_back(1, num_init_threads);
  mstack   = &mstores[0];
  tretvals = &mstores[1];
}

racehog::Thread*
racehog::Environment::
pushThread(void) {
  auto id = threads.size();
  (void) threads.emplace_back(id);
  auto thread = &threads.back();
  if (hasActiveThread() == true) 
    // Only the main execution thread lacks a parent.
    (void) thread->setParent(getActiveThread());
  (void) tstack.push_back(thread);
  (void) tretvals->pushAtoms(1);
  return thread;
}

racehog::MemoryAtom*
racehog::Environment::
getMemoryAtom(CallContext&   context,
              const Operand& op) {
  // Ordered by expected frequency of occurrence:
  if (const auto l = op.as<LocalMemoryAtomOperand>())
    return (*mstack)[context.getLocalBaseId() + l->getMemoryAtomId()];
  if (const auto g = op.as<GlobalMemoryAtomOperand>())
    return (*mstack)[g->getMemoryAtomId()];
  LOG(ERROR) << "Unexpected operand.";
  return nullptr;
}

const racehog::MemoryAtom*
racehog::Environment::
getMemoryAtom(const CallContext& context,
              const Operand&     op) const {
  // Ordered by expected frequency of occurrence:
  if (const auto l = op.as<LocalMemoryAtomOperand>())
    return (*mstack)[context.getLocalBaseId() + l->getMemoryAtomId()];
  if (const auto g = op.as<GlobalMemoryAtomOperand>())
    return (*mstack)[g->getMemoryAtomId()];
  LOG(ERROR) << "Unexpected operand.";
  return nullptr;
}

racehog::CallContext&
racehog::Environment::
pushCallContext(const Function& func,
                MemoryAtom*     retval) {
  (void) trace.pushFrame(func.getMetadata().getRaw());
  auto base_id = mstack->size();
  // Beware that level `0` is reserved for global memory atoms.
  (void) mstack->pushAtoms(func.getNumLocalMemoryAtoms(), contexts.size() + 1);
  (void) contexts.emplace_back(base_id, retval, func);
  return contexts.back();
}

void
racehog::Environment::
popCallContext(void) {
  (void) trace.popFrame();
  auto& context = getActiveCallContext();
  if (context.isThreadBase() == true)
    (void) popThread();
  (void) mstack->popAtoms(context.getFunction().getNumLocalMemoryAtoms());
  (void) contexts.pop_back();
}

bool
racehog::Environment::
hasNextInstruction(void) const {
  // This loop terminates during the first iteration in most cases.
  auto context = contexts.rbegin();
  auto end     = contexts.rend();
  for (; context != end; ++context) {
    if (context->hasNextInstruction() == true)
      return true;
  }
  return false;
}

const racehog::Instruction&
racehog::Environment::
getNextInstruction(void) {
  while (getActiveCallContext().hasNextInstruction() == false)
    (void) popCallContext();
  const Instruction& inst = getActiveCallContext().getNextInstruction();
  (void) trace.updateLocation(inst.getMetadata().getRaw());
  return inst;
}
