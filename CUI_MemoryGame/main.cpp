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

//�^�C�g���A�Q�[���{�ҁA���U���g�A�J�����J�[�h�̖����ɂ��X�e�[�g��ύX
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

	// �G�X�P�[�v�V�[�P���X��L����
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(hStdOut, &mode);
	SetConsoleMode(hStdOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	// �J�[�\��������
	printf("\x1b[?25l");

	// �L�[����
	int k;

	// �^�C�}�[
	time_t start_time;
	time_t end_time;

	// �J�[�h���߂������񐔁i1�g��1��j
	int card_open_count = 0;

	// �J�[�h�̐����������_����
	string num[] = { "�P", "�P", "�Q", "�Q", "�R", "�R", "�S", "�S", "�T", "�T", "�U", "�U", "�V", "�V", "�W" , "�W", "�X", "�X", "�O", "�O" };
	// ������������g�p���ăV���b�t��
	random_device rd;
	mt19937 gen(rd());
	shuffle(begin(num), end(num), gen);

	// �J�[�h��������
	const int CARD_SIZE = 20;
	Card card[CARD_SIZE];
	for (int i = 0; i < CARD_SIZE; i++) {
		card[i].card_num = num[i];
		card[i].is_remain = true;
	}

	//�|�C���g�����A�ŏ��ɗ��Ԃ����J�[�h�A���ɗ��Ԃ����J�[�h�̍��W�ʒu
	Position pointer = { 0, 0 };
	Position fc = { 0, 0 };
	Position sc = { 0, 0 };
	// ��Ɏc���Ă���J�[�h�̖���
	int remain_card_num = 20;

	// �������b�Z�[�W
	string message = "(^o^)�����ꂩ�玄�����������I";
	// ��������Ȃ������Ƃ�
	string bad_message[] = { "(�L;��;�M)����ˁ`��", "��(..)�L���L���I���������I", "( *�L䇁M)�������", "(���L�́M)�� ����΂ꂥ�`�I", "(�K-�K)�撣���Ă�H" };
	// �����������Ƃ�
	string good_message[] = { "(��_��;)��A��邶��Ȃ��́I", "���̒��q�I�R(^o^)��", "(߄t�)�I�Ȃ�Ƃ����L���́I�I", "(/�E�ցE)/���݂��ƁI" };

	// �V�ѕ�����
	string gameExplanation =
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
	string gameExplanation_short =
		"�@���|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�[��\n"
		"�@ �b�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@ �b�y������@�z�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@ �b�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@ �b�@�@W(��) A(��) S(��) D(��)�F�J�[�\���ړ� �@ �@�@|\n"
		"�@ �b�@�@Enter�F�J�[�h���߂���@�@�@�@�@�@�@�@ �@�@�@ |\n"
		"�@ �b�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@���[�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�[�|��\n";
	string clearMessage =
		"�@�@���|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�[�[��\n"
		"�@�@�@���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��\n"
		"�@�@�@�b�@�@�Q�[���N���A�I�I�I�@�@�@�@�@�@|\n"
		" �@�@  ���@ �����l�I�悭�撣��܂����I ��\n"
		" �@�@ �b�@�@���́A���ʔ��\����I�@�@ �@ |\n"
		"�@�@�@���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��\n"
		"�@�@���[�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�[�[�|��\n";

	// ������ʑJ�ڃX�e�[�g
	GameState currentTransState = GameState::TITLE;
	// �����J�[�h�X�e�[�g
	GameState currentCardState = GameState::CARD0;

	while (currentTransState != GameState::RESULT) {
		// ��ʂ��N���A
		system("cls");
		switch (currentTransState) {
		case GameState::TITLE:

			cout << endl << endl;
			cout << "�@�@�@�@�@�@�@�@�@�@�@�@�@ �@(^O^) �ЂƂ�_�o���� (^O^)" << endl << endl;
			cout << "�@�@�@�@�@�@�@�@�@�@�@�@�@�@�G���^�[�L�[�������Ǝn�܂��I" << endl << endl;
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
				// ��ʂ��N���A
				system("cls");
				cout << endl << endl << endl << "�@�@�@�Q�[���܂ŁF" << i << endl;
			std:this_thread::sleep_for(std::chrono::seconds(1));
			}

			// �Q�[���J�n�Ɠ����Ƀ^�C�}�[�Z�b�g
			start_time = time(nullptr);

			//�L�[���͂��Ƃɉ�ʍX�V�A�J�[�h�̕`��
			do
			{
				// ��ʂ��N���A
				system("cls");
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
							cout << endl;
							cout << endl;
						}
						if (column == pointer.x && row == pointer.y) {
							if (column == 0) {
								cout << "�@";
							}
							cout << "��" << "�@";
						}
						else {
							if (column == 0) {
								cout << "�@";
							}
							if (card[i].is_remain == true) {
								cout << "��" << "�@";
							}
							else {
								cout << "�@" << "�@";
							}
						}
						column += 1;
					}

					// ���b�Z�[�W
					cout << endl << endl << endl;
					cout << "�@" << message << endl << endl << endl;
					// �V�ѕ�����
					cout << gameExplanation_short;

					break;

					// 1���J�[�h���J�����Ƃ�
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
								cout << "�@";
							}
							cout << card[i].card_num << "�@";
						}
						else if (column == pointer.x && row == pointer.y) {
							if (column == 0) {
								cout << "�@";
							}
							cout << "��" << "�@";
						}
						else {
							if (column == 0) {
								cout << "�@";
							}
							if (card[i].is_remain == true) {
								cout << "��" << "�@";
							}
							else {
								cout << "�@" << "�@";
							}
						}
						column += 1;
					}

					// ���b�Z�[�W
					cout << endl << endl << endl;
					cout << "�@" << message << endl << endl << endl;
					// �V�ѕ�����
					cout << gameExplanation_short;

					break;

					// 2���J�[�h���J�����Ƃ�
				case GameState::CARD2:
					// 2���̃J�[�h�̐������������ꍇ
					if (card[fc.y * 5 + fc.x].card_num == card[sc.y * 5 + sc.x].card_num) {

						// �����_���ȃ��b�Z�[�W��ݒ�
						// ������������g�p���ăV���b�t��
						random_device rd_good;
						mt19937 gen(rd_good());
						shuffle(begin(good_message), end(good_message), gen);
						message = good_message[0];

						card[fc.y * 5 + fc.x].is_remain = false;
						card[sc.y * 5 + sc.x].is_remain = false;
						// ��ɂ���J�[�h�̖������Ǘ�
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
									cout << "�@";
								}
								cout << "�@" << "�@";
							}
							else if (column == sc.x && row == sc.y) {
								if (column == 0) {
									cout << "�@";
								}
								cout << "�@" << "�@";
							}
							else {
								if (column == 0) {
									cout << "�@";
								}
								if (card[i].is_remain == true) {
									cout << "��" << "�@";
								}
								else {
									cout << "�@" << "�@";
								}
							}
							column += 1;
						}

						// ���b�Z�[�W
						cout << endl << endl << endl;
						cout << "�@" << message << endl << endl << endl;
						// �V�ѕ�����
						cout << gameExplanation_short;

						// ��莞�Ԍ��CARD0�ɑJ�ڂ���
						// std::this_thread::sleep_for(std::chrono::seconds(1)); // 1�b�x��
						fc = { 0, 0 };
						sc = { 0, 0 };
						currentCardState = GameState::CARD0;
						continue;
					}
					// 2���̃J�[�h�̐���������Ȃ������ꍇ
					else {

						// �����_���ȃ��b�Z�[�W��ݒ�
						// ������������g�p���ăV���b�t��
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
									cout << "��" << "�@";
								}
								else {
									cout << "�@" << "�@";
								}
							}
							column += 1;
						}

						// ���b�Z�[�W
						cout << endl << endl << endl;
						cout << "�@" << message << endl << endl << endl;
						// �V�ѕ�����
						cout << gameExplanation_short;

						// ��莞�Ԍ��CARD0�ɑJ�ڂ���
						std::this_thread::sleep_for(std::chrono::seconds(1)); // 1�b�x��
						fc = { 0, 0 };
						sc = { 0, 0 };
						currentCardState = GameState::CARD0;
						continue;
					}
					break;
				}

				if (remain_card_num == 0) {
					// �Q�[���N���A�Ɠ����Ƀ^�C�}�[���~�߂�
					end_time = time(nullptr);

					// ��ʂ��N���A
					system("cls");
					// ���b�Z�[�W
					cout << endl << endl << endl;
					cout << clearMessage << endl;
					currentTransState = GameState::RESULT;
					// ��莞�Ԍ��RESULT�ɑJ�ڂ���
					std::this_thread::sleep_for(std::chrono::seconds(4)); // 4�b�x��
					break;
				}

				// �o�O�`�F�b�N
				//cout << endl;
				//cout << fc.x << " " << fc.y << " " << sc.x << " " << sc.y << endl;
				//cout << pointer.x << " " << pointer.y << endl;

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
			} while ('q' != k); //  'q'�L�[�ŏI��
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

		// ��ʂ��N���A
		system("cls");
		// ���ʔ��\
		cout << endl;
		cout << "�@���ʔ��\" << endl << endl;
		cout << "�@�X�R�A: " << score << endl << endl;
		cout << "�@�J���҂��炠�Ȃ��փR�����g: " << score_message << endl << endl;
		cout << "�@�N���A�ɂ�����������: " << difftime(end_time, start_time) << " �b" << endl << endl;
		cout << "�@�J�[�h���J������: " << card_open_count << " ��" << endl << endl;
		cout << "�@�V��ł���Ă��肪�Ƃ��������܂����I" << endl << endl << endl;
		cout << "�@�� �G���^�[�L�[�������ƃQ�[�����I�����܂��B" << endl;

		// �L�[���͂�҂�
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