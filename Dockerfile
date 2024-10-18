FROM ubuntu:22.04

# Install necessary packages, including development tools and libraries
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    pkg-config \
    libgtk-3-dev \
    libsystemd-dev \
    libjpeg-dev \
    libwebp-dev \
    libzstd-dev \
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

# Set the command to run the application (instead of re-running compile.sh)
CMD ["./tictactoe"]


