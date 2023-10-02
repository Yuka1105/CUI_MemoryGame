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
	// �G�X�P�[�v�V�[�P���X��L����
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(hStdOut, &mode);
	SetConsoleMode(hStdOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	// �J�[�\��������
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
	cout << "\n\n\n�@" << message << "\n\n\n"; // �當�����b�Z�[�W
	cout << gameExplanation_short; // �V�ѕ�����
}

void Game::start() {
	// �J�[�h���V���b�t�����ď�����
	string num[] = { "�P", "�P", "�Q", "�Q", "�R", "�R", "�S", "�S", "�T", "�T", "�U", "�U", "�V", "�V", "�W" , "�W", "�X", "�X", "�O", "�O" };
	random_device rd;
	mt19937 gen(rd());
	shuffle(begin(num), end(num), gen);
	for (int i = 0; i < CARD_SIZE; i++) {
		card[i].card_num = num[i];
		card[i].is_remain = true;
	}
	// �e�평����
	pointer = { 0, 0 };
	fc = { 0, 0 };
	sc = { 0, 0 };
	remain_card_num = 20;
	// �Q�[���J�n�܂ł̃J�E���g�_�E��
	for (int i = 3; i >= 1; i--) {
		system("cls"); // ��ʂ��N���A
		cout << "\n\n\n�@�@�@�Q�[���܂ŁF" << i << endl;
		this_thread::sleep_for(chrono::seconds(1));
	}
	// �Q�[���J�n�Ƌ��Ƀ^�C�}�[�Z�b�g
	start_time = time(nullptr);
	// �Q�[���J�n �L�[���͂��Ƃɉ�ʍX�V & �J�[�h�`��
	while ('q' != k)
	{
		system("cls"); // ��ʂ��N���A
		cout << endl;
		// 20���̃J�[�h�̍��W�ʒu���Ǘ�
		int row = 0;
		int column = 0;
		switch (currentCardState) {
		// 1�����J�[�h���J���ĂȂ��Ƃ�
		case GameState::CARD0:
			for (int i = 0; i < CARD_SIZE; i++) {
				if (i % 5 == 0 && i != 0) {
					column = 0;
					row += 1;
					cout << "\n\n";
				}
				if (column == pointer.x && row == pointer.y) {
					if (column == 0) {
						cout << "�@";
					}
					cout << "���@";
				}
				else {
					if (column == 0) {
						cout << "�@";
					}
					if (card[i].is_remain == true) {
						cout << "���@";
					}
					else {
						cout << "�@�@";
					}
				}
				column += 1;
			}
			message_explanation();
			break;
		// 1���J�[�h���J�����Ƃ�
		case GameState::CARD1:
			for (int i = 0; i < CARD_SIZE; i++) {
				if (i % 5 == 0 && i != 0) {
					column = 0;
					row += 1;
					cout << "\n\n";
				}
				if (column == fc.x && row == fc.y) {
					if (column == 0) {
						cout << "�@";
					}
					cout << card[i].card_num << "�@";
				}
				else if (column == pointer.x && row == pointer.y) {
					if (column == 0) {
						cout << "�@";
					}
					cout << "���@";
				}
				else {
					if (column == 0) {
						cout << "�@";
					}
					if (card[i].is_remain == true) {
						cout << "���@";
					}
					else {
						cout << "�@�@";
					}
				}
				column += 1;
			}
			message_explanation();
			break;
		// 2���J�[�h���J�����Ƃ�
		case GameState::CARD2:
			// ����
			if (card[fc.y * 5 + fc.x].card_num == card[sc.y * 5 + sc.x].card_num) {
				// �����_���ȃ��b�Z�[�W��ݒ�
				random_device rd_good;
				mt19937 gen(rd_good());
				shuffle(begin(good_message), end(good_message), gen);
				message = good_message[0];
				card[fc.y * 5 + fc.x].is_remain = false;
				card[sc.y * 5 + sc.x].is_remain = false;
				remain_card_num -= 2; // ��ɂ���J�[�h�̖������Ǘ�
				for (int i = 0; i < CARD_SIZE; i++) {
					if (i % 5 == 0 && i != 0) {
						column = 0;
						row += 1;
						cout << "\n\n";
					}
					if (column == fc.x && row == fc.y) {
						if (column == 0) {
							cout << "�@";
						}
						cout << "�@�@";
					}
					else if (column == sc.x && row == sc.y) {
						if (column == 0) {
							cout << "�@";
						}
						cout << "�@�@";
					}
					else {
						if (column == 0) {
							cout << "�@";
						}
						if (card[i].is_remain == true) {
							cout << "���@";
						}
						else {
							cout << "�@�@";
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
			// �s����
			else {
				// �����_���ȃ��b�Z�[�W��ݒ�
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
							cout << "�@";
						}
						cout << card[i].card_num << "�@";
					}
					else if (column == sc.x && row == sc.y) {
						if (column == 0) {
							cout << "�@";
						}
						cout << card[i].card_num << "�@";
					}
					else {
						if (column == 0) {
							cout << "�@";
						}
						if (card[i].is_remain == true) {
							cout << "���@";
						}
						else {
							cout << "�@�@";
						}
					}
					column += 1;
				}
				message_explanation();
				this_thread::sleep_for(chrono::seconds(1)); // 1�b��CARD0�ɑJ��
				fc = { 0, 0 };
				sc = { 0, 0 };
				currentCardState = GameState::CARD0;
				continue;
			}
			break;
		}
		if (remain_card_num == 0) {
			end_time = time(nullptr); // �Q�[���N���A�Ɠ����Ƀ^�C�}�[���~�߂�
			system("cls"); // ��ʂ��N���A
			cout << clearMessage; // �N���A���b�Z�[�W
			currentTransState = GameState::RESULT;
			this_thread::sleep_for(chrono::seconds(4)); // 4�b��RESULT�ɑJ��
			break;
		}
		// �L�[���͑҂�
		k = _getch();
		// �L�[���͂ō��W�X�Vor�X�e�[�g�ύX
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
	score = static_cast<int>((1.0 / difftime(end_time, start_time) + 1.0 / card_open_count) * 1000); // �X�R�A�v�Z
	// �X�R�A�Ɋ�Â����b�Z�[�W�ݒ�
	if (score < 40) {
		score_message = "D�����N�I �����Ȃ�����Ɗ撣���͂��I(�K-�K)";
	}
	else if (score < 55) {
		score_message = "C�����N�I �ɂ����I�ǂ����Ȃ�B�����N�߂������I�H(^O^)";
	}
	else if (score < 70) {
		score_message = "B�����N�I �ǂ���炻�������^���ɂ���Ă��ꂽ�݂����I�H(�K-�K)���肪�Ƃ��I";
	}
	else if (score < 85) {
		score_message = "A�����N�I �Ȃ��Ȃ��������I�I���������āA�_�o����߂����ᓾ�ӁI�H(*''��'')";
	}
	else {
		score_message = "S�����N�I �Ȃ��Ȃ����̈�ɂ͒B���Ȃ��ł��B�f���炵���I���߂łƂ��������܂��B( ^^) _�U~~";
	}
	system("cls"); // ��ʂ��N���A
	cout << "\n�@���ʔ��\\n\n";
	cout << "�@�X�R�A: " << score << "\n\n";
	cout << "�@�J���҂��炠�Ȃ��փR�����g: " << score_message << "\n\n";
	cout << "�@�N���A�ɂ�����������: " << difftime(end_time, start_time) << " �b\n\n";
	cout << "�@�J�[�h���J������: " << card_open_count << " ��\n\n";
	cout << "�@�V��ł���Ă��肪�Ƃ��������܂����I\n\n\n";
	cout << "�@�� �G���^�[�L�[�������ƃQ�[�����I�����܂��B" << endl;
	// �L�[���͑҂�
	k = _getch();
	if (k == 13) {
		currentTransState = GameState::EXIT;
	};
}