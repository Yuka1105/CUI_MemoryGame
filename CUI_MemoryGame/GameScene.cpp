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
#include "Game.h"
#define _USE_MATH_DEFINES
using namespace std;

void Game::display_title() {
	// エスケープシーケンスを有効に
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(hStdOut, &mode);
	SetConsoleMode(hStdOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	// カーソルを消す
	printf("\x1b[?25l");
	cout << gameExplanation;
	while (k != 13) {
		k = _getch();
		if (k == 13) {
			currentTransState = GameState::GAME;
		}
	}
}

void Game::message_explanation() {
	cout << "\n\n\n　" << message << "\n\n\n"; // 顔文字メッセージ
	cout << gameExplanation_short; // 遊び方説明
}

void Game::start() {
	// カードをシャッフルして初期化
	string num[] = { "１", "１", "２", "２", "３", "３", "４", "４", "５", "５", "６", "６", "７", "７", "８" , "８", "９", "９", "０", "０" };
	random_device rd;
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
	// ゲーム開始までのカウントダウン
	for (int i = 3; i >= 1; i--) {
		system("cls"); // 画面をクリア
		cout << "\n\n\n　　　ゲームまで：" << i << endl;
		this_thread::sleep_for(chrono::seconds(1));
	}
	// ゲーム開始と共にタイマーセット
	start_time = time(nullptr);
	// ゲーム開始 キー入力ごとに画面更新 & カード描画
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
			message_explanation();
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
			message_explanation();
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
				message_explanation();
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
				message_explanation();
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

void Game::display_result() {
	score = static_cast<int>((1.0 / difftime(end_time, start_time) + 1.0 / card_open_count) * 1000); // スコア計算
	// スコアに基づくメッセージ設定
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