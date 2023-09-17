#pragma once
#include <array>
#include <stack>
using namespace std;

// class Tを最大MAXSIZE個確保可能なPoolAllocatorを実装してください
template<class T, size_t MAXSIZE> class PoolAllocator
{
public:
	// コンストラクタ
	PoolAllocator() {

		// リスト内の使用可能なアドレスを格納
		for (size_t i = 0; i < MAXSIZE; ++i) {
			pool_ad.push(&pool[i]);
		}
	}

	// デストラクタ
	~PoolAllocator() {

	}

	// 確保できない場合はnullptrを返す事。
	T* Alloc() {

		if (pool_ad.empty()) {
			// 使用可能なアドレスがなかった場合（＝メモリを確保できなかった場合）
			return nullptr;
		}
		else {
			// 使用可能なアドレスがあった場合（＝メモリを確保できる場合）
			T* can_use = pool_ad.top();
			pool_ad.pop();
			return can_use;
		}
	}

	// Free(nullptr)で誤動作しないようにする事。
	void Free(T* addr) {

		if (addr == nullptr) {

		}
		else {
			// 引数のアドレスを使用可能にする
			pool_ad.push(addr);
		}
	}

private:

	array<T, MAXSIZE> pool;
	stack<T*> pool_ad;
};
