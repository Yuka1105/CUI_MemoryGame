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

// �V�[��
enum class GameState { TITLE, GAME, RESULT, EXIT, CARD0, CARD1, CARD2 };

// �J�[�h�̏��
struct Card {
	string card_num;
	bool is_remain;
};

// �ʒu
struct Position {
	int x;
	int y;
};

int main() {

	// �G�X�P�[�v�V�[�P���X��L����
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(hStdOut, &mode);
	SetConsoleMode(hStdOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	// �J�[�\��������
	printf("\x1b[?25l");
	
	int k; // �L�[����
	time_t start_time; // �^�C�}�[
	time_t end_time;
	int card_open_count = 0; // �J�[�h���߂������񐔁i1�g��1��j

	// �J�[�h��������
	const int CARD_SIZE = 20;
	Card card[CARD_SIZE];
	string num[] = { "�P", "�P", "�Q", "�Q", "�R", "�R", "�S", "�S", "�T", "�T", "�U", "�U", "�V", "�V", "�W" , "�W", "�X", "�X", "�O", "�O" };
	random_device rd; // �J�[�h���V���b�t��
	mt19937 gen(rd());
	shuffle(begin(num), end(num), gen);
	for (int i = 0; i < CARD_SIZE; i++) {
		card[i].card_num = num[i];
		card[i].is_remain = true;
	}

	//�@�ʒu
	Position pointer = { 0, 0 }; // ��
	Position fc = { 0, 0 }; // 1��ڂɗ��Ԃ����J�[�h
	Position sc = { 0, 0 }; // 2��ڂɗ��Ԃ����J�[�h

	int remain_card_num = 20; // ��Ɏc���Ă���J�[�h�̖���

	// �������b�Z�[�W
	string message = "(^o^)�����ꂩ�玄�����������I";
	// ����
	string bad_message[] = { "(�L;��;�M)����ˁ`��", "��(..)�L���L���I���������I", "( *�L䇁M)�������", "(���L�́M)�� ����΂ꂥ�`�I", "(�K-�K)�撣���Ă�H" };
	// �s����
	string good_message[] = { "(��_��;)��A��邶��Ȃ��́I", "���̒��q�I�R(^o^)��", "(߄t�)�I�Ȃ�Ƃ����L���́I�I", "(/�E�ցE)/���݂��ƁI" };
	
	// �V�ѕ�����
	string gameExplanation =
		"\n\n"
		"�@�@�@�@�@�@�@�@�@�@�@�@�@ �@(^O^) �ЂƂ�_�o���� (^O^)\n\n"
		"�@�@�@�@�@�@�@�@�@�@�@�@�@�@�G���^�[�L�[�������Ǝn�܂��I\n\n"
		"�@���|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�[�[�[�[�[�|��\n"
		"�@ �b�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@ �b�y�V�ѕ��z�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@ �b�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@|\n"
		"�@ �b�@�ЂƂ�_�o����Ƃ́H�H�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@ �b�@�@1�l�łЂ�����J�[�h���߂���A���������̃J�[�h�������ăy�A������܂��B�@  |\n"
		"�@ �b�@�@���ׂẴJ�[�h���y�A�ɂł�����Q�[���N���A�ł��B�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@ �b�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@|\n"
		"�@ �b�@������@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@ �b�@�@W(��) A(��) S(��) D(��)�F�J�[�\���ړ� �@ �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@|\n"
		"�@ �b�@�@Enter�F�J�[�h���߂���@�@�@�@�@�@�@�@�@�@�@�@ �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@ �b�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@ �b�@�|�C���g�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@ �b�@�@���@�ł��邾�������@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@ �b�@�@���@�߂���񐔂����Ȃ߂Ł�  �@�@�@  �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ �@�@�@|\n"
		"�@ �b�@�@�Ƃ肠�����A�͂��߂Ă݂悤�I�@�@�@�@�@�@�@�@   �@�@�@  �@�@�@�@ �@�@�@�@ �@�@|\n"
		"�@ �b�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@���[�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�[�[�|�|�[�[��\n";
	
	// �Q�[���J�E���g�e�L�X�g
	string gameCountText =
		"\n\n\n�@�@�@�Q�[���܂ŁF";
	
	// �V���[�g�ŗV�ѕ�����
	string gameExplanation_short =
		"�@���|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�[��\n"
		"�@ �b�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@ �b�y������@�z�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@ �b�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@ �b�@�@W(��) A(��) S(��) D(��)�F�J�[�\���ړ� �@ �@�@|\n"
		"�@ �b�@�@Enter�F�J�[�h���߂���@�@�@�@�@�@�@�@ �@�@�@ |\n"
		"�@ �b�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@���[�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�[�|��\n";
	
	// �N���A��̃��b�Z�[�W
	string clearMessage =
		"\n\n\n"
		"�@�@���|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�[�[��\n"
		"�@�@�@���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��\n"
		"�@�@�@�b�@�@�Q�[���N���A�I�I�I�@�@�@�@�@�@|\n"
		" �@�@  ���@ �����l�I�悭�撣��܂����I ��\n"
		" �@�@ �b�@�@���́A���ʔ��\����I�@�@ �@ |\n"
		"�@�@�@���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��\n"
		"�@�@���[�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�[�[�|��\n"
		"\n";

	GameState currentTransState = GameState::TITLE; // ������ʑJ�ڃX�e�[�g
	GameState currentCardState = GameState::CARD0; // �����J�[�h�X�e�[�g

	while (currentTransState != GameState::RESULT) {
		system("cls"); // ��ʂ��N���A
		switch (currentTransState) {
		case GameState::TITLE:
			cout << gameExplanation;
			do {
				k = _getch();
				if (k == 13) {
					currentTransState = GameState::GAME;
				}
			} while (k != 13);
			break;
		case GameState::GAME:
			// �J�E���g�_�E��
			for (int i = 3; i >= 1; i--) {
				system("cls"); // ��ʂ��N���A
				cout << gameCountText << i << endl;
				this_thread::sleep_for(chrono::seconds(1));
			}
			// �Q�[���J�n�Ƌ��Ƀ^�C�}�[�Z�b�g
			start_time = time(nullptr);
			// �L�[���͂��Ƃɉ�ʍX�V & �J�[�h�`��
			do
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
					cout << "\n\n\n�@" << message << "\n\n\n"; // �當�����b�Z�[�W
					cout << gameExplanation_short; // �V�ѕ�����
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
					cout << "\n\n\n�@" << message << "\n\n\n"; // �當�����b�Z�[�W
					cout << gameExplanation_short; // �V�ѕ�����
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
						cout << "\n\n\n�@" << message << "\n\n\n"; // �當�����b�Z�[�W
						cout << gameExplanation_short; // �V�ѕ�����
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
						cout << "\n\n\n�@" << message << "\n\n\n"; // �當�����b�Z�[�W
						cout << gameExplanation_short; // �V�ѕ�����
						
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
			} while ('q' != k); // 'q'�L�[�ŏI��
			break;
		}
	}
	do {
		// �X�R�A�v�Z
		int score = static_cast<int>((1.0 / difftime(end_time, start_time) + 1.0 / card_open_count) * 1000);
		string score_message;
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
		// ���ʔ��\
		cout << endl;
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
			break;
		};
	} while (currentTransState == GameState::RESULT);

	// �J�[�\����\��
	printf("\x1b[?25h");
	return EXIT_SUCCESS;
}