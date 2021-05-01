#include "DigiKeyboard.h"

/* 上記で定義されていないキーを定義 */
#define KEY_ESC     0x29 /* ESCAPE */
#define KEY_RESERVE 0x00 /* 未定義キー(Shiftのみ入力用) */

#define TRADE_COUNT 30   /* 交換ループ回数(1スタック=交換上限33個*30回+9個*1回) */
#define KEY_COUNT   16   /* 入力キー数 */

// ◆動作条件
// ・エクスキューブ交換ショップ店員に話しかけられる位置にいること
// ・アイテム画面はグラインダーのページを開いた状態で閉じてあること
// ・グラインダーより上に表示されるアイテムを所持していないこと(カジノコインパスなど)
// ・基本倉庫に十分な空きがあること
// ・Scroll Lockがオフになっていること(Enter押下後にチャットが打てる状態)
// ・キーバインドが初期設定のものと同等であること(話しかける時など)

// ◆使い方
// ・TRADE_COUNTを設定(キューブ1スタックなら30回)
// ・digisparkにこのスケッチを書き込む(終了後キー入力が実行されるので注意)
// ・動作条件を満たした状態でUSB端子にdigisparkを差し込む

// ◆参考
// https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf (P53以降)

/* 一度だけ動作させるためのフラグ */
uint8_t one_flag = 0;

/* 操作テーブル①(キー) */
const byte KEYS[KEY_COUNT] = {
  KEY_E,           /* 話かける */
  KEY_ENTER,       /* エクスキューブ交換ショップを選択 */
  KEY_ARROW_LEFT,  /* グラインダー33個選択 */
  KEY_ENTER,
  KEY_ENTER,       /* 交換確認：はい */
  KEY_ENTER,       /* 交換に成功しました：はい */
  KEY_ESC,         /* エクスキューブ交換ショップ閉じる */
  KEY_ESC,
  KEY_ESC,         /* メニュー開く */
  KEY_ENTER,       /* アイテム選択 */
  KEY_RESERVE,     /* グラインダー全選択 */
  KEY_ENTER,
  KEY_ENTER,       /* 倉庫にあずける */
  KEY_ENTER,       /* 基本倉庫を選択 */
  KEY_ESC,         /* メニューを閉じる */
  KEY_ESC };

/* 操作テーブル②(ディレイ時間) */
const uint16_t DELAYS[KEY_COUNT] = {
  800, /* 話しかけた際の画面遷移時間を考慮 */
  250,
  250,
  250,
  250,
  250,
  250,
  250,
  250,
  250,
  250,
  250,
  250,
  250,
  250,
  250 };

void setup()
{
  /* 何もしない */
}

void loop()
{
  DigiKeyboard.delay( 1000 );

  /* 既に動作済み? */
  if( 0 == one_flag )
  {
    /* TRADE_COUNTの設定回数ループ処理 */
    for( uint8_t loop_cnt = 0; loop_cnt < TRADE_COUNT; loop_cnt++ )
    {
      /* 操作テーブルのループ処理 */
      for( uint8_t loop_key = 0; loop_key < KEY_COUNT; loop_key++ )
      {
        /* 未定義キー? */
        if( KEY_RESERVE == KEYS[loop_key] )
        {
          /* Shiftのみ入力 */
          DigiKeyboard.sendKeyStroke( KEY_RESERVE, MOD_SHIFT_LEFT );
        }
        else
        {
          /* それ以外の入力 */
          DigiKeyboard.sendKeyStroke( KEYS[loop_key] );
        }

        /* ディレイ */
        DigiKeyboard.delay( DELAYS[loop_key] );
      }
    }
    one_flag++;
  }

  DigiKeyboard.delay( 5000 );
}
