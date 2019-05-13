#!/bin/bash
set -exo pipefail

dir="${PWD}"

rmdirs=(
    $(readlink -f "build/debug")
    $(readlink -f "build/release")
    $(readlink -f "docs/html")
    $(readlink -f "docs/latex")
)

patterns=(
    "*~"
    "*.bc"
    "*.ll"
    "*.rh"
    "*.s"
)

for p in "${patterns[@]}"; do
    find .             \
         -type f       \
         -name "${p}"  \
         -exec rm {} \;
done

for d in "${rmdirs[@]}"; do
    if test -d "${d}"; then
        rm -r "${d}"
    fi
done

cd "${dir}"
