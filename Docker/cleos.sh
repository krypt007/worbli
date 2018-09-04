#!/bin/bash

# Usage:
# Go into cmd loop: sudo ./cleos.sh
# Run single cmd:  sudo ./cleos.sh <cleos paramers>

PREFIX="docker-compose exec nodeosd worbli"
if [ -z $1 ] ; then
  while :
  do
    read -e -p "cleos " cmd
    history -s "$cmd"
    $PREFIX $cmd
  done
else
  $PREFIX "$@"
fi
