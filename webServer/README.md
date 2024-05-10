# C/C++ Socker based Web Server
This project entails an HTTP TCP web server that is capable of handling HTTP requests asynchronously on port 9999. 
Additionally, it includes support for Docker and Docker Swarm configurations.
Web Server support different listener mode (select(), poll(), epoll()).

## Prerequisites
Before running the server, ensure that MySQL server is installed. 
Configure user credentials by replacing "CONFIGURE_USERNAME" and "CONFIGURE_PASSWORD" in the "webServer/src/Modules/MySQLConnector.cpp" file, and ensure the same username/password combination is configured in the MySQL database. 
Additionally, make sure the following packages are installed on your Linux system:

  - OpenSSL library packages
  - Crypt library

## Contact
For further discussions or inquiries, please feel free to reach out via email at v.cse59@gmail.com.
