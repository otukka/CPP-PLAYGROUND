FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
&& apt-get install -y \
build-essential \
cmake \
valgrind

RUN useradd builder \
&& mkdir /work \
&& chown -R builder:builder /work

USER builder

WORKDIR /work
