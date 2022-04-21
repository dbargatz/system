FROM debian:bullseye-20220418

# Version of LLVM/Clang to download and install.
ARG LLVM_VERSION=14
ARG LLVM_URL=https://apt.llvm.org/llvm.sh

# Version of build2 to download and install. Hash from installation instructions
# at https://build2.org/install.xhtml.
ARG BUILD2_VERSION=0.14.0
ARG BUILD2_URL=https://download.build2.org/${BUILD2_VERSION}/build2-install-${BUILD2_VERSION}.sh
ARG BUILD2_HASH=f2e0795fda1bdc6b6ea4d2fc5917469725c20962bb1f6672c8d2462d76b3a7db

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
      qemu-system-arm                                  \
      qemu-system-x86                                  \
      software-properties-common                       \
      wget                                             \
      xorriso                                          \
    && apt-get autoremove -y                           \
    && apt-get clean -y                                \
    && rm -rf /var/lib/apt/lists/*

# Download and install the specified version of LLVM/Clang and associated tools.
WORKDIR /workdir
RUN set -x                                                     \
    && curl -sSLf -o llvm.sh ${LLVM_URL}                       \
    && chmod +x llvm.sh                                        \
    && ./llvm.sh ${LLVM_VERSION}                               \
    && ln -s /usr/bin/clang-${LLVM_VERSION} /usr/bin/clang     \
    && ln -s /usr/bin/clang++-${LLVM_VERSION} /usr/bin/clang++ \
    && ln -s /usr/bin/lld-${LLVM_VERSION} /usr/bin/lld         \
    && rm -rf /workdir

# Download, compile, and install the specified version of build2.
WORKDIR /workdir
RUN set -x                                                      \
    && curl -sSLf -o build2.sh ${BUILD2_URL}                    \
    && echo "${BUILD2_HASH} build2.sh" | sha256sum --check      \
    && chmod +x build2.sh                                       \
    && ./build2.sh --yes --trust yes --cxx /usr/bin/clang++     \
    && rm -rf /workdir

# Revert back to debconf's interactive prompts for the user.
ENV DEBIAN_FRONTEND=dialog
