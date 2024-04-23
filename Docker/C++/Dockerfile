# Use a base image with necessary build tools and dependencies
FROM gcc:latest

# Set the working directory in the container
WORKDIR /app

# Copy the C++ source code into the container
COPY main.cpp .

# Compile the C++ application
RUN g++ -o main main.cpp

# Specify the command to run the C++ application
CMD ["./main"]
