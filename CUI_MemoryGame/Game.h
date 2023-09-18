#pragma once
#include <ctime>
#include <string>
#include <windows.h>
using namespace std;

// シーン
enum class GameState { TITLE, GAME, RESULT, EXIT, CARD0, CARD1, CARD2 };

// カードの状態
struct Card {
	string card_num;
	bool is_remain;
};

// 位置
struct Position {
	int x;
	int y;
};

class Game {
public:
	// STATE
	GameState currentTransState = GameState::TITLE;
	GameState currentCardState = GameState::CARD0;
	void display_title(); // タイトル表示関数
	void start(); // ゲーム本編関数
	void display_result(); //リザルト表示関数
private:
	const int CARD_SIZE = 20;
	Card card[20]; // 20枚のカード
	Position pointer; // 手
	Position fc; // 1回目に裏返したカード
	Position sc; // 2回目に裏返したカード
	int remain_card_num; // 場に残っているカードの枚数
	time_t start_time; // startタイマー
	time_t end_time; // endタイマー
	int card_open_count = 0; // カードをめくった回数（1組で1回）
	int k; // キー入力
	void message_explanation();// 顔文字メッセージと遊び方説明表示関数
	// 遊び方説明
	string gameExplanation =
		"\n\n"
		"　　　　　　　　　　　　　 　(^O^) ひとり神経衰弱 (^O^)\n\n"
		"　　　　　　　　　　　　　　エンターキーを押すと始まるよ！\n\n"
		"　□－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－ーーーーー－□\n"
		"　 ｜　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　 |\n"
		"　 ｜【遊び方】　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　 |\n"
		"　 ｜　　　　　　　　　　　　　　　　　　　　　　 　　　　　　　　　　　　　　　　　　|\n"
		"　 ｜　ひとり神経衰弱とは？？　　　　　　　　　　　　　　　　　　　　　　　　　　　　 |\n"
		"　 ｜　　1人でひたすらカードをめくり、同じ数字のカードを見つけてペアをつくります。　  |\n"
		"　 ｜　　すべてのカードをペアにできたらゲームクリアです。　　　　　　　　　　　　　　 |\n"
		"　 ｜　　　　　　　　　　　　　　　　　　　　　　 　　　　　　　　　　　　　　　　　　|\n"
		"　 ｜　操作方法　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　 |\n"
		"　 ｜　　W(↑) A(←) S(↓) D(→)：カーソル移動 　 　　　　　　　　　　　　　　　　　　|\n"
		"　 ｜　　Enter：カードをめくる　　　　　　　　　　　　 　　　　　　　　　　　　　　　 |\n"
		"　 ｜　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　 |\n"
		"　 ｜　ポイント　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　 |\n"
		"　 ｜　　◎　できるだけ早く　　　　　　　　　　　　　　　　　　　　　　　　　　　　　 |\n"
		"　 ｜　　◎　めくる回数も少なめで♪  　　　  　　　　　　　　　　　　　　　　　 　　　|\n"
		"　 ｜　　とりあえず、はじめてみよう！　　　　　　　　   　　　  　　　　 　　　　 　　|\n"
		"　 ｜　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　 |\n"
		"　□ー－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－ーー－－ーー□\n";
	string gameExplanation_short =
		"　□－－－－－－－－－－－－－－－－－－－－－－－－ー□\n"
		"　 ｜　　　　　　　　　　　　　　　　　　　　　　　　 |\n"
		"　 ｜【操作方法】　　　　　　　　　　　　　　　　　　 |\n"
		"　 ｜　　　　　　　　　　　　　　　　　　　　　　　　 |\n"
		"　 ｜　　W(↑) A(←) S(↓) D(→)：カーソル移動 　 　　|\n"
		"　 ｜　　Enter：カードをめくる　　　　　　　　 　　　 |\n"
		"　 ｜　　　　　　　　　　　　　　　　　　　　　　　　 |\n"
		"　□ー－－－－－－－－－－－－－－－－－－－－－－ー－□\n";
	// メッセージ
	string message = "(^o^)丿これから私が応援するよ！";
	string good_message[4] = { "(◎_◎;)や、やるじゃないの！", "その調子！ヽ(^o^)丿", "(ﾟдﾟ)！なんという記憶力！！", "(/・ω・)/おみごと！" };
	string bad_message[5] = { "(´;ω;｀)ざんね～ん", "φ(..)記憶記憶！メモメモ！", "( *´艸｀)ありゃりゃ", "(∩´∀｀)∩ がんばれぇ～！", "(゜-゜)頑張ってる？" };	
	string clearMessage =
		"\n\n\n"
		"　　★－－－－－－－－－－－－－－－－－ーー★\n"
		"　　　★　　　　　　　　　　　　　　　　　★\n"
		"　　　｜　　ゲームクリア！！！　　　　　　|\n"
		" 　　  ★　 お疲れ様！よく頑張りました！ ★\n"
		" 　　 ｜　　つぎは、結果発表だよ！　　 　 |\n"
		"　　　★　　　　　　　　　　　　　　　　　★\n"
		"　　★ー－－－－－－－－－－－－－－－ーー－★\n"
		"\n";
	// スコア
	int score;
	string score_message;
};