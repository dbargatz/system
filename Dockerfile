FROM debian:bullseye@sha256:c6e865b5373b09942bc49e4b02a7b361fcfa405479ece627f5d4306554120673

# Version of LLVM/Clang to download and install.
ARG LLVM_VERSION=13

# Avoid issues with debconf asking interactive questions during installation.
ENV DEBIAN_FRONTEND=noninteractive

# Update to the latest versions of packages.
RUN apt-get update                                                  \
    && apt-get -y upgrade                                           \
    # Install tools needed for the configure script or VS Code.
    && apt-get -y install apt-utils libicu67 procps wget            \
    # Make bash the default shell instead of dash.
    && (echo "dash dash/sh boolean false" | debconf-set-selections) \
    && dpkg-reconfigure dash                                        \
    && chsh -s /bin/bash root

# Install the dependencies for compiling, debugging, and running.
RUN apt-get -y --no-install-recommends install \
    bear                                       \
    doxygen                                    \
    g++                                        \
    gdb-multiarch                              \
    git                                        \
    gnupg2                                     \
    graphviz                                   \
    grub-common                                \
    grub-pc-bin                                \
    file                                       \
    less                                       \
    lsb-release                                \
    make                                       \
    nasm                                       \
    openssh-client                             \
    python3                                    \
    python3-pip                                \
    qemu-system                                \
    software-properties-common                 \
    xorriso

# Install meson and ninja via pip.
RUN pip3 install meson ninja

# Download and install the specified version of LLVM/Clang and associated tools.
RUN wget https://apt.llvm.org/llvm.sh          \
    && chmod +x llvm.sh                        \
    && ./llvm.sh $LLVM_VERSION

# Clean up to reduce image size.
RUN apt-get autoremove -y                      \
    && apt-get clean -y                        \
    && rm -rf /var/lib/apt/lists/*

# Revert back to debconf's interactive prompts for the user.
ENV DEBIAN_FRONTEND=dialog
