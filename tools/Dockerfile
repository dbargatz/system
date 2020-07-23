FROM debian:stable

ARG CONFIGURE_SCRIPT="https://raw.githubusercontent.com/dbargatz/system/master/tools/configure"

# Avoid issues with debconf asking interactive questions during installation.
ENV DEBIAN_FRONTEND=noninteractive

# Update to the latest versions of packages.
RUN apt-get update \
    && apt-get -y upgrade \
    # Install tools needed for the configure script or VS Code.
    && apt-get -y install apt-utils libicu63 procps wget \
    # Make bash the default shell instead of dash.
    && (echo "dash dash/sh boolean false" | debconf-set-selections) \
    && dpkg-reconfigure dash \
    && chsh -s /bin/bash root \
    # Download and run the configure script to install all the needed tools.
    && wget --quiet --output-document=/tmp/configure.sh $CONFIGURE_SCRIPT \
    && /bin/bash /tmp/configure.sh \
    && rm /tmp/configure.sh \
    # Clean up to reduce image size.
    && apt-get autoremove -y \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*

# Revert back to debconf's interactive prompts for the user.
ENV DEBIAN_FRONTEND=dialog
