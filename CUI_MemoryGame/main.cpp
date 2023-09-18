#include <algorithm>
#include <chrono>
#include <cmath>
#include <conio.h>
#include <ctime>
#include <iostream>
#include <io.h>
#include <math.h>
#include <random>
#include <stdio.h>
#include <string>
#include <thread>
#include <windows.h>
#define _USE_MATH_DEFINES
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

	// TITLE
	// 遊び方説明描画関数
	void display_game_explanation() {
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
		cout << gameExplanation;
		while (k != 13) {
			k = _getch();
			if (k == 13) {
				currentTransState = GameState::GAME;
			}
		};
	};

	// GAME
	void init() {
		// カードをシャッフルして初期化
		string num[] = { "１", "１", "２", "２", "３", "３", "４", "４", "５", "５", "６", "６", "７", "７", "８" , "８", "９", "９", "０", "０" };
		random_device rd; // カードをシャッフル
		mt19937 gen(rd());
		shuffle(begin(num), end(num), gen);
		for (int i = 0; i < CARD_SIZE; i++) {
			card[i].card_num = num[i];
			card[i].is_remain = true;
		}
		// 各種初期化
		pointer = { 0, 0 };
		fc = { 0, 0 };
		sc = { 0, 0 };
		remain_card_num = 20;
	}
	void countdown_before_game() {
		for (int i = 3; i >= 1; i--) {
			system("cls"); // 画面をクリア
			cout << "\n\n\n　　　ゲームまで：" << i << endl;
			this_thread::sleep_for(chrono::seconds(1));
		}
	}
	void run_loop() {
		// キー入力ごとに画面更新 & カード描画
		while ('q' != k)
		{
			system("cls"); // 画面をクリア
			cout << endl;
			// 20枚のカードの座標位置を管理
			int row = 0;
			int column = 0;
			switch (currentCardState) {
			// 1枚もカードを開いてないとき
			case GameState::CARD0:
				for (int i = 0; i < CARD_SIZE; i++) {
					if (i % 5 == 0 && i != 0) {
						column = 0;
						row += 1;
						cout << "\n\n";
					}
					if (column == pointer.x && row == pointer.y) {
						if (column == 0) {
							cout << "　";
						}
						cout << "◆　";
					}
					else {
						if (column == 0) {
							cout << "　";
						}
						if (card[i].is_remain == true) {
							cout << "□　";
						}
						else {
							cout << "　　";
						}
					}
					column += 1;
				}
				cout << "\n\n\n　" << message << "\n\n\n"; // 顔文字メッセージ
				cout << gameExplanation_short; // 遊び方説明
				break;
				// 1枚カードを開いたとき
			case GameState::CARD1:
				for (int i = 0; i < CARD_SIZE; i++) {
					if (i % 5 == 0 && i != 0) {
						column = 0;
						row += 1;
						cout << "\n\n";
					}
					if (column == fc.x && row == fc.y) {
						if (column == 0) {
							cout << "　";
						}
						cout << card[i].card_num << "　";
					}
					else if (column == pointer.x && row == pointer.y) {
						if (column == 0) {
							cout << "　";
						}
						cout << "◆　";
					}
					else {
						if (column == 0) {
							cout << "　";
						}
						if (card[i].is_remain == true) {
							cout << "□　";
						}
						else {
							cout << "　　";
						}
					}
					column += 1;
				}
				cout << "\n\n\n　" << message << "\n\n\n"; // 顔文字メッセージ
				cout << gameExplanation_short; // 遊び方説明
				break;
				// 2枚カードを開いたとき
			case GameState::CARD2:
				// 正解
				if (card[fc.y * 5 + fc.x].card_num == card[sc.y * 5 + sc.x].card_num) {
					// ランダムなメッセージを設定
					random_device rd_good;
					mt19937 gen(rd_good());
					shuffle(begin(good_message), end(good_message), gen);
					message = good_message[0];
					card[fc.y * 5 + fc.x].is_remain = false;
					card[sc.y * 5 + sc.x].is_remain = false;
					remain_card_num -= 2; // 場にあるカードの枚数を管理
					for (int i = 0; i < CARD_SIZE; i++) {
						if (i % 5 == 0 && i != 0) {
							column = 0;
							row += 1;
							cout << "\n\n";
						}
						if (column == fc.x && row == fc.y) {
							if (column == 0) {
								cout << "　";
							}
							cout << "　　";
						}
						else if (column == sc.x && row == sc.y) {
							if (column == 0) {
								cout << "　";
							}
							cout << "　　";
						}
						else {
							if (column == 0) {
								cout << "　";
							}
							if (card[i].is_remain == true) {
								cout << "□　";
							}
							else {
								cout << "　　";
							}
						}
						column += 1;
					}
					cout << "\n\n\n　" << message << "\n\n\n"; // 顔文字メッセージ
					cout << gameExplanation_short; // 遊び方説明
					fc = { 0, 0 };
					sc = { 0, 0 };
					currentCardState = GameState::CARD0;
					continue;
				}
				// 不正解
				else {
					// ランダムなメッセージを設定
					random_device rd_bad;
					mt19937 gen(rd_bad());
					shuffle(begin(bad_message), end(bad_message), gen);
					message = bad_message[0];
					for (int i = 0; i < CARD_SIZE; i++) {
						if (i % 5 == 0 && i != 0) {
							column = 0;
							row += 1;
							cout << "\n\n";
						}
						if (column == fc.x && row == fc.y) {
							if (column == 0) {
								cout << "　";
							}
							cout << card[i].card_num << "　";
						}
						else if (column == sc.x && row == sc.y) {
							if (column == 0) {
								cout << "　";
							}
							cout << card[i].card_num << "　";
						}
						else {
							if (column == 0) {
								cout << "　";
							}
							if (card[i].is_remain == true) {
								cout << "□　";
							}
							else {
								cout << "　　";
							}
						}
						column += 1;
					}
					cout << "\n\n\n　" << message << "\n\n\n"; // 顔文字メッセージ
					cout << gameExplanation_short; // 遊び方説明

					this_thread::sleep_for(chrono::seconds(1)); // 1秒後CARD0に遷移
					fc = { 0, 0 };
					sc = { 0, 0 };
					currentCardState = GameState::CARD0;
					continue;
				}
				break;
			}
			if (remain_card_num == 0) {
				end_time = time(nullptr); // ゲームクリアと同時にタイマーを止める
				system("cls"); // 画面をクリア
				cout << clearMessage; // クリアメッセージ
				currentTransState = GameState::RESULT;
				this_thread::sleep_for(chrono::seconds(4)); // 4秒後RESULTに遷移
				break;
			}
			// キー入力待ち
			k = _getch();
			// キー入力で座標更新orステート変更
			switch (k) {
			case 's':
				if (pointer.y < 3) {
					++pointer.y;
				}
				break;
			case 'a':
				if (pointer.x > 0) {
					--pointer.x;
				}
				break;
			case 'd':
				if (pointer.x < 4) {
					++pointer.x;
				}
				break;
			case 'w':
				if (pointer.y > 0) {
					--pointer.y;
				}
				break;
			case 13:
				switch (currentCardState) {
				case GameState::CARD0:
					if (card[pointer.y * 5 + pointer.x].is_remain == true) {
						fc.x = pointer.x;
						fc.y = pointer.y;
						currentCardState = GameState::CARD1;
					}
					break;

				case GameState::CARD1:
					if (card[pointer.y * 5 + pointer.x].is_remain == true) {
						if (pointer.x != fc.x || pointer.y != fc.y) {
							card_open_count += 1;
							sc.x = pointer.x;
							sc.y = pointer.y;
							currentCardState = GameState::CARD2;
						}
					}
					break;
				}
				break;
			}
		}
	}
	
	// RESULT
	time_t start_time; // startタイマー
	time_t end_time; // endタイマー
	int card_open_count = 0; // カードをめくった回数（1組で1回）
	// スコア計算関数
	void calculate_score() {
		score = static_cast<int>((1.0 / difftime(end_time, start_time) + 1.0 / card_open_count) * 1000);
	}
	// スコアに対するメッセージ設定関数
	void set_score_message() {
		if (score < 40) {
			score_message = "Dランク！ あたならもっと頑張れるはず！(゜-゜)";
		}
		else if (score < 55) {
			score_message = "Cランク！ 惜しい！どうせならBランクめざそう！？(^O^)";
		}
		else if (score < 70) {
			score_message = "Bランク！ どうやらそこそこ真剣にやってくれたみたい！？(゜-゜)ありがとう！";
		}
		else if (score < 85) {
			score_message = "Aランク！ なかなかすごい！！もしかして、神経衰弱めっちゃ得意！？(*''▽'')";
		}
		else {
			score_message = "Sランク！ なかなかこの域には達しないです。素晴らしい！おめでとうございます。( ^^) _旦~~";
		}
	}
	// 結果出力関数
	void display_result() {
		calculate_score();
		set_score_message();
		system("cls"); // 画面をクリア
		cout << "\n　結果発表\n\n";
		cout << "　スコア: " << score << "\n\n";
		cout << "　開発者からあなたへコメント: " << score_message << "\n\n";
		cout << "　クリアにかかった時間: " << difftime(end_time, start_time) << " 秒\n\n";
		cout << "　カードを開いた回数: " << card_open_count << " 回\n\n";
		cout << "　遊んでくれてありがとうございました！\n\n\n";
		cout << "　※ エンターキーを押すとゲームが終了します。" << endl;
		// キー入力待ち
		k = _getch();
		if (k == 13) {
			currentTransState = GameState::EXIT;
		};
	}
private:
	// GAME
	const int CARD_SIZE = 20;
	Card card[20]; // 20枚のカード
	Position pointer; // 手
	Position fc; // 1回目に裏返したカード
	Position sc; // 2回目に裏返したカード
	int remain_card_num; // 場に残っているカードの枚数
	int k; // キー入力
	// 応援メッセージ
	string message = "(^o^)丿これから私が応援するよ！";
	// 不正解
	string bad_message[5] = { "(´;ω;｀)ざんね～ん", "φ(..)記憶記憶！メモメモ！", "( *´艸｀)ありゃりゃ", "(∩´∀｀)∩ がんばれぇ～！", "(゜-゜)頑張ってる？" };
	// 正解
	string good_message[4] = { "(◎_◎;)や、やるじゃないの！", "その調子！ヽ(^o^)丿", "(ﾟдﾟ)！なんという記憶力！！", "(/・ω・)/おみごと！" };
	// ショート版遊び方説明
	string gameExplanation_short =
		"　□－－－－－－－－－－－－－－－－－－－－－－－－ー□\n"
		"　 ｜　　　　　　　　　　　　　　　　　　　　　　　　 |\n"
		"　 ｜【操作方法】　　　　　　　　　　　　　　　　　　 |\n"
		"　 ｜　　　　　　　　　　　　　　　　　　　　　　　　 |\n"
		"　 ｜　　W(↑) A(←) S(↓) D(→)：カーソル移動 　 　　|\n"
		"　 ｜　　Enter：カードをめくる　　　　　　　　 　　　 |\n"
		"　 ｜　　　　　　　　　　　　　　　　　　　　　　　　 |\n"
		"　□ー－－－－－－－－－－－－－－－－－－－－－－ー－□\n";
	// クリア後のメッセージ
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
	// RESULT
	int score;
	string score_message;
};


int main() {
	// エスケープシーケンスを有効に
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(hStdOut, &mode);
	SetConsoleMode(hStdOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	// カーソルを消す
	printf("\x1b[?25l");

	Game game;
	while (game.currentTransState != GameState::RESULT) {
		system("cls"); // 画面をクリア
		switch (game.currentTransState) {
		case GameState::TITLE:
			game.display_game_explanation();
		case GameState::GAME:
			game.init(); // 初期化
			game.countdown_before_game(); // カウントダウン
			game.start_time = time(nullptr); // ゲーム開始と共にタイマーセット
			game.run_loop();
		}
	}
	while (game.currentTransState == GameState::RESULT) {
		game.display_result();
	}
	// カーソルを表示
	printf("\x1b[?25h");
	return EXIT_SUCCESS;
}