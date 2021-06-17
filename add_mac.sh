#!/usr/bin/env bash

P=$(pwd)
NAME="ttestt"

cd /etc/paths.d && touch $NAME && echo '/usr/local/$NAME/bin' >> $NAME

cd /usr/local && mkdir $NAME && cd $NAME && mkdir bin

cd $P && cp ttestt /usr/local/$NAME/bin 

#touch test.txt && echo 'adad' >> test.txt || sudo cp test.txt ./test
