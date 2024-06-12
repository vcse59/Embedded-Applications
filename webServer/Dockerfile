# Use a base image with necessary build tools and dependencies
FROM gcc:latest

# Set the working directory in the container
WORKDIR /app

# Install CMake
RUN apt-get update && apt-get install -y cmake

# Set environment variable to specify custom library path
ENV LD_LIBRARY_PATH=/app/lib/mysql-lib/Ubuntu/mysql-connector-cpp/lib/x86_64-linux-gnu:${LD_LIBRARY_PATH}
ENV CPLUS_INCLUDE_PATH=/app/lib/mysql-lib/Ubuntu/mysql-connector-cpp/include/mysql-cppconn-8/jdbc/cppconn:${CPLUS_INCLUDE_PATH}
ENV CPLUS_INCLUDE_PATH=/app/lib/mysql-lib/Ubuntu/mysql-connector-cpp/include/mysql-cppconn-8/jdbc:${CPLUS_INCLUDE_PATH}
ENV CPLUS_INCLUDE_PATH=/app/build/server/thirdparty/jsoncpp/include:${CPLUS_INCLUDE_PATH}

# Copy the C++ source code into the container
COPY . /app

# set the execute permission on bash script
RUN chmod +x INSTALL.sh
RUN ["./INSTALL.sh"]

# Execute the binary
CMD ["./bin/server"]
