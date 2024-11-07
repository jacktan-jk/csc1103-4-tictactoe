#!/bin/bash
target_exe=$1

ldd $target_exe > ldd_output.txt
# Directory to copy the DLLs to
target_dir="./dlls"

# Create the target directory if it doesn't exist
mkdir -p $target_dir

# Parse the ldd output and copy the DLLs
while IFS= read -r line; do
    # Extract the DLL path (anything after '=>' and before the first '(')
    dll_path=$(echo $line | grep -oP '=> \K.*(?= \()')
    
    # Check if the DLL path is non-empty and copy the DLL
    if [[ -n "$dll_path" ]]; then
        echo "Copying $dll_path to $target_dir"
        cp "$dll_path" "$target_dir"
    fi
done < ldd_output.txt

rm ldd_output.txt
