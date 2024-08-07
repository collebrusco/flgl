#!/bin/bash

if [ -f ./lib/flgl/user/user_CMakeLists.txt ]; then
	cp ./lib/flgl/user/user_CMakeLists.txt ./CMakeLists.txt
	cp ./lib/flgl/.gitignore/ ./
	mkdir src
	cp ./lib/flgl/user/hellowindow.cpp ./src/main.cpp
else
	echo "Run this from your project directory with flgl at ./lib/flgl to bring in a CMakeLists, gitignore, & place an flgl hello world main at src/main.cpp"
fi
