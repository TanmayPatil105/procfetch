FROM ubuntu:latest

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update &&\
    apt-get install -y --no-install-recommends \
            adduser sudo \
            make g++ gdb \
            clang-format \
            doxygen graphviz &&\
    apt-get -y clean &&\
    rm -rf /var/lib/apt/lists/*

RUN adduser --disabled-password --gecos '' user
RUN echo 'user ALL=(root) NOPASSWD:ALL' > /etc/sudoers.d/user

WORKDIR /home/user
USER user
