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

//タイトル、ゲーム本編、リザルト、開いたカードの枚数によりステートを変更
enum class GameState { TITLE, GAME, RESULT, EXIT, CARD0, CARD1, CARD2 };

struct Card {
	string card_num;
	bool is_remain;
};

struct Position {
	int x;
	int y;
};

int main() {

	// エスケープシーケンスを有効に
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(hStdOut, &mode);
	SetConsoleMode(hStdOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	// カーソルを消す
	printf("\x1b[?25l");

	// キー入力
	int k;

	// タイマー
	time_t start_time;
	time_t end_time;

	// カードをめくった回数（1組で1回）
	int card_open_count = 0;

	// カードの数字をランダム化
	string num[] = { "１", "１", "２", "２", "３", "３", "４", "４", "５", "５", "６", "６", "７", "７", "８" , "８", "９", "９", "０", "０" };
	// 乱数生成器を使用してシャッフル
	random_device rd;
	mt19937 gen(rd());
	shuffle(begin(num), end(num), gen);

	// カードを初期化
	const int CARD_SIZE = 20;
	Card card[CARD_SIZE];
	for (int i = 0; i < CARD_SIZE; i++) {
		card[i].card_num = num[i];
		card[i].is_remain = true;
	}

	//ポイント部分、最初に裏返したカード、次に裏返したカードの座標位置
	Position pointer = { 0, 0 };
	Position fc = { 0, 0 };
	Position sc = { 0, 0 };
	// 場に残っているカードの枚数
	int remain_card_num = 20;

	// 応援メッセージ
	string message = "(^o^)丿これから私が応援するよ！";
	// 数が合わなかったとき
	string bad_message[] = { "(´;ω;｀)ざんね～ん", "φ(..)記憶記憶！メモメモ！", "( *´艸｀)ありゃりゃ", "(∩´∀｀)∩ がんばれぇ～！", "(゜-゜)頑張ってる？" };
	// 数が合ったとき
	string good_message[] = { "(◎_◎;)や、やるじゃないの！", "その調子！ヽ(^o^)丿", "(ﾟдﾟ)！なんという記憶力！！", "(/・ω・)/おみごと！" };

	// 遊び方説明
	string gameExplanation =
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
	string clearMessage =
		"　　★－－－－－－－－－－－－－－－－－ーー★\n"
		"　　　★　　　　　　　　　　　　　　　　　★\n"
		"　　　｜　　ゲームクリア！！！　　　　　　|\n"
		" 　　  ★　 お疲れ様！よく頑張りました！ ★\n"
		" 　　 ｜　　つぎは、結果発表だよ！　　 　 |\n"
		"　　　★　　　　　　　　　　　　　　　　　★\n"
		"　　★ー－－－－－－－－－－－－－－－ーー－★\n";

	// 初期画面遷移ステート
	GameState currentTransState = GameState::TITLE;
	// 初期カードステート
	GameState currentCardState = GameState::CARD0;

	while (currentTransState != GameState::RESULT) {
		// 画面をクリア
		system("cls");
		switch (currentTransState) {
		case GameState::TITLE:

			cout << endl << endl;
			cout << "　　　　　　　　　　　　　 　(^O^) ひとり神経衰弱 (^O^)" << endl << endl;
			cout << "　　　　　　　　　　　　　　エンターキーを押すと始まるよ！" << endl << endl;
			cout << gameExplanation;

			do {
				k = _getch();
				if (k == 13) {
					currentTransState = GameState::GAME;
				}
			} while (k != 13);
			break;
		case GameState::GAME:

			// カウントダウン
			for (int i = 3; i >= 1; i--) {
				// 画面をクリア
				system("cls");
				cout << endl << endl << endl << "　　　ゲームまで：" << i << endl;
			std:this_thread::sleep_for(std::chrono::seconds(1));
			}

			// ゲーム開始と同時にタイマーセット
			start_time = time(nullptr);

			//キー入力ごとに画面更新、カードの描画
			do
			{
				// 画面をクリア
				system("cls");
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
							cout << endl;
							cout << endl;
						}
						if (column == pointer.x && row == pointer.y) {
							if (column == 0) {
								cout << "　";
							}
							cout << "◆" << "　";
						}
						else {
							if (column == 0) {
								cout << "　";
							}
							if (card[i].is_remain == true) {
								cout << "□" << "　";
							}
							else {
								cout << "　" << "　";
							}
						}
						column += 1;
					}

					// メッセージ
					cout << endl << endl << endl;
					cout << "　" << message << endl << endl << endl;
					// 遊び方説明
					cout << gameExplanation_short;

					break;

					// 1枚カードを開いたとき
				case GameState::CARD1:
					for (int i = 0; i < CARD_SIZE; i++) {
						if (i % 5 == 0 && i != 0) {
							column = 0;
							row += 1;
							cout << endl;
							cout << endl;
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
							cout << "◆" << "　";
						}
						else {
							if (column == 0) {
								cout << "　";
							}
							if (card[i].is_remain == true) {
								cout << "□" << "　";
							}
							else {
								cout << "　" << "　";
							}
						}
						column += 1;
					}

					// メッセージ
					cout << endl << endl << endl;
					cout << "　" << message << endl << endl << endl;
					// 遊び方説明
					cout << gameExplanation_short;

					break;

					// 2枚カードを開いたとき
				case GameState::CARD2:
					// 2枚のカードの数字が合った場合
					if (card[fc.y * 5 + fc.x].card_num == card[sc.y * 5 + sc.x].card_num) {

						// ランダムなメッセージを設定
						// 乱数生成器を使用してシャッフル
						random_device rd_good;
						mt19937 gen(rd_good());
						shuffle(begin(good_message), end(good_message), gen);
						message = good_message[0];

						card[fc.y * 5 + fc.x].is_remain = false;
						card[sc.y * 5 + sc.x].is_remain = false;
						// 場にあるカードの枚数を管理
						remain_card_num -= 2;
						for (int i = 0; i < CARD_SIZE; i++) {
							if (i % 5 == 0 && i != 0) {
								column = 0;
								row += 1;
								cout << endl;
								cout << endl;
							}
							if (column == fc.x && row == fc.y) {
								if (column == 0) {
									cout << "　";
								}
								cout << "　" << "　";
							}
							else if (column == sc.x && row == sc.y) {
								if (column == 0) {
									cout << "　";
								}
								cout << "　" << "　";
							}
							else {
								if (column == 0) {
									cout << "　";
								}
								if (card[i].is_remain == true) {
									cout << "□" << "　";
								}
								else {
									cout << "　" << "　";
								}
							}
							column += 1;
						}

						// メッセージ
						cout << endl << endl << endl;
						cout << "　" << message << endl << endl << endl;
						// 遊び方説明
						cout << gameExplanation_short;

						// 一定時間後にCARD0に遷移する
						// std::this_thread::sleep_for(std::chrono::seconds(1)); // 1秒遅延
						fc = { 0, 0 };
						sc = { 0, 0 };
						currentCardState = GameState::CARD0;
						continue;
					}
					// 2枚のカードの数字が合わなかった場合
					else {

						// ランダムなメッセージを設定
						// 乱数生成器を使用してシャッフル
						random_device rd_bad;
						mt19937 gen(rd_bad());
						shuffle(begin(bad_message), end(bad_message), gen);
						message = bad_message[0];

						for (int i = 0; i < CARD_SIZE; i++) {
							if (i % 5 == 0 && i != 0) {
								column = 0;
								row += 1;
								cout << endl;
								cout << endl;
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
									cout << "□" << "　";
								}
								else {
									cout << "　" << "　";
								}
							}
							column += 1;
						}

						// メッセージ
						cout << endl << endl << endl;
						cout << "　" << message << endl << endl << endl;
						// 遊び方説明
						cout << gameExplanation_short;

						// 一定時間後にCARD0に遷移する
						std::this_thread::sleep_for(std::chrono::seconds(1)); // 1秒遅延
						fc = { 0, 0 };
						sc = { 0, 0 };
						currentCardState = GameState::CARD0;
						continue;
					}
					break;
				}

				if (remain_card_num == 0) {
					// ゲームクリアと同時にタイマーを止める
					end_time = time(nullptr);

					// 画面をクリア
					system("cls");
					// メッセージ
					cout << endl << endl << endl;
					cout << clearMessage << endl;
					currentTransState = GameState::RESULT;
					// 一定時間後にRESULTに遷移する
					std::this_thread::sleep_for(std::chrono::seconds(4)); // 4秒遅延
					break;
				}

				// バグチェック
				//cout << endl;
				//cout << fc.x << " " << fc.y << " " << sc.x << " " << sc.y << endl;
				//cout << pointer.x << " " << pointer.y << endl;

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
			} while ('q' != k); //  'q'キーで終了
			break;
		}
	}

	do {

		// スコア計算
		int score = static_cast<int>((1.0 / difftime(end_time, start_time) + 1.0 / card_open_count) * 1000);
		string score_message;
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

		// 画面をクリア
		system("cls");
		// 結果発表
		cout << endl;
		cout << "　結果発表" << endl << endl;
		cout << "　スコア: " << score << endl << endl;
		cout << "　開発者からあなたへコメント: " << score_message << endl << endl;
		cout << "　クリアにかかった時間: " << difftime(end_time, start_time) << " 秒" << endl << endl;
		cout << "　カードを開いた回数: " << card_open_count << " 回" << endl << endl;
		cout << "　遊んでくれてありがとうございました！" << endl << endl << endl;
		cout << "　※ エンターキーを押すとゲームが終了します。" << endl;

		// キー入力を待つ
		k = _getch();
		if (k == 13) {
			currentTransState = GameState::EXIT;
			break;
		};

	} while (currentTransState == GameState::RESULT);

	// カーソルを表示
	printf("\x1b[?25h");
	return EXIT_SUCCESS;
}