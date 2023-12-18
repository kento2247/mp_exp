# How to build

~~1. `cd ./soft`~~  
~~2. `make clean`~~  
~~3. `make`~~  
~~4. `cd ../hard`~~  
~~5. `make clean`~~  
~~6. `make`~~  
~~7. `make program`~~  
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

# External design

- プレイ人数: 2 人
- ライフ: 5
- プレイヤーは球を撃ち返す
- 撃ち返すラケット位置は固定
- ボールが適切な位置に移動してきた時に、ボタンを押すと球を撃ち返す（タイミングゲーム）
- 打ち返せないとライフが-1
- タイミングが良いと球速が速くなる
- タイミングがいい時と悪い時でブザー音が変わる
- ラケットを振ったら（ボタンを押したら）一定時間降ることができない
- ゲーム開始時と終了時にテロップを表示
- 画面上にはライフと経過時間を表示

# Internal design

- 必須条件 1: 割り込みハンドラの処理と main の処理を分ける
- 必須条件 2: 大域変数を使用した値共有の仕様

1.
