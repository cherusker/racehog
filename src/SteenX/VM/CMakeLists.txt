set("SOURCE_DIR" "${PROJECT_SOURCE_DIR}/SteenX/VM")

add_library("steenx-vm" STATIC
  "${SOURCE_DIR}/Environment.cpp"
  "${SOURCE_DIR}/Equaliser.cpp"
  "${SOURCE_DIR}/Execution.cpp"
  "${SOURCE_DIR}/ExecutionInteractor.cpp"
  "${SOURCE_DIR}/Memory/MemoryAccess.cpp"
  "${SOURCE_DIR}/Memory/MemoryAtom.cpp"
  "${SOURCE_DIR}/Memory/MemoryStore.cpp"
  "${SOURCE_DIR}/Thread.cpp"
  "${SOURCE_DIR}/VM.cpp")

add_library("steenx-builder" STATIC
  "${SOURCE_DIR}/SteenXBuilder.cpp")
