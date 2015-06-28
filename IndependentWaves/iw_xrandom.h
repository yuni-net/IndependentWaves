#ifndef iw_xrandom_h_
#define iw_xrandom_h_

namespace iw
{
	// 乱数を取得する
	// 初期化必須！下記のxrandom_init関数を使用すること。
	// 取得した値が全てゼロになる場合は初期化されていない証拠
	// 複数回初期化しても問題無い
	unsigned long xrandom();

	// xrandom関数で取得する乱数のシードを設定する
	// 0以外であれば何でもよい
	// 他の多くの乱数生成器と同じように、同じシードを設定すれば常に同じ乱数が生成される
	void xrandom_init(unsigned long seed);

}

#endif