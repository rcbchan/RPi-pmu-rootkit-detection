#!/bin/bash

gcc hpc.c -o hpc
sudo setcap cap_sys_admin+ep hpc
./hpc