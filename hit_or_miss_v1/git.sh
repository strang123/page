#!/bin/bash

git add .
echo "enter message"
read message
git commit -m $message
git push origin master
