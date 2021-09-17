#!/bin/bash
./tri | sort | uniq | awk -v r="," -v q='"' '{print q $0 q r}' > tri_list.h

