touch ./mips/soft/test.c

if [ "$#" -ne 1 ]; then
    echo "No filename specified. Using default: main.c"
    filename="main.c"
else
    filename="$1"
fi

# ファイルのコピー
cat ./"$filename" > ./mips/soft/test.c

cd ./mips/soft
make clean
make
cd ../hard
make clean
make