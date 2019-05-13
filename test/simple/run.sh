#!/bin/bash
set -exo pipefail

# ------------------------------------------------------------------------------
# User defines:

run_make="1"
run_compiler="1"
run_translation="1"
run_inspect="0"
run_vm="1"

exe_version="debug"
exe_version="release"

extra_flags=""
extra_flags="-g"

vm_flags=""
# vm_flags="--interactive"

programs=(
    "thesis.2019"
    # "thesis.2019.100k"
    # "01.hello-racehog"
    # "02.hello-threads"
    # "03.global-aggregates"
    # "04.simple-functionptr"
    # "05.aggregate-pointers"
    # "06.bitcast-funcptr"
    # "07.hello-locks"
    # "08.hello-malloc"
    # "09.thread-layouts"
    # "10.hello-atomics"
    # "11.function-dispatch"
    # "12.static-local"
    # "13.global-inits"
)

# ------------------------------------------------------------------------------
# Program defines:

build_path="../../build/${exe_version}"
lib_path="${build_path}/lib"
translation_pass="${lib_path}/libsteenx-translation.so"
opt_exe="opt"
inspect_exe="${build_path}/bin/inspect"
racehog_exe="${build_path}/bin/racehog"

# ------------------------------------------------------------------------------
# Logic:

dir="${PWD}"
script_dir=$(dirname $(readlink -f "${0}"))

cd "${script_dir}"

if [[ "${run_make}" == "1" ]]; then
    cd "${build_path}"
    make -j$(nproc)
    cd "${script_dir}"
fi

for p in "${programs[@]}"; do

    if [[ "${run_compiler}" == "1" ]]; then
        clang ${extra_flags} -S            -pthread "${p}.c"
        clang ${extra_flags} -S -emit-llvm -pthread "${p}.c"
    fi

    if [[ "${run_translation}" == "1" ]]; then
        time "${opt_exe}"                \
             -load "${translation_pass}" \
             -steenx-translation         \
             -xirfile "${p}.xir"         \
             -o /dev/null                \
             < "${p}.ll"
    fi

    if [[ "${run_inspect}" == "1" ]]; then
        time "${inspect_exe}" "${p}.xir"
    fi

    if [[ "${run_vm}" == "1" ]]; then
        time "${racehog_exe}" ${vm_flags} "${p}.xir"
    fi
done

cd "${dir}"
