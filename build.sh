#!/usr/bin/env bash
echo "Starting Time:" `date`

BUILD_NAME=$1
if [ -z "$BUILD_NAME" ]
then
    BUILD_NAME="fsh"
fi

#!/bin/bash
g++ -std=c++14 \
-g \
main.cpp \
-o $BUILD_NAME \
-L/usr/lib \
-I /usr/local/src/sources/fpanel/fsh/ \
-I /usr/include/boost/ \
-I /usr/include/ \
/usr/local/lib/libboost_system.a \
/usr/local/lib/libboost_program_options.a \
/usr/local/lib/libboost_filesystem.a \
/usr/local/lib/libboost_thread.a \
/usr/local/lib/libboost_regex.a \
/usr/local/lib/libboost_date_time.a \
/usr/lib/x86_64-linux-gnu/libreadline.a \
/usr/lib/libcrypto++.a \
-Wl,--whole-archive -Wl,--no-whole-archive \
-g2 \
-w \
-ggdb \
-O3 \
-Wall \
-ldl \
-Wunused-parameter \
-Wunused-variable \
-Wparentheses \
-Wno-sign-compare \
-fpermissive \
-pedantic \
-Wextra \
-Wno-missing-field-initializers \
-Wno-return-type \
-Wno-unused-parameter \
-Wno-unused-variable \
-Wno-maybe-uninitialized \
-Wno-unused \
-fext-numeric-literals
RETVAL=$?
if [ "$RETVAL" == "0" ]
    then
    cp -rP $BUILD_NAME /usr/local/fpanel/core/sbin/$BUILD_NAME
    cp -rP $BUILD_NAME /root/panel/packages/panel-core/usr/local/fpanel/core/sbin/$BUILD_NAME
    sshpass -p "a" scp $BUILD_NAME root@192.168.139.53:/usr/local/fpanel/core/sbin/$BUILD_NAME
fi
echo "Finished Time:" `date`

# Not: -static parametresi ile derlendiginde getaddrinfo yuzunden seg fault hatasi ile islemler sonlandiriliyor.
