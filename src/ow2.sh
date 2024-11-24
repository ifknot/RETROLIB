#!/bin/bash

clear

PS3="Select Target: "

select opt in DOS-16bit DOS-32bit WIN-16bit WIN-32bit OS/2-16bit OS/2-32bit quit; do

  case $opt in
    DOS-16bit)
      source ./exportOW2-DOS-16bit.sh
      ;;
    DOS-32bit)
      source ./exportOW2-DOS-32bit.sh
      ;;
    WIN-16bit)
      source ./exportOW2-WIN-16bit.sh
      ;;
    WIN-32bit)
      source ./exportOW2-WIN-32bit.sh
      ;;
    OS/2-16bit)
      source ./exportOW2-OS2-16bit.sh
      ;;
    OS/2-32bit)
      source ./exportOW2-OS2-32bit.sh
      ;;
    quit)
      break
      ;;
    *) 
      echo "Invalid option $REPLY"
      ;;
  esac
done