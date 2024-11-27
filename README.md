# **TIC TAC TOE - CSC1103 & CSC1104 Project**
## Installation Instructions (Linux)

To get started with the project, ensure you have the following installed:

> [!IMPORTANT]
> Ensure that you follow all the instruction below!

1. **Setup Docker's apt repository**
```
 # Add Docker's official GPG key:
 sudo apt-get update
 sudo apt-get install ca-certificates curl
 sudo install -m 0755 -d /etc/apt/keyrings
 sudo curl -fsSL https://download.docker.com/linux/debian/gpg -o /etc/apt/keyrings/docker.asc
 sudo chmod a+r /etc/apt/keyrings/docker.asc
 # Add the repository to Apt sources:
 echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/debian \
  $(. /etc/os-release && echo "$VERSION_CODENAME") stable" | \
sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
sudo apt-get update
```

2. **Installing xhost(X11)**
```
sudo apt-get install x11-apps
sudo apt-get install x11-xserver-utils
```
2. **Installing Docker Desktop**
```
sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
```
  
> [!TIP]
> Enable Docker service on boot. Make your life easier.
> ```
>  sudo systemctl enable docker.service
>  sudo systemctl enable containerd.service
> ```
>  To stop this behavior, use disable instead.
> ```
>  sudo systemctl disable docker.service
>  sudo systemctl disable containerd.service
> ```

### Building the Project via Docker (Linux)
After setting up WSL2 and Docker, you can choose to either load a Docker image or build the Docker image yourself.
#### **[OPTIONAL]** Loading Docker Image
```
sudo docker load -i FILE_NAME.tar
```

#### Build and Run TicTacToe Application on Docker (Linux)
```
sudo ./run_docker.sh
```
> [!WARNING]
> **Warning for Docker Building:**
> 
> If `compile.sh` or `run_docker.sh` is not found and it's clearly in the directory, run the following command to convert it to Unix line endings:
> ```
> dos2unix SCRIPT_NAME.sh
> ```

> [!TIP]
> **Saving Docker Image**
> ```
> ./run_docker.sh -s
> ```
> **Run Docker Verbose**
> ```
> ./run_docker.sh -d
> ```
> 
## Installation Instructions (MacOS) 

To get started with the project, ensure you have the following installed:

1. **Docker Desktop**
Select based on your specs.
     - [Install Docker Desktop](https://www.docker.com/products/docker-desktop)
     -  Select "Download for Mac - Intel Chip"
     -  Select "Download for Mac - Apple Silicon"

2. **Installation Docker Desktop**
     - Go to "Downloads" and double click "Docker.dmg"
     - Drag and Drop "Docker" to "Application"
     - "Launch Pad" either search or swipe to find "Docker"

3. **Verify Docker Desktop**
     - Launch the application and select "Recommended Settings"
                          OR
     - Type the command in terminal "docker --version"

4. **Verify Docker Image and Containers**
Now create a new directory/folder and "cd" to that folder.
     inside terminal use the following command and wait for "Download complete"
     - docker pull hello-world
     - Verify docker image in "Docker Desktop"

Under the action tab click on "Run"
          OR
From terminal type the following.
```
  docker run hello-world
```
To verifiy container.
Switch to the containers tab.
     Under the name tab and click on the "highlighted blue" and view the logs.
         OR
Use the following command.
```
  docker ps -a
```

### Run TicTacToe Application on Docker (MacOS)
> [!IMPORTANT]
> **Pre-requisite application to be install.
>
> **BEFORE BUILDING DOCKER IMAGE OR CONTAINER.

1. Install brew
     - [Install brew](https://brew.sh)

2. install xquartz from terminal
     ```
      brew install --cask xquartz
     ```
3. xquarts settings
     - setting -> security -> allow connections from network clients
     - Restart xquartz
4. Terminal command
     ```
      xhost + 127.0.0.1
     ```
5. Build and Run Docker
     ```
      docker-compose up --build -d
     ```

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

### Building the Project via Docker (Windows)
After setting up WSL2 and Docker, you can choose to either load a Docker image or build the Docker image yourself.
#### **[OPTIONAL]** Loading Docker Image
```
docker load -i FILE_NAME.tar
```

#### Build and Run TicTacToe Application on Docker (Windows)
```
./run_docker.sh
```

> [!WARNING]
> **Warning for Docker Building:**
> If `compile.sh` or `run_docker.sh` is not found and it's clearly in the directory, run the following command to convert it to Unix line endings:
> ```
> dos2unix SCRIPT_NAME.sh
> ```

> [!TIP]
> **Saving Docker Image**
> ```
> ./run_docker.sh -s
> ```
> **Run Docker Verbose**
> ```
> ./run_docker.sh -d
> ```
> 
### Build and Run the Project in Windows (w/o Docker)
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
- [X] GUI indication when player WIN (e.g, blinking))

## **[^1]PM-CSC1103 REQUIRMENTS**
- [X] Improve Minimax memory usage
- [X] Implement ML Algorithm (80:20)
    > [TIP]
    > Linear regression, Navie bayes, Neural network and Reinforcement learning
- [X] Plot the confusion matrix for the training and testing accuracy
- [X] Calculate the number of times the computer wins as a gauge of difficulty level. 
 
## **[^2]COA-CSC1104 REQUIRMENTS**
- [X] Replace one function with assembly
    > [TIP]
    > Use inline assembly code in C source file, or linking separate C and assembly object files.

[^1]: PROGRAMMING METHODOLOGY.
[^2]: COMPUTER ORGANIZATION AND ARCH.
