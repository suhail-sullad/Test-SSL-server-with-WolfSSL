#!/bin/bash
echo "Installing WolfSSL server dependencies"
./wolfssl-3.6.8/configure  --enable-fastmath --enable-smallcache
make -C ./wolfssl-3.6.8
make -C ./wolfssl-3.6.8 install
echo "Installing WolfSSL server dependencies -Completed"
echo "Generating Certificates"
#./generate_certificate.sh

