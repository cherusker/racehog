set("SOURCE_DIR" "${PROJECT_SOURCE_DIR}/SteenX/LLVM")

add_library("steenx-translation" MODULE
  "${SOURCE_DIR}/AtomicLocksTable.cpp"
  "${SOURCE_DIR}/CallHandler/CallHandlerRegistry.cpp"
  "${SOURCE_DIR}/CallHandler/MallocCallHandler.cpp"
  "${SOURCE_DIR}/CallHandler/PthreadCreateCallHandler.cpp"
  "${SOURCE_DIR}/CallHandler/PthreadJoinCallHandler.cpp"
  "${SOURCE_DIR}/CallHandler/PthreadMutexLockCallHandler.cpp"
  "${SOURCE_DIR}/CallHandler/PthreadMutexUnlockCallHandler.cpp"
  "${SOURCE_DIR}/CallHandler/RegularCallHandler.cpp"
  "${SOURCE_DIR}/FunctionTable.cpp"
  "${SOURCE_DIR}/GetElementPtrExtraction.cpp"
  "${SOURCE_DIR}/GlobalsInitialiser.cpp"
  "${SOURCE_DIR}/InstructionExtractor.cpp"
  "${SOURCE_DIR}/Memory/MemoryAllocator.cpp"
  "${SOURCE_DIR}/Memory/MemoryManager.cpp"
  "${SOURCE_DIR}/MetadataExtractor.cpp"
  "${SOURCE_DIR}/OperandTranslator.cpp"
  "${SOURCE_DIR}/SteenXTranslationPass.cpp"
  "${SOURCE_DIR}/TypeSizesTable.cpp")

# "SYSTEM" hides warnings that stem from LLVM (and we cannot change ...)

target_include_directories("steenx-translation"
  SYSTEM PRIVATE "${LLVM_INCLUDE_DIRS}")

target_link_libraries("steenx-translation"
  "glog"
  "steenx-program-pic" # link before `racehog-support-pic'
  "racehog-support-pic")
