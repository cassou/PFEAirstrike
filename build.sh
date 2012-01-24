#!/bin/bash

QT_CLIENT=0
LIB_ENET=0
AIRSTRIKE=0
HOME=$(pwd)
echo "Home=$HOME"

build_enet () {
    echo "Build eNet library"
    cd enet/enet-1.3.3
    autoreconf -vfi
    ./configure
    make
    if [ $? -ne 0 ]; then
	echo "Error while compiling eNet. Exiting."
	exit 0
    fi
    cd $HOME
    echo "Done"
}

check_enet () {
    echo "Checking ENet library..."
    if [ -e ./enet/enet-1.3.3/.libs/libenet.so ]||[ -e /usr/lib/i386-linux-gnu/libenet.so ]; then
	echo "OK"
	export LIBRARY_PATH=$HOME/enet/enet-1.3.3/.libs
    else
	echo "WARNING: ENet is not installed."
    fi
    
}

build_qt () {
    check_enet
    echo
    echo "Build Qt client"
    cd QtClient
    qmake
    make
    cd $HOME
    echo "Done"
}

create_package_qt () {
    echo
    echo "Create package for Qt client for Linux"
    mkdir -p Release/ClientLinux/bin
    mkdir -p Release/ClientLinux/lib
    mkdir -p Release/ClientLinux/Solo
    cp QtClient/QtClient Release/ClientLinux/bin/
    cp enet/enet-1.3.3/.libs/libenet.so Release/ClientLinux/lib/libenet.so.1
    cp Sprites/Solo/* Release/ClientLinux/Solo/
    cp QtClient/start.sh Release/ClientLinux/
    cp QtClient/lib/libQtCore.so.4.7.4 Release/ClientLinux/lib/libQtCore.so.4
    cp QtClient/lib/libQtGui.so.4.7.4 Release/ClientLinux/lib/libQtGui.so.4
    cp QtClient/README Release/ClientLinux/README
    tar -czvf Release/clientLinux.tar.gz Release/ClientLinux
    echo "Done"
}

build_airstrike () {
    check_enet
    echo
    echo "Build airstrike server"
    cd airstrike-pre6a-src
    make
    cd $HOME
    echo "Done"
}

build_all () {
    echo "Build all"
    build_enet
    build_airstrike
    build_qt
    create_package_qt
    echo "Done"
}

showmenu () {
    echo
    echo "Menu:"
    echo "1. Build Airstrike"
    echo "2. Build Qt client"
    echo "3. Build eNet library"
    echo "4. Create Qt client package for Linux"
    echo "5. All"
}

showmenu
read choices
for choice in $choices ; do
    case "$choice" in
	1)
	    build_airstrike ;;
	2)
	    build_qt ;;
	3)
	    build_enet ;;
	4)
	    create_package_qt ;;
	5)
	    build_all ;;
	*)
	    exit 0 ;;
    esac
done