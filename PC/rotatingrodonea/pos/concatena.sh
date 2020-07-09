#!/bin/bash
cat $(ls -tr *.bin) > './outputfile.out'
./uffman2 -c './outputfile.out' './outputfile.Z'
