#!/usr/bin/env bash

NAME="fs"

cd /etc/paths.d && touch $NAME && echo '/usr/local/$NAME/bin'

cd /usr/local && mkdir $NAME && cd $NAME && mkdir bin && cd bin && cd "${0%/*}"

cp fs /usr/local/$NAME/bin 

#touch test.txt && echo 'adad' >> test.txt || sudo cp test.txt ./test
