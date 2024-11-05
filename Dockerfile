# Use Ubuntu as the base image
FROM --platform=linux/arm/v7 ubuntu:22.04

# Enable ARM architecture
RUN dpkg --add-architecture armhf

# Update the package lists to include the new architecture
RUN apt-get update

# Install necessary packages, including development tools and libraries
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    build-essential \
    gcc-arm-linux-gnueabihf \
    pkg-config \
    libc6-dev-armhf-cross \
    libgtk-3-dev:armhf \  
    libc6-dev \
    libsystemd-dev \
    libjpeg-dev \
    libwebp-dev \
    libzstd-dev \
    x11-apps \
    xorg \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Copy the source code and compile script to the container
COPY . /app

# Set the working directory inside the container
WORKDIR /app
RUN ls -al
RUN chmod 777 /app/compile.sh

# Compile the application
RUN /app/compile.sh
RUN chmod +x /app/tictactoe

# Set the command to run the application (instead of re-running compile.sh)
CMD ["./tictactoe"]


