#!/bin/zsh

echo "* Open Watcom 2 setup PATH and export environment variables for DOS-16bit *"

export WATCOM=/opt/watcom		#to make life easier...
export PATH=$PATH:$WATCOM/armo64 	#add to local path
export EDDAT=$WATCOM/eddat 		# editor information path
export INCLUDE=$WATCOM/h 		# setup DOS 8088/8086 development
export LIB=$WATCOM/lib286 		# OW2 16-bit compilers OS-independent libraries

echo $WATCOM
echo $PATH
echo $EDDAT
echo $INCLUDE
echo $LIB 
