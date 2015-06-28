#ifndef iw_Array_h_
#define iw_Array_h_

#include <cstdlib>
#include <stdexcept>

#include "iw_trade.h"
#include "iw_typedef.h"

namespace iw
{
	/*
	 * オリジナル可変長配列クラス。
	 * メモリプールを作成し、必要に応じてplacement newでオブジェクトを作成する。
	 * visual studio 標準のstd::vectorより高速。
	 * 削除と追加は常に末尾において行われる。
	 * 要素は常に連続して順番通りに並んでいることが保証されている。
	 */
	template<typename X>
	class Array
	{
	public:

		iw::Array<X>& init(){
			zerosize();
			return *this;
		}

		Array(){ construct(); }

		iw::Array<X>& init(const iw::Array<X>& req){
			zerosize();
			add(req);
			return *this;
		}

		Array(const iw::Array<X>& req){
			construct();
			init(req);
		}

		iw::Array<X>& operator= (const iw::Array<X>& req){ return init(req); }

		iw::Array<X>& init(const X& req){
			zerosize();
			add(req);
			return *this;
		}

		Array(const X& req){
			construct();
			init(req);
		}

		iw::Array<X>& operator= (const X& req){ return init(req); }

		iw::Array<X>& init(const X* arr, const uint size){
			zerosize();
			add(arr, size);
			return *this;
		}

		Array(const X* arr, const uint size){
			construct();
			init(arr, size);
		}

		// メモリプールのサイズ(要素の個数)を直接指定する。
		// 要素数があらかじめ大まかにでもわかっている場合はその分のメモリプールを準備しておくと実行速度が向上する。
		// 現在の要素数を下回るサイズを指定すると問答無用でデストラクタが呼ばれメモリプールサイズが変更される。
		iw::Array<X>& secure(const uint size){
			if (size != space) re_secure(size);
			return *this;
		}

		// メモリプールのサイズ(要素の個数)を増加させるようリクエストする。
		// sizeには増加後のサイズを指定する。
		// 指定したサイズより現在のサイズの方が大きかった場合は何も起こらない。
		iw::Array<X>& requre(const uint size){
			if (size > space) re_secure(size);
			return *this;
		}

		// メモリプールのサイズ(要素の個数)をsizeに指定した数だけ増加させる。
		iw::Array<X>& addcure(const uint size){
			re_secure(this->size() + size);
			return *this;
		}

		// メモリプールのサイズ(要素の個数)をsizeに指定した数だけ減少させる。
		iw::Array<X>& popcure(const uint size){
			uint popsize = this->size();
			if (size < this->size()) popsize = size;
			secure(popsize);
			return *this;
		}

		// メモリプールのサイズ(要素の個数)をゼロにする。
		// 問答無用で全てのデストラクタが呼ばれメモリプールは消滅する。
		iw::Array<X>& zerocure(){
			secure(0);
			return *this;
		}

		// 要素数を指定する。
		// 新規追加された要素はデフォルトコンストラクタで初期化される。
		// 削除された要素はデストラクタが呼ばれる。
		iw::Array<X>& setsize(const uint size){
			int gap = size - this->size();
			if (gap > 0) addsize(gap);
			if (gap < 0) popsize(-gap);
			return *this;
		}

		// 要素数を増加させるようリクエストする。
		// sizeには増加後の要素数を指定する。
		// 指定した要素数より現在の要素数の方が大きかった場合は何も起こらない。
		iw::Array<X>& reqsize(const uint size){
			if (size > this->size()) addsize(size - this->size());
			return *this;
		}

		iw::Array<X>& addsize(const uint size = 1){
			uint needsize = this->size() + size;
			if (needsize > space){
				if (needsize < 128) needsize = 128;
				re_secure(needsize * 2);
			}
			for (uint i = 0; i<size; ++i) AddObject();
			return *this;
		}
		iw::Array<X>& addsize(const uint size, const X& req){
			uint needsize = this->size() + size;
			if (needsize > space){
				if (needsize < 128) needsize = 128;
				re_secure(needsize * 2);
			}
			for (uint i = 0; i < size; ++i) AddObject(req);
			return *this;
		}
		iw::Array<X>& operator++ (){ return addsize(); }
		iw::Array<X> operator++ (int){
			iw::Array<X> old(*this);
			addsize();
			return old;
		}

		iw::Array<X>& popsize(const uint size = 1){
			uint gap = size;
			if (size > this->size()) gap = this->size();
			for (uint i = 0; i < gap; ++i)
			{
				last().~X();
				--elem_size;
			}
			return *this;
		}
		iw::Array<X>& operator-- (){ return popsize(); }
		iw::Array<X> operator-- (int){
			iw::Array<X> old(*this);
			popsize();
			return old;
		}

		iw::Array<X>& zerosize(){
			setsize(0);
			return *this;
		}

		iw::Array<X>& add(){
			uint needsize = size() + 1;
			if (needsize > space){
				if (needsize < 128) needsize = 128;
				re_secure(needsize * 2);
			}
			AddObject();
			return *this;
		}
		iw::Array<X>& add(const X& req){
			uint needsize = size() + 1;
			if (needsize > space){
				if (needsize < 128) needsize = 128;
				re_secure(needsize * 2);
			}
			AddObject(req);
			return *this;
		}
		iw::Array<X> & operator+= (const X& input){
			add(input);
			return *this;
		}
		iw::Array<X> & operator<< (const X& input){
			add(input);
			return *this;
		}
		iw::Array<X>& add(const X& req, const uint num){
			uint needsize = size() + num;
			if (needsize > space){
				if (needsize < 128) needsize = 128;
				re_secure(needsize * 2);
			}
			for (uint i = 0; i < num; ++i) AddObject(req);
			return *this;
		}
		iw::Array<X> & add(const iw::Array<X>& input){
			requre(input.size());
			for (uint i = 0; i < input.size(); ++i) add(input[i]);
			return *this;
		}
		iw::Array<X> & operator+= (const iw::Array<X>& input){
			add(input);
			return *this;
		}
		iw::Array<X> & operator<< (const iw::Array<X>& input){
			add(input);
			return *this;
		}
		iw::Array<X>& add(const X* arr, const uint size){
			requre(size);
			for (uint i = 0; i < size; ++i) add(arr[i]);
			return *this;
		}

		iw::Array<X>& pop(const uint size = 1){ return popsize(size); }

		const X& access(const uint index) const {
			if (index >= size()){
				throw std::out_of_range("範囲外アクセスエラー");
			}
			return content[index];
		}
		const X& operator[] (const uint index) const { return access(index); }
		X& access(const uint index){
			const Array<X>& ci = *this;
			return const_cast<X&>(ci.access(index));
		}
		X& operator[] (const uint index){ return access(index); }
		X value(const uint index) const { return access(index); }

		const X* address(const uint index) const { return &access(index); }
		X* address(const uint index){ return &access(index); }
		const X* head() const {
			if (size() == 0) return nullptr;
			return address(0);
		}
		X* head(){
			if (size() == 0) return nullptr;
			return address(0);
		}

		const X& last() const { return access(size() - 1); }
		X& last(){ return access(size() - 1); }
		X lastvalue() const { return last(); }

		X& next(){
			addsize();
			return last();
		}
		X nextvalue(){ return next(); }
		X& next(const X& req){
			add(req);
			return last();
		}
		X nextvalue(const X& req){ return next(req); }

		iw::Array<X> member(const uint index, const uint size) const {
			iw::Array<X> r(address(index), size);
			return r;
		}
		iw::Array<X> member(const uint size) const {
			iw::Array<X> r(address(0), size);
			return r;
		}

		iw::Array<X> operator+ (const iw::Array<X>& another) const {
			iw::Array<X> r(*this);
			r += another;
			return r;
		}
		iw::Array<X> operator+ (const X& another) const {
			iw::Array<X> r(*this);
			r += another;
			return r;
		}

		uint size() const { return elem_size; }

		~Array(){
			for (uint i = 0; i < size(); ++i) access(i).~X();
			free(content);
		}


		private:
			X * content;
			uint elem_size;
			uint space;

			void re_secure(uint size){
				X* another = static_cast<X*>(malloc(sizeof(X) *size));
				trade(content, another);
				uint oldsize = this->size();
				elem_size = 0;
				for (uint i = 0; i < oldsize; ++i){
					AddObject(another[i]);
					another[i].~X();
				}
				free(another);
				space = size;
			}

			void AddObject(){
				++elem_size;
				new(address(size() - 1)) X;
			}
			void AddObject(const X& req){
				++elem_size;
				new(address(size() - 1)) X(req);
			}

			void construct(){
				content = nullptr;
				elem_size = 0;
				space = 0;
			}

	};

}

#endif