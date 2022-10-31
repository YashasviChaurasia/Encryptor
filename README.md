# Encryptor
C code to encrypt FILES in chunks using AES GCM

Use docker file to build image: 
`docker build -t Encryptor .`

then use `docker run -it --cpuset-cpus=0 Encryptor`

Use dockerhub to access container or Simply access container and run ./a.out file.

## Working Inside a Container
1. container should be running 
1. if not use `ENTRYPOINT ["tail", "-f", "/dev/null"]` inside dockerfile as it keeps container running
1. now use another terminal and do `docker ps` as this gives you container id for running containers
1. `docker exec -it <container id> /bin/sh` this gets you into docker container , Enjoyyyyy
1. `docker stop <containerId>`: just in case you are wondering how to close the container..

### Tip: when working on Remote servers ,simply use vscode ssh integration as it sorts out stuff with much needed text editor GUI

### Tip: You can solely use container and work in it like a happy person
