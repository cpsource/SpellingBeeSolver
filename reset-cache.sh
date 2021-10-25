#!/bin/bash
git config credential.helper store
# timeout cache in 1 day
git config credential.helper 'cache --timeout 86400'
