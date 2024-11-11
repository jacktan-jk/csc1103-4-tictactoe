#!/bin/bash

# Function to check if running in WSL
is_wsl() {
    grep -qE "(Microsoft|WSL)" /proc/version
}

# Function to show usage
usage() {
    echo "Usage: $0 [-s]"
    echo "  -s : Save the Docker image after building"
    exit 1
}

# Check for options
SAVE_IMAGE=false
while getopts ":s" opt; do
    case ${opt} in
        s )
            SAVE_IMAGE=true
            ;;
        \? )
            usage
            ;;
    esac
done

rm docker_tictactoe.tar 2>/dev/null
rm tictactoe.exe 2>/dev/null

dos2unix compile.sh

# Build the Docker image
echo "[DOCKER] Building image..."
if ! docker build -t docker_tictactoe .; then
    echo "[DOCKER] Docker image build failed!"
    exit 1
fi
if [ "$SAVE_IMAGE" = true ]; then
	echo "[DOCKER] Saving Image..."
	if ! docker save -o docker_tictactoe.tar docker_tictactoe; then
        echo "[DOCKER] Failed to save Docker image!"
	fi
fi

OS=$(uname)

if [ "$OS" = "Linux" ]; then
    echo "[DOCKER] Running container..."
    sudo xhost +local:
    export DISPLAY=:0
    sudo docker run -it --user $(id -u):$(id -g) -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix docker_tictactoe

elif [[ "$OS" == *"_NT"* ]]; then
    # Run the Docker command inside WSL
    echo "[DOCKER] Opening WSL to run container..."

    # Construct the Docker run command
    DOCKER_RUN_CMD="export DISPLAY=:0 && docker run -it -e DISPLAY=\$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix docker_tictactoe"

    # Use `wsl` command to open WSL and run the Docker command
    wsl bash -c "$DOCKER_RUN_CMD"

fi



echo "[DOCKER] Container ran successfully."

