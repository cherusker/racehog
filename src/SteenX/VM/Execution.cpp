#include "SteenX/VM/Execution.h"

#include "SteenX/Program/Operand/Operands.h"
#include "SteenX/VM/ExecutionInteractor.h"

racehog::Execution::
Execution(UserStream&    stream,
          DataRaceQueue& races,
          const Program& prog)
  : stream(stream),
    races(races),
    env(prog),
    eq(env) {
  (void) env.pushThread()->start();
  auto& context = env.pushCallContext(*prog.getEntryPoint(), &exe_retval);
  (void) context.setIsThreadBase();
}

bool
racehog::Execution::
run(void) {
  while (env.hasNextInstruction() == true) {
    if (dispatch(env.getNextInstruction()) == false ||
        stream.hasFatalMessage()           == true) {
      LOG(WARNING) << "Aborting early.";
      return false;
    }
  }
  return true;
}

bool
racehog::Execution::
interactiveRun(void) {
  ExecutionInteractor jester(env, stream);
  (void) jester.welcome();
  while (env.hasNextInstruction() == true) {
    const auto& inst = env.getNextInstruction();
    (void) jester.showInstruction("Next", inst);
    if (jester.interact() == false) {
      LOG(WARNING) << "Aboring early.";
      return false;
    }
    if (dispatch(inst) == false) {
      LOG(WARNING) << "Aborting early.";
      return false;
    }
    (void) jester.showInstruction("Executed", inst);
    if (stream.hasFatalMessage() == true) {
      LOG(WARNING) << "Aborting early.";
      return false;
    }
  }
  (void) jester.showText("The End.");
  (void) jester.interact();
  return true;
}

bool
racehog::Execution::
visitForkInstruction(const ForkInstruction& inst) {
  auto thread     = env.pushThread();
  auto thread_ptr = env.getMemoryAtom(inst.getThreadPointer());
  if (thread_ptr == nullptr)
    RH_FATAL_FALSE_BROKEN_PROGRAM("Illegal thread pointer.");
  (void) thread_ptr->setThread(thread);
  (void) thread->start();
  if (beginFunctionCall(inst, env.getThreadReturnValue(thread)) == false)
    RH_FATAL_FALSE_BROKEN_PROGRAM("Execution::beginFunctionCall failed.");
  (void) env.getActiveCallContext().setIsThreadBase();
  return true;
}

bool
racehog::Execution::
visitJoinInstruction(const JoinInstruction& inst) {
  auto thread_ptr = env.getMemoryAtom(inst.getThreadPointer());
  if (thread_ptr == nullptr)
    RH_FATAL_FALSE_BROKEN_PROGRAM("Expected thread pointer.");
  auto thread = thread_ptr->getThread();
  if (thread == nullptr)
    RH_FATAL_FALSE_BROKEN_PROGRAM("Unable to join thread.");
  (void) thread->stop();
  if (eq.equalise(
        env.getThreadReturnValue(thread),
        inst.getReturnValue()) == false)
    RH_FATAL_FALSE_BROKEN_PROGRAM("Equaliser::equalise failed.");
  return true;
}

bool
racehog::Execution::
visitAcquireInstruction(const AcquireInstruction& inst) {
  if (env.hasActiveThread() == false)
    RH_FATAL_FALSE_BROKEN_PROGRAM("Environment::hasActiveThread failed.");
  auto lock = env.getMemoryAtom(inst.getLock());
  if (lock == nullptr)
    RH_FATAL_FALSE_BROKEN_PROGRAM("Environment::getMemoryAtom failed.");
  auto            metaref = inst.getMetadata();
  const Metadata* meta    = metaref.isSet() == true ? metaref.getRaw() : nullptr;
  for (const auto p : lock->getPointees()) {
    auto lock_id = p->getLockId();
    if (lock_id == 0) {
      lock_id = env.getNextLockId();
      (void) p->setLockId(lock_id);
    }
    (void) env.getActiveThread()->acquireLock(lock_id, meta);
  }
  return true;
}
    
bool
racehog::Execution::
visitReleaseInstruction(const ReleaseInstruction& inst) {
  if (env.hasActiveThread() == false)
    RH_FATAL_FALSE_BROKEN_PROGRAM("Environment::hasActiveThread failed.");
  auto lock = env.getMemoryAtom(inst.getLock());
  if (lock == nullptr)
    RH_FATAL_FALSE_BROKEN_PROGRAM("Environment::getMemoryAtom failed.");
  for (const auto p : lock->getPointees())
    (void) env.getActiveThread()->releaseLock(p->getLockId());
  return true;
}

bool
racehog::Execution::
visitCallInstruction(const CallInstruction& inst) {
  if (beginFunctionCall(
        inst,
        env.getMemoryAtom(inst.getReturnValue())) == false)
    RH_FATAL_FALSE_BROKEN_PROGRAM("Execution::beginFunctionCall failed.");
  return true;
}

bool
racehog::Execution::
visitReturnInstruction(const ReturnInstruction& inst) {
  if (eq.equalise(
        env.getActiveCallContext().getReturnValue(),
        inst.getReturnValue()) == false)
    RH_FATAL_FALSE_BROKEN_PROGRAM("Equaliser::equalise failed.");
  return true;
}

bool
racehog::Execution::
visitReferenceInstruction(const ReferenceInstruction& inst) {
  auto       val  = env.getMemoryAtom(inst.getValue());
  const auto base = env.getMemoryAtom(inst.getBase());
  if (val == nullptr || base == nullptr)
    RH_FATAL_FALSE_BROKEN_PROGRAM("Environment::getMemoryAtom failed.");
  const auto baseop = inst.getBase().as<MemoryAtomOperand>();
  // `baseop != nullptr` can be assumed due to specification.
  if (baseop->isPointer() == false) {
    (void) resolveReference(inst, val, base);
    return true;
  }
  for (const auto p : base->getPointees())
    (void) resolveReference(inst, val, p);
  return true;
}

bool
racehog::Execution::
visitEqualiseInstruction(const EqualiseInstruction& inst) {
  if (eq.equalise(inst.getLeftValue(), inst.getRightValue()) == false)
    RH_FATAL_FALSE_BROKEN_PROGRAM("Equaliser::equalise failed.");
  return true;
}

bool
racehog::Execution::
visitAllocateInstruction(const AllocateInstruction& inst) {
  (void) env.getMemoryAtom(inst.getValue())
    ->addPointee(env.pushMemoryStore(inst.getSize())[0]);
  return true;
}

const racehog::Function*
racehog::Execution::
extractFunction(const Operand& op) {
  if (const auto f = op.as<FunctionOperand>())
    return f->getFunction().getRaw();
  if (const auto m = op.as<MemoryAtomOperand>()) {
    const auto ma = env.getMemoryAtom(op);
    // `ma->getFunction()` can never be `const`.
    return ma == nullptr ? nullptr : ma->getFunction();
  }
  return nullptr;
}

bool
racehog::Execution::
beginFunctionCall(const CallInstructionBase& inst,
                  MemoryAtom*                retval) {
  // TODO:  Split/fork Execution when facing multiple functions.
  const auto func = extractFunction(inst.getFunction());
  if (func == nullptr)
    RH_FATAL_FALSE_BROKEN_PROGRAM("Unexpected function operand.");
  auto& caller = env.getActiveCallContext();
  auto& callee = env.pushCallContext(*func, retval);
  if (inst.getNumArguments() > func->getNumLocalMemoryAtoms()) {
    const auto& meta = func->getMetadata();
    stream.warn()
      << "Empty function"
      << (meta.isSet() == true ? " \"" + *meta->getName() + "\"" : "")
      << ".\n";
    return true;
  }
  MemoryAtom::Id arg_id = callee.getLocalBaseId();
  for (const auto& arg : inst.getArguments()) {
    if (eq.equalise(env.getMemoryAtom(arg_id), caller, arg) == false)
      RH_FATAL_FALSE_BROKEN_PROGRAM("Equaliser::equalise failed.");
    ++arg_id;
  }
  return true;
}

void
racehog::Execution::
resolveReference(const ReferenceInstruction& inst,
                 MemoryAtom*                 val,
                 MemoryAtom*                 base) {
  auto       reps   = inst.getRangeRepeat();
  const auto spread = inst.getRangeSpread();
  const auto sid    = base->getStoreId();
  auto       id     = base->getId() + inst.getBaseOffset();
  do {
    (void) val->addPointee(env.getMemoryAtom(sid, id));
    id += spread;
  } while (reps-- > 0);
}

bool
racehog::Execution::
visitAccessInstructionImpl(const ValueInstructionBase& inst,
                           MemoryAccess::Mode          mode) {
  auto time   = env.getClock().tick();
  if (const auto maop = inst.getValue().as<MemoryAtomOperand>()) {
    auto atom = env.getMemoryAtom(*maop);
    if (atom == nullptr)
      RH_FATAL_FALSE_BROKEN_PROGRAM("Environment::getMemoryAtom failed.");
    if (maop->isPointer() == false) {
      (void) atom->appendAccess(
        races,
        mode,
        time,
        env.getActiveThread(),
        env.getCallStackMetatrace());
      return true;
    }
    for (auto p : atom->getPointees())
      (void) p->appendAccess(
        races,
        mode,
        time,
        env.getActiveThread(),
        env.getCallStackMetatrace());
    return true;
  }
  RH_FATAL_FALSE_BROKEN_PROGRAM("Unexpected operand.");
  return true;
}
