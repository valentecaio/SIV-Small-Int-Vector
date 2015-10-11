#!/bin/bash
 
if [ $# -lt 1 ]; then
   echo "USAGE: $0 <script_name>"
   exit 1
fi

gcc -Wall -m32 -g -o $1 $1.c $2.c || exit 1

echo "script compilado!"
sleep 1

echo "EXECUÇÃO:"
echo " "

./$1
