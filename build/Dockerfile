FROM debian:10

ARG CONFIGURE_SCRIPT="https://raw.githubusercontent.com/dbargatz/system/master/configure"

# Avoid warnings by switching to noninteractive
ENV DEBIAN_FRONTEND=noninteractive

# Update and upgrade to the latest versions of packages.
RUN apt-get update \
    && apt-get -y upgrade \
    #
    # Install tools needed for the configure script.
    && apt-get -y install curl less \
    #
    # Download and run the configure script to install all the needed tools.
    && apt-get -y install curl less \
    && curl --silent --output /tmp/configure.sh $CONFIGURE_SCRIPT \
    && /bin/bash /tmp/configure.sh \
    && rm /tmp/configure.sh \
    #
    # Clean up to reduce image size.
    && apt-get autoremove -y \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*

# Switch back to dialog for any ad-hoc use of apt-get
ENV DEBIAN_FRONTEND=dialog