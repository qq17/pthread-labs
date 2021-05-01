#!/bin/bash

g++ -c lab8_1.cpp
g++ -o lab8_1 lab8_1.o -lpthread -lrt

g++ -c lab8_2.cpp
g++ -o lab8_2 lab8_2.o -lpthread -lrt

sudo setcap cap_sys_resource=eip lab8_1
sudo setcap cap_sys_resource=eip lab8_2