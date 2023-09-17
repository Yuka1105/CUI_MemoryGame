#pragma once
#include <array>
#include <stack>
using namespace std;

// class T���ő�MAXSIZE�m�ۉ\��PoolAllocator���������Ă�������
template<class T, size_t MAXSIZE> class PoolAllocator
{
public:
	// �R���X�g���N�^
	PoolAllocator() {

		// ���X�g���̎g�p�\�ȃA�h���X���i�[
		for (size_t i = 0; i < MAXSIZE; ++i) {
			pool_ad.push(&pool[i]);
		}
	}

	// �f�X�g���N�^
	~PoolAllocator() {

	}

	// �m�ۂł��Ȃ��ꍇ��nullptr��Ԃ����B
	T* Alloc() {

		if (pool_ad.empty()) {
			// �g�p�\�ȃA�h���X���Ȃ������ꍇ�i�����������m�ۂł��Ȃ������ꍇ�j
			return nullptr;
		}
		else {
			// �g�p�\�ȃA�h���X���������ꍇ�i�����������m�ۂł���ꍇ�j
			T* can_use = pool_ad.top();
			pool_ad.pop();
			return can_use;
		}
	}

	// Free(nullptr)�Ō듮�삵�Ȃ��悤�ɂ��鎖�B
	void Free(T* addr) {

		if (addr == nullptr) {

		}
		else {
			// �����̃A�h���X���g�p�\�ɂ���
			pool_ad.push(addr);
		}
	}

private:

	array<T, MAXSIZE> pool;
	stack<T*> pool_ad;
};
