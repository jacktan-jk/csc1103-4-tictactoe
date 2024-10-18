# **TIC TAC TOE - CSC1103 & CSC1104 Project**
## Installation Instructions (Linux)

To get started with the project, ensure you have the following installed:
```
sudo apt-get install -y \
       docker-ce \
       x11-xserver-utils \
```
### Building the Project via Docker
After setting up WSL2 and Docker, you can choose to either load a Docker image or build the Docker image yourself.
#### Loading Docker Image
```
sudo docker load -i FILE_NAME.tar
```

#### Building Docker Image
```
sudo ./run_docker.sh
```
> [!WARNING]
> **Warning for Docker Building:**
> If `compile.sh` or `run_docker.sh` is not found after building, run the following command to convert it to Unix line endings:
> ```
> dos2unix SCRIPT_NAME.sh
> ```

> [!TIP]
> **Saving Docker Image**
> ```
> ./run_script.sh -s
> ```

## Installation Instructions (Windows)

To get started with the project, ensure you have the following installed:

1. **Docker Desktop**
   - [Install Docker Desktop](https://www.docker.com/products/docker-desktop)

2. **WSL2 (Windows Subsystem for Linux)**
   - Follow [Microsoft's guide to install WSL2](https://docs.microsoft.com/en-us/windows/wsl/install)
   - Install Ubuntu with the following command:
     ```
     wsl --install -d Ubuntu
     ```
   - Set Ubuntu as your default WSL distribution:
     ```
     wsl --set-default Ubuntu
     ```
   - Check default:
     ```
     wsl -l -v
     ```

### Building the Project via Docker
After setting up WSL2 and Docker, you can choose to either load a Docker image or build the Docker image yourself.
#### Loading Docker Image
```
docker load -i FILE_NAME.tar
```

#### Building Docker Image
```
./run_docker.sh
```

> [!WARNING]
> **Warning for Docker Building:**
> If `compile.sh` or `run_docker.sh` is not found after building, run the following command to convert it to Unix line endings:
> ```
> dos2unix SCRIPT_NAME.sh
> ```

> [!TIP]
> **Saving Docker Image**
> ```
> ./run_script.sh -s
> ```

### Build and Run the Project in Windows
> [!CAUTION]
> **Not recommended**, unless you know what you're doing and install the right packages. (refer Dockerfile)
> ```
> ./compile.sh
> ```

## **BASIC REQUIREMENTS** (BOTH)
- [X] GUI (GTK)
- [X] 2 Player Mode
- [X] 1 Player Mode ("Perfect" Minimax)
- [X] Winning Logic
- [ ] GUI indication when player WIN (e.g, blinking))

## **[^1]PM-CSC1103 REQUIRMENTS**
- [X] Improve Minimax memory usage
- [ ] Implement ML Algorithm (80:20)
    > [TIP]
    > Linear regression, Navie bayes, Neural network and Reinforcement learning
- [ ] Plot the confusion matrix for the training and testing accuracy
- [ ] Calculate the number of times the computer wins as a gauge of difficulty level. 
 
## **[^2]COA-CSC1104 REQUIRMENTS**
- [ ] Replace one function with assembly
    > [TIP]
    > Use inline assembly code in C source file, or linking separate C and assembly object files.

[^1]: PROGRAMMING METHODOLOGY.
[^2]: COMPUTER ORGANIZATION AND ARCH.
