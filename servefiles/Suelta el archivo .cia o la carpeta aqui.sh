#!/bin/bash

read -p "Introduce la IP de tu Nintendo 3DS: " -e input
python servefiles.py $input .
