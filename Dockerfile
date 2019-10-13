FROM ubuntu:xenial

RUN add-apt-repository ppa:ubuntu-toolchain-r/test \
    apt update \
    apt install g++-9

ENV CXX=/usr/bin/g++-9
