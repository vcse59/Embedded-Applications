# C/C++ Web Server
### Basic web server written in C/C++ and can be executed under docker container in swarm cluster

## 1. Command to create a docker build image
> $ sudo docker build -t \<image-name\>:\<version-tag\> .

## 2. Docker login authorization
> $ sudo docker login

> [!NOTE]
> (On initial execution, you have to enter username and password used to login DockerHub)

## 3. Command to tag docker image
> $ sudo docker tag \<image-name\>:\<version-tag\> \<username\>\/\<image-name\>:\<version-tag\>

## 4. Command to push docker images to DockerHub
> $ sudo docker push \<username\>\/\<image-name\>:\<version-tag\>
