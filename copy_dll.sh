#!/bin/bash

# Get the target executable from the first argument
target_exe="$1"

# Check if the target executable exists
if [[ ! -f "$target_exe" ]]; then
    echo "Error: Target executable '$target_exe' not found."
    exit 1
fi

# Run ldd to list the shared libraries and save the output to a file
ldd "$target_exe" > ldd_output.txt

# Directory to copy the DLLs to
target_dir="./dlls"

# Create the target directory if it doesn't exist
mkdir -p "$target_dir"

# Parse the ldd output and copy the DLLs
while IFS= read -r line; do
    # Extract the DLL path (anything after '=>' and before the first '(')
    dll_path=$(echo "$line" | grep -oP '=> \K.*(?= \()')

    # Check if the DLL path is non-empty and file exists
    if [[ -n "$dll_path" && -f "$dll_path" ]]; then
        echo "Copying $dll_path to $target_dir"
        cp "$dll_path" "$target_dir"
    elif [[ -n "$dll_path" ]]; then
        echo "Warning: DLL '$dll_path' does not exist."
    fi
done < ldd_output.txt

# Clean up by removing the ldd output file
rm ldd_output.txt

echo "DLLs copied to $target_dir"