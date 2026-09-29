FROM debian:stable-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake \
    g++ \
    libwxgtk3.2-dev \
    gettext \
    librsvg2-bin \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
