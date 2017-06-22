#!/usr/bin/env python
import fileinput

#train = ( 3,)
for line in fileinput.input():
    nick, sep, rest = line.partition(" 0 ")
    if rest:
       print(rest.strip())
