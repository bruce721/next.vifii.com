#! /bin/bash -e

# Note: this probably requires Check 0.9.1 to work

rm -f ../libmb/.libs/*.da *.da
CF_FORK=no make check || true

for i in *.da; do
    gcov `basename $i .da`.c | grep executed
done

cd ../libmb
for i in *.c; do
    gcov -o .libs $i | grep -v /usr/include | grep executed
done