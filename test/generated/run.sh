#!/bin/bash
set -exo pipefail

# ------------------------------------------------------------------------------
# User defines:

output_path="../simple"
py_scripts=("thesis.2019.100k")

# ------------------------------------------------------------------------------
# Logic:

dir="${PWD}"
cd $(dirname $(readlink -f "${0}"))

for s in "${py_scripts[@]}"; do
    python3 "${s}.py" > "${output_path}/${s}.c"
done

cd "${dir}"
