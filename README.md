# How to build

~

1. `cd ./soft`
2. `make clean`
3. `make`
4. `cd ../hard`
5. `make clean`
6. `make`
7. `make program`
   ~
   `./write.sh`で書き込めます
   > 書き込み完了

# How to edit

- ハードウェアに書き込むファイル: `./mips/soft/test.c`
- 編集するときのファイル: `./main.c`
- 編集内容を書き込みファイルに反映: 以下のコマンドを実行  
  `./update.sh`

# Concept

- 直線上に球を打ち返すゲーム
- 打ち返すタイミングによって球速が変化
- 判定するロジックの周波数を高くする必要がある
- 球を打つたびに効果音

# Hardware

- main board
- large LCD display
- buzzer
- cable 1
- cable 2
- extension switch 1
- extension switch 2

# ToDo

- [ ] 音
- [ ] I/O port 管理（top.v を編集）
