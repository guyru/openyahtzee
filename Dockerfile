FROM debian:stable-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake \
    g++ \
    libwxgtk3.2-dev \
    gettext \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
