# C/C++ Socket based web server

This project involves developing an HTTP TCP web server capable of asynchronously handling HTTP requests on port 9999. 
It also provides support for Docker and Docker Swarm configurations. 
The web server supports different listener modes such as select(), poll(), and epoll().

## Prerequisites
Before running the server, ensure that MySQL server is installed.
Configure user credentials by replacing "CONFIGURE_USERNAME" and "CONFIGURE_PASSWORD" in the "webServer/src/Modules/MySQLConnector.cpp" file, and ensure the same username/password combination is configured in the MySQL database. Additionally, ensure the following packages are installed on your Linux system:

  -  OpenSSL library packages
  -  Crypt library

## Build Steps

  -  Navigate to the **webServer** directory.
  -  Execute the bash shell script **./INSTALL.sh** to compile the source code and link object files to generate the executable/binary.
  -  Execute **./startServer.sh** to run the binary and launch the web server to listen on port number **9999**.
  -  Execute **./startClient.sh** in a new tab/window shell to send requests to the web server on port number **9999** for testing.

## Steps to Create Docker Image and Push/Pull it to/from Docker Hub

  -  Navigate to the webServer directory.
    
  -  Build the Docker image using the command **sudo docker build -t <image-name>:<tag> .**
      -  Example: $sudo docker build -t webServer:0.1 .
    
  -  Test the Docker image by running the command **sudo docker run -it -p <host-port>:<container-port> <image-name>:<tag>**
      -  Example: $sudo docker run -it -p 9999:80 webServer:0.1
        
  -  Authenticate the Docker Hub user by running the command** sudo docker login**.
      -  You may need to enter Docker Hub credentials for the first time.
        
  -  Tag the Docker image using the command **sudo docker tag <image-name>:<tag> <username>/<image-name>:<tag>**
      -  Example: $sudo docker tag webServer:0.1 vivek/webServer:0.1
        
  -  Push the Docker image to Docker Hub using the command **sudo docker push <username>/<image-name>:<tag>**
      -  Example: $sudo docker push vivek/webServer:0.1
        
  -  To download the Docker image on other systems, execute the command **sudo docker pull <username>/<image-name>:<tag>**
      -  Example: $sudo docker pull vivek/webServer:0.1
        
  -  To execute the image pulled from Docker Hub, run the command **sudo docker run -it -p <host-port>:<container-port> <username>/<image-name>:<tag>**
      -  Example: $sudo docker run -it -p 9999:80 vivek/webServer:0.1

## Contact
For further discussions or inquiries, please feel free to reach out via email at **v.cse59@gmail.com**.
