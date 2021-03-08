#!/bin/sh
set -e
sh headers.sh

for PROJECT in $PROJECTS; do
  (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install)
done
