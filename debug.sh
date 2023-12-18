touch ./mips/soft/test.c
cat ./debug.c > ./mips/soft/test.c
cd ./mips/soft
make clean
make
cd ../hard
make clean
make
make program