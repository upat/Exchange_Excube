#include "DigiKeyboard.h"

/* 上記で定義されていないキーを定義 */
#define KEY_ESC         0x29 /* ESCAPE */
#define KEY_ARROW_RIGHT 0x4F /* 右矢印キー */

#define TRADE_COUNT     1    /* 交換ループ回数 */

// ◆動作条件
// ・エクスキューブ交換ショップ店員に話しかけられる位置にいること
// ・アイテム画面はグラインダーのページを開いた状態で閉じてあること
// ・グラインダーより上に表示されるアイテムを所持していないこと(カジノコインパスなど)
// ・基本倉庫に十分な空きがあること
// ・Scroll Lockがオフになっていること(Enter押下後にチャットが打てる状態)
// ・キーバインドが初期設定のものと同等であること(話しかける時など)

// ◆使い方
// ・TRADE_COUNTを設定(キューブ1スタックなら30回程度？)
// ・digisparkにこのスケッチを書き込む(終了後キー入力が実行されるので注意)
// ・動作条件を満たした状態でUSB端子にdigisparkを差し込む

// ◆参考
// https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf (P53以降)

/* 一度だけ動作させるためのフラグ */
uint8_t one_flag = 0;

void setup()
{
  /* 何もしない */
}

void loop()
{
  DigiKeyboard.delay( 1000 );
  
  if( 0 == one_flag )
  {
    for( uint8_t loop_cnt = TRADE_COUNT; loop_cnt > 0; loop_cnt-- )
    {
      /* 話かける */
      sendKeyStroke_sub( KEY_E );
      /* エクスキューブ交換ショップを選択 */
      sendKeyStroke_sub( KEY_ENTER );
      /* グラインダー33個選択 */
      sendKeyStroke_sub( KEY_ARROW_LEFT );
      sendKeyStroke_sub( KEY_ENTER );
      /* 交換確認：はい */
      sendKeyStroke_sub( KEY_ENTER );
      /* 交換に成功しました：はい */
      sendKeyStroke_sub( KEY_ENTER );
      /* エクスキューブ交換ショップ閉じる */
      sendKeyStroke_sub( KEY_ESC );
      sendKeyStroke_sub( KEY_ESC );
      /* メニュー開く */
      sendKeyStroke_sub( KEY_ESC );
      /* アイテム選択 */
      sendKeyStroke_sub( KEY_ENTER );
      /* グラインダー選択 */
      sendKeyStroke_sub( KEY_ENTER );
      /* 倉庫にあずける */
      sendKeyStroke_sub( KEY_ENTER );
      /* 基本倉庫を選択 */
      sendKeyStroke_sub( KEY_ENTER );
      /* 個数を選択(990個) */
      sendKeyStroke_sub( KEY_ARROW_RIGHT );
      sendKeyStroke_sub( KEY_ENTER );
      sendKeyStroke_sub( KEY_ENTER );
      /* メニューを閉じる */
      sendKeyStroke_sub( KEY_ESC );
      sendKeyStroke_sub( KEY_ESC );
    }
    one_flag++;
  }

  DigiKeyboard.delay( 5000 );
}

void sendKeyStroke_sub( uint8_t key )
{
  DigiKeyboard.sendKeyStroke( key );
  DigiKeyboard.delay( 750 ); /* これ以上早くすると事故る(750ms) */
}
