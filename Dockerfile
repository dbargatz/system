FROM debian:bullseye-20220509

# Version of LLVM/Clang to download and install.
ARG LLVM_VERSION=14
ARG LLVM_URL=https://apt.llvm.org/llvm.sh

# Avoid issues with debconf asking interactive questions during installation.
ENV DEBIAN_FRONTEND=noninteractive

# Update to the latest versions of packages, then install the dependencies for
# compiling, debugging, and running.
RUN set -x                                             \
    && apt-get update                                  \
    && apt-get upgrade --yes                           \
    && apt-get install --yes --no-install-recommends   \
      bear                                             \
      curl                                             \
      doxygen                                          \
      gdb-multiarch                                    \
      git                                              \
      gnupg2                                           \
      graphviz                                         \
      grub-common                                      \
      file                                             \
      less                                             \
      lsb-release                                      \
      make                                             \
      nasm                                             \
      ninja-build                                      \
      openssh-client                                   \
      python3                                          \
      python3-pip                                      \
      qemu-system-arm                                  \
      qemu-system-x86                                  \
      software-properties-common                       \
      wget                                             \
      xorriso                                          \
    && apt-get autoremove -y                           \
    && apt-get clean -y                                \
    && rm -rf /var/lib/apt/lists/*

# Install a more up-to-date version of Meson than is available in Debian repos.
RUN set -x                \
    && pip install meson  \
    && meson --version

# Download and install the specified version of LLVM/Clang and associated tools.
WORKDIR /workdir
RUN set -x                                                                \
    && curl -sSLf -o llvm.sh ${LLVM_URL}                                  \
    && chmod +x llvm.sh                                                   \
    && ./llvm.sh ${LLVM_VERSION}                                          \
    && ln -s /usr/bin/clang-${LLVM_VERSION} /usr/bin/clang                \
    && ln -s /usr/bin/clang++-${LLVM_VERSION} /usr/bin/clang++            \
    && ln -s /usr/bin/ld.lld-${LLVM_VERSION} /usr/bin/ld.lld              \
    && ln -s /usr/bin/llvm-ar-${LLVM_VERSION} /usr/bin/llvm-ar            \
    && ln -s /usr/bin/llvm-objcopy-${LLVM_VERSION} /usr/bin/llvm-objcopy  \
    && ln -s /usr/bin/llvm-strip-${LLVM_VERSION} /usr/bin/llvm-strip      \
    && rm -rf /workdir

# Revert back to debconf's interactive prompts for the user.
ENV DEBIAN_FRONTEND=dialog
