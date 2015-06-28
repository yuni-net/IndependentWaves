#ifndef iw_Array_h_
#define iw_Array_h_

#include <cstdlib>
#include <stdexcept>

#include "iw_trade.h"
#include "iw_typedef.h"

namespace iw
{
	/*
	 * �I���W�i���ϒ��z��N���X�B
	 * �������v�[�����쐬���A�K�v�ɉ�����placement new�ŃI�u�W�F�N�g���쐬����B
	 * visual studio �W����std::vector��荂���B
	 * �폜�ƒǉ��͏�ɖ����ɂ����čs����B
	 * �v�f�͏�ɘA�����ď��Ԓʂ�ɕ���ł��邱�Ƃ��ۏ؂���Ă���B
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

		// �������v�[���̃T�C�Y(�v�f�̌�)�𒼐ڎw�肷��B
		// �v�f�������炩���ߑ�܂��ɂł��킩���Ă���ꍇ�͂��̕��̃������v�[�����������Ă����Ǝ��s���x�����シ��B
		// ���݂̗v�f���������T�C�Y���w�肷��Ɩⓚ���p�Ńf�X�g���N�^���Ă΂ꃁ�����v�[���T�C�Y���ύX�����B
		iw::Array<X>& secure(const uint size){
			if (size != space) re_secure(size);
			return *this;
		}

		// �������v�[���̃T�C�Y(�v�f�̌�)�𑝉�������悤���N�G�X�g����B
		// size�ɂ͑�����̃T�C�Y���w�肷��B
		// �w�肵���T�C�Y��茻�݂̃T�C�Y�̕����傫�������ꍇ�͉����N����Ȃ��B
		iw::Array<X>& requre(const uint size){
			if (size > space) re_secure(size);
			return *this;
		}

		// �������v�[���̃T�C�Y(�v�f�̌�)��size�Ɏw�肵������������������B
		iw::Array<X>& addcure(const uint size){
			re_secure(this->size() + size);
			return *this;
		}

		// �������v�[���̃T�C�Y(�v�f�̌�)��size�Ɏw�肵������������������B
		iw::Array<X>& popcure(const uint size){
			uint popsize = this->size();
			if (size < this->size()) popsize = size;
			secure(popsize);
			return *this;
		}

		// �������v�[���̃T�C�Y(�v�f�̌�)���[���ɂ���B
		// �ⓚ���p�őS�Ẵf�X�g���N�^���Ă΂ꃁ�����v�[���͏��ł���B
		iw::Array<X>& zerocure(){
			secure(0);
			return *this;
		}

		// �v�f�����w�肷��B
		// �V�K�ǉ����ꂽ�v�f�̓f�t�H���g�R���X�g���N�^�ŏ����������B
		// �폜���ꂽ�v�f�̓f�X�g���N�^���Ă΂��B
		iw::Array<X>& setsize(const uint size){
			int gap = size - this->size();
			if (gap > 0) addsize(gap);
			if (gap < 0) popsize(-gap);
			return *this;
		}

		// �v�f���𑝉�������悤���N�G�X�g����B
		// size�ɂ͑�����̗v�f�����w�肷��B
		// �w�肵���v�f����茻�݂̗v�f���̕����傫�������ꍇ�͉����N����Ȃ��B
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
				throw std::out_of_range("�͈͊O�A�N�Z�X�G���[");
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