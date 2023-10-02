#pragma once
#include <ctime>
#include <string>
#include <windows.h>
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

class Game {
public:
	// STATE
	GameState currentTransState = GameState::TITLE;
	GameState currentCardState = GameState::CARD0;
	void display_title(); // �^�C�g���\���֐�
	void start(); // �Q�[���{�Ҋ֐�
	void display_result(); //���U���g�\���֐�
private:
	const int CARD_SIZE = 20;
	Card card[20]; // 20���̃J�[�h
	Position pointer; // ��
	Position fc; // 1��ڂɗ��Ԃ����J�[�h
	Position sc; // 2��ڂɗ��Ԃ����J�[�h
	int remain_card_num; // ��Ɏc���Ă���J�[�h�̖���
	time_t start_time; // start�^�C�}�[
	time_t end_time; // end�^�C�}�[
	int card_open_count = 0; // �J�[�h���߂������񐔁i1�g��1��j
	int k; // �L�[����
	void message_explanation();// �當�����b�Z�[�W�ƗV�ѕ������\���֐�
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
	string gameExplanation_short =
		"�@���|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�[��\n"
		"�@ �b�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@ �b�y������@�z�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@ �b�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@ �b�@�@W(��) A(��) S(��) D(��)�F�J�[�\���ړ� �@ �@�@|\n"
		"�@ �b�@�@Enter�F�J�[�h���߂���@�@�@�@�@�@�@�@ �@�@�@ |\n"
		"�@ �b�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n"
		"�@���[�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�[�|��\n";
	// ���b�Z�[�W
	string message = "(^o^)�����ꂩ�玄�����������I";
	string good_message[4] = { "(��_��;)��A��邶��Ȃ��́I", "���̒��q�I�R(^o^)��", "(߄t�)�I�Ȃ�Ƃ����L���́I�I", "(/�E�ցE)/���݂��ƁI" };
	string bad_message[5] = { "(�L;��;�M)����ˁ`��", "��(..)�L���L���I���������I", "( *�L䇁M)�������", "(���L�́M)�� ����΂ꂥ�`�I", "(�K-�K)�撣���Ă�H" };	
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
	// �X�R�A
	int score;
	string score_message;
};