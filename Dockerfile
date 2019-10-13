FROM ubuntu:xenial

RUN apt update && \
    apt install -y software-properties-common && \
    add-apt-repository ppa:ubuntu-toolchain-r/test && \
    apt update && \
    apt install -y cmake g++-9

ENV CXX=/usr/bin/g++-9
