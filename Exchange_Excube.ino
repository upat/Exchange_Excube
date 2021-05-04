#include "DigiKeyboard.h"

/* 上記で定義されていないキーを定義 */
#define KEY_ESC         0x29 /* ESCAPE */
#define KEY_ARROW_RIGHT 0x4F /* 右矢印キー */
#define KEY_RESERVE     0x00 /* 未定義キー(Shiftのみ入力用) */

#define TRADE_COUNT     30   /* 交換ループ回数(1スタック=交換上限33個*30回+9個*1回) */
#define KEY_COUNT       16   /* 入力キー数 */
#define KEY_LAST_COUNT  24   /* 入力キー数(最後) */

#define DELAY_LONG      900  /* ディレイ時間 900ms */
#define DELAY_SHORT     250  /* ディレイ時間 250ms(250ms未満は稀に抜けるため) */

#define DEBUG_FLAG      1

// ◆動作条件
// ・過密ブロックで行わないこと(読込時間が長くなり、想定外のディレイが必要になるため)
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

/* 操作テーブル①(33個単位の交換) */
const byte KEYS[KEY_COUNT] = {
  KEY_E,           /* 話かける(画面遷移時間を考慮すること) */
  KEY_ENTER,       /* エクスキューブ交換ショップを選択(読込時間を考慮すること) */
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

/* 操作テーブル②(最後の9個分の交換) */
const uint16_t KEYS_LAST[KEY_LAST_COUNT] = {
  KEY_E,           /* 話かける(画面遷移時間を考慮すること) */
  KEY_ENTER,       /* エクスキューブ交換ショップを選択(読込時間を考慮すること) */
  KEY_ARROW_RIGHT, /* グラインダー選択数+1 */
  KEY_ARROW_RIGHT, /* グラインダー選択数+1 */
  KEY_ARROW_RIGHT, /* グラインダー選択数+1 */
  KEY_ARROW_RIGHT, /* グラインダー選択数+1 */
  KEY_ARROW_RIGHT, /* グラインダー選択数+1 */
  KEY_ARROW_RIGHT, /* グラインダー選択数+1 */
  KEY_ARROW_RIGHT, /* グラインダー選択数+1 */
  KEY_ARROW_RIGHT, /* グラインダー選択数+1 */
  KEY_ARROW_RIGHT, /* グラインダー選択数+1 */
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

void setup()
{
  /* 何もしない */
}

void loop()
{
  /* 既に動作済み? */
  if( 0 == one_flag )
  {
    /* TRADE_COUNTの設定回数ループ処理 */
    for( uint8_t loop_cnt = 0; loop_cnt < TRADE_COUNT; loop_cnt++ )
    {
      /* 操作テーブルのループ処理(33個単位の交換) */
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

        /* 初回ディレイ？ */
        if( 2 > loop_key )
        {
          /* ディレイ 900ms */
          DigiKeyboard.delay( DELAY_LONG );
        }
        else
        {
          /* ディレイ 250ms */
          DigiKeyboard.delay( DELAY_SHORT );
        }
      }
    }
#if DEBUG_FLAG
    /* 操作テーブルのループ処理(最後の9個分の交換) */
    for( uint8_t loop_key = 0; loop_key < KEY_LAST_COUNT; loop_key++ )
    {
      /* 未定義キー? */
      if( KEY_RESERVE == KEYS_LAST[loop_key] )
      {
        /* Shiftのみ入力 */
        DigiKeyboard.sendKeyStroke( KEY_RESERVE, MOD_SHIFT_LEFT );
      }
      else
      {
        /* それ以外の入力 */
        DigiKeyboard.sendKeyStroke( KEYS_LAST[loop_key] );
      }

      /* 初回ディレイ？ */
      if( 2 > loop_key )
      {
        /* ディレイ 900ms */
        DigiKeyboard.delay( DELAY_LONG );
      }
      else
      {
        /* ディレイ 250ms */
        DigiKeyboard.delay( DELAY_SHORT );
      }
    }
#endif
    one_flag++;
  }

  DigiKeyboard.delay( 5000 );
}
