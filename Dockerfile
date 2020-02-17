FROM ubuntu:xenial

USER root

# Install cmake and g++9
RUN apt update && \
    apt install -y software-properties-common apt-transport-https ca-certificates \ 
        gnupg software-properties-common wget libsfml-dev git && \
    add-apt-repository ppa:ubuntu-toolchain-r/test && \
    wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | apt-key add - && \
    apt-add-repository 'deb https://apt.kitware.com/ubuntu/ xenial main' && \
    apt update && \
    apt install -y cmake g++-9

RUN useradd -ms /bin/bash dev
ENV CXX /usr/bin/g++-9 
ENV DISPLAY :0

USER dev
RUN mkdir /home/dev/.pulse

