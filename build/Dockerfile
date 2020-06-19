FROM debian:stable

ARG CONFIGURE_SCRIPT="https://raw.githubusercontent.com/dbargatz/system/master/configure"

# Update to the latest versions of packages.
RUN apt-get update \
    # Install tools needed for the configure script or VS Code.
    && apt-get -y install wget \
    # Download and run the configure script to install all the needed tools.
    && wget --quiet --output-document=/tmp/configure.sh $CONFIGURE_SCRIPT \
    && /bin/bash /tmp/configure.sh \
    && rm /tmp/configure.sh \
    # Clean up to reduce image size.
    && apt-get autoremove -y \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*
