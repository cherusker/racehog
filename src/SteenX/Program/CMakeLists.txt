set("SOURCE_DIR" "${PROJECT_SOURCE_DIR}/SteenX/Program")

set("SOURCE_FILES"
  "${SOURCE_DIR}/Bitfile.cpp"
  "${SOURCE_DIR}/Function.cpp"
  "${SOURCE_DIR}/Instruction/AcquireInstruction.cpp"
  "${SOURCE_DIR}/Instruction/AllocateInstruction.cpp"
  "${SOURCE_DIR}/Instruction/CallInstruction.cpp"
  "${SOURCE_DIR}/Instruction/CallInstructionBase.cpp"
  "${SOURCE_DIR}/Instruction/ForkInstruction.cpp"
  "${SOURCE_DIR}/Instruction/EqualiseInstruction.cpp"
  "${SOURCE_DIR}/Instruction/Instruction.cpp"
  "${SOURCE_DIR}/Instruction/JoinInstruction.cpp"
  "${SOURCE_DIR}/Instruction/ReadInstruction.cpp"
  "${SOURCE_DIR}/Instruction/ReferenceInstruction.cpp"
  "${SOURCE_DIR}/Instruction/ReleaseInstruction.cpp"
  "${SOURCE_DIR}/Instruction/ReturnInstruction.cpp"
  "${SOURCE_DIR}/Instruction/WriteInstruction.cpp"
  "${SOURCE_DIR}/Metadata/Metadata.cpp"
  "${SOURCE_DIR}/Metadata/MetadataPool.cpp"
  "${SOURCE_DIR}/Metadata/NamedMetadata.cpp"
  "${SOURCE_DIR}/Operand/FunctionOperand.cpp"
  "${SOURCE_DIR}/Operand/MemoryAtomOperand.cpp"
  "${SOURCE_DIR}/Program.cpp"
  "${SOURCE_DIR}/ProgramLinker.cpp"
  "${SOURCE_DIR}/ProgramSyntaxChecker.cpp"
  "${SOURCE_DIR}/StringPool.cpp")

add_library("steenx-program"     STATIC "${SOURCE_FILES}")
add_library("steenx-program-pic" STATIC "${SOURCE_FILES}")

set_property(TARGET "steenx-program-pic"
  PROPERTY "POSITION_INDEPENDENT_CODE" "ON")

add_executable("steenx-inspect"
  "${SOURCE_DIR}/BitfileInspector.cpp"
  "${SOURCE_DIR}/BitfileInspectorCommandLine.cpp")

target_link_libraries("steenx-inspect"
  "glog"
  "steenx-program"
  "racehog-support")
