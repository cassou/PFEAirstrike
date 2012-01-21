#!/bin/bash

QT_CLIENT=0
LIB_ENET=0
AIRSTRIKE=0
HOME=$(pwd)
echo "Home=$HOME"

build_enet () {
    echo "Build eNet library"
    cd enet/enet-1.3.3
    ./configure
    make
    cd $HOME
    echo "Done"
}

check_enet () {
    if [ -e ./enet/enet-1.3.3/.libs/libenet.so ]; then
	echo "eNet ok"
	return 1
    else
	echo "eNet not ok"
	build_enet
	return 0
    fi
    
}

build_qt () {
    check_enet
    echo
    echo "Build Qt client"
    cd QtClient
    qmake
    make
    cd ..
    echo "Done"
}

build_airstrike () {
    check_enet
    echo
    echo "Build airstrike server"
    cd airstrike-pre6a-src
    make
    echo "Done"
}

build_all () {
    echo "Build all"

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
    echo "6. Quit"
}

check_enet
while true ; do
    showmenu
    read choices
    for choice in $choices ; do
	case "$choice" in
	    1)
		build_airstrike ;;
	    2)
		build_qt ;;
	    3)
		echo "Number Three" ;;
	    4)
		echo "Numbers One, two, three" ;;
	    5)
		echo "5" ;;
	    6)
		echo "Exit"
		exit 0 ;;
	    *)
		echo "Please enter number ONLY ranging from 1-5!"
		;;
	esac
    done
done