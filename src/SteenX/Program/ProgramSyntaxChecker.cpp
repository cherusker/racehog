#include "SteenX/Program/ProgramSyntaxChecker.h"

bool
racehog::ProgramSyntaxChecker::
verifyImpl(const Program& prog,
           bool           is_exhaustive) {
  bool retval = true;
  num_global_atoms = prog.getNumGlobalMemoryAtoms();
  for (const auto& func : prog.getFunctions()) {
    num_local_atoms = func.getNumLocalMemoryAtoms();
    for (const auto& inst : func.getInstructions()) {
      if (dispatch(inst) == false) {
        retval = false;
        if (is_exhaustive == false)
          return false;
      }
    }
  }
  return retval;
}

bool
racehog::ProgramSyntaxChecker::
report(const std::string& message,
       const Instruction& inst) {
  stream.fatal() << message << " ";
  (void) inst.print(stream);
  stream << "\n";
  return false;
}

bool
racehog::ProgramSyntaxChecker::
report(const std::string& message,
       const Operand&     op,
       const Instruction& inst) {
  stream.fatal() << message << " ";
  (void) op.print(stream);
  stream << " in ";
  (void) inst.print(stream);
  stream << "\n";
  return false;
}

bool
racehog::ProgramSyntaxChecker::
verifyOperands(const Instruction& inst,
               SyntaxOperandTypes types,
               bool               has_opt_ops) {
  const auto num_ops = types.size();
  if (has_opt_ops == true) {
    if (inst.getNumOperands() < num_ops)
      return report("Not enough operands in", inst);
  } else {
    if (inst.getNumOperands() != num_ops)
      return report("Unexpected operands in", inst);
  }
  auto op = inst.getOperands().begin();
  for (auto type : types) {
    if (verifyOperand(inst, *op++, type) == false)
      return false;
  }
  return true;
}

bool
racehog::ProgramSyntaxChecker::
verifyOperand(const Instruction& inst,
              const Operand&     op,
              SyntaxOperandType  type) {
  // TODO:  We could use some templates/helpers/macros to make this more
  //   readable.
  bool type_ok = false;
  switch (type) {
  case SyntaxOperandType::Anything: {
    type_ok = true; // Trivial.
  } break;
  case SyntaxOperandType::MemoryAtom: {
    if (const auto maop = op.as<MemoryAtomOperand>()) {
      if (maop->isPointer() == false)
        type_ok = true;
    }
  } break;
  case SyntaxOperandType::MemoryAtomPointer: {
    if (const auto maop = op.as<MemoryAtomOperand>()) {
      if (maop->isPointer() == true)
        type_ok = true;
    }
  } break;
  case SyntaxOperandType::MemoryAtomOrMemoryAtomPointer: {
    if (op.isMemoryAtom() == true)
      type_ok = true;
  } break;
  case SyntaxOperandType::FunctionOrMemoryAtom: {
    if (op.isFunction() == true) {
      type_ok = true;
    } else if (const auto maop = op.as<MemoryAtomOperand>()) {
      if (maop->isPointer() == false)
        type_ok = true;
    }
  } break;
  case SyntaxOperandType::FunctionOrMemoryAtomOrMemoryAtomPointer: {
    if (op.isFunction()   == true ||
        op.isMemoryAtom() == true) {
      type_ok = true;
    }
  } break;
  }
  if (type_ok == false)
    return report("Unexpected operand", op, inst);
  if (op.isGlobalMemoryAtom() == true)
    return verifyOperandBounds(inst, op, num_global_atoms);
  if (op.isLocalMemoryAtom() == true)
    return verifyOperandBounds(inst, op, num_local_atoms);
  return true;
}

bool
racehog::ProgramSyntaxChecker::
verifyOperandBounds(const Instruction& inst,
                    const Operand&     op,
                    MemoryAtomId       num_atoms) {
  if (const auto maop = op.as<MemoryAtomOperand>()) {
    if (maop->getMemoryAtomId() < num_atoms)
      return true;
  }
  return report("Out of bounds", op, inst);
}
