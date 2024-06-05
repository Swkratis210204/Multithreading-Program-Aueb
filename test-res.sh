#!/bin/bash

# Μεταγλώττιση του προγράμματος
gcc -pthread -o p3220028-p3220031 p3220028-p3220031.c -lm

# Έλεγχος αν η μεταγλώττιση ήταν επιτυχής
if [ $? -eq 0 ]; then
    echo -e "The compile was successful.Starting to execute the program.\n\n"
    
    # Εκτέλεση του προγράμματος με παραμέτρους 100 πελάτες και αρχικό σπόρο 10
    ./p3220028-p3220031 100 10
else
    echo -e "The compile failed.\n\n"
fi
