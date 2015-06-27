#ifndef iw_vector_h_
#define iw_vector_h_


#include "trade.h"
#include "typedef.h"
#include <cstdlib>
#include <stdexcept>

namespace iw
{
	// constructfull

	template<typename X>
	class vector
	{
		X * content;
		uint Size;
		uint space;

		void ReSecure(uint size){
			X* another = static_cast<X*>(malloc(sizeof(X) *size));
			trade(content, another);
			uint oldsize = this->size();
			Size = 0;
			for (uint i = 0; i < oldsize; ++i){
				AddObject(another[i]);
				another[i].~X();
			}
			free(another);
			space = size;
		}

		void AddObject(){
			++Size;
			new(address(size() - 1)) X;
		}
		void AddObject(const X& req){
			++Size;
			new(address(size() - 1)) X(req);
		}

		void Init(){
			content = nullptr;
			Size = 0;
			space = 0;
		}

	public:

		iw::vector<X>& init(){
			zerosize();
			return *this;
		}
		vector(){ Init(); }
		iw::vector<X>& init(const iw::vector<X>& req){
			zerosize();
			add(req);
			return *this;
		}
		vector(const iw::vector<X>& req){
			Init();
			init(req);
		}
		iw::vector<X>& operator= (const iw::vector<X>& req){ return init(req); }
		iw::vector<X>& init(const X& req){
			zerosize();
			add(req);
			return *this;
		}
		vector(const X& req){
			Init();
			init(req);
		}
		iw::vector<X>& operator= (const X& req){ return init(req); }
		iw::vector<X>& init(const X* arr, const uint size){
			zerosize();
			add(arr, size);
			return *this;
		}
		vector(const X* arr, const uint size){
			Init();
			init(arr, size);
		}

		iw::vector<X>& secure(const uint size){
			if (size != space) ReSecure(size);
			return *this;
		}
		iw::vector<X>& requre(const uint size){
			if (size > space) ReSecure(size);
			return *this;
		}
		iw::vector<X>& addcure(const uint size){
			ReSecure(this->size() + size);
			return *this;
		}
		iw::vector<X>& popcure(const uint size){
			uint popsize = this->size();
			if (size < this->size()) popsize = size;
			secure(popsize);
			return *this;
		}
		iw::vector<X>& zerocure(){
			secure(0);
			return *this;
		}

		iw::vector<X>& setsize(const uint size){
			int gap = size - this->size();
			if (gap > 0) addsize(gap);
			if (gap < 0) popsize(-gap);
			return *this;
		}

		iw::vector<X>& reqsize(const uint size){
			if (size > this->size()) addsize(size - this->size());
			return *this;
		}

		iw::vector<X>& addsize(const uint size = 1){
			uint needsize = this->size() + size;
			if (needsize > space){
				if (needsize < 128) needsize = 128;
				ReSecure(needsize * 2);
			}
			for (uint i = 0; i<size; ++i) AddObject();
			return *this;
		}
		iw::vector<X>& addsize(const uint size, const X& req){
			uint needsize = this->size() + size;
			if (needsize > space){
				if (needsize < 128) needsize = 128;
				ReSecure(needsize * 2);
			}
			for (uint i = 0; i < size; ++i) AddObject(req);
			return *this;
		}
		iw::vector<X>& operator++ (){ return addsize(); }
		iw::vector<X> operator++ (int){
			iw::vector<X> old(*this);
			addsize();
			return old;
		}

		iw::vector<X>& popsize(const uint size = 1){
			uint gap = size;
			if (size > this->size()) gap = this->size();
			for (uint i = 0; i < gap; ++i)
			{
				last().~X();
				--Size;
			}
			return *this;
		}
		iw::vector<X>& operator-- (){ return popsize(); }
		iw::vector<X> operator-- (int){
			iw::vector<X> old(*this);
			popsize();
			return old;
		}

		iw::vector<X>& zerosize(){
			setsize(0);
			return *this;
		}

		iw::vector<X>& add(){
			uint needsize = size() + 1;
			if (needsize > space){
				if (needsize < 128) needsize = 128;
				ReSecure(needsize * 2);
			}
			AddObject();
			return *this;
		}
		iw::vector<X>& add(const X& req){
			uint needsize = size() + 1;
			if (needsize > space){
				if (needsize < 128) needsize = 128;
				ReSecure(needsize * 2);
			}
			AddObject(req);
			return *this;
		}
		iw::vector<X> & operator+= (const X& input){
			add(input);
			return *this;
		}
		iw::vector<X> & operator<< (const X& input){
			add(input);
			return *this;
		}
		iw::vector<X>& add(const X& req, const uint num){
			uint needsize = size() + num;
			if (needsize > space){
				if (needsize < 128) needsize = 128;
				ReSecure(needsize * 2);
			}
			for (uint i = 0; i < num; ++i) AddObject(req);
			return *this;
		}
		iw::vector<X> & add(const iw::vector<X>& input){
			requre(input.size());
			for (uint i = 0; i < input.size(); ++i) add(input[i]);
			return *this;
		}
		iw::vector<X> & operator+= (const iw::vector<X>& input){
			add(input);
			return *this;
		}
		iw::vector<X> & operator<< (const iw::vector<X>& input){
			add(input);
			return *this;
		}
		iw::vector<X>& add(const X* arr, const uint size){
			requre(size);
			for (uint i = 0; i < size; ++i) add(arr[i]);
			return *this;
		}

		iw::vector<X>& pop(const uint size = 1){ return popsize(size); }

		const X& access(const uint index) const {
			if (index >= size()){
				throw std::out_of_range("範囲外アクセスエラー");
			}
			return content[index];
		}
		const X& operator[] (const uint index) const { return access(index); }
		X& access(const uint index){
			const vector<X>& ci = *this;
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

		iw::vector<X> member(const uint index, const uint size) const {
			iw::vector<X> r(address(index), size);
			return r;
		}
		iw::vector<X> member(const uint size) const {
			iw::vector<X> r(address(0), size);
			return r;
		}

		iw::vector<X> operator+ (const iw::vector<X>& another) const {
			iw::vector<X> r(*this);
			r += another;
			return r;
		}
		iw::vector<X> operator+ (const X& another) const {
			iw::vector<X> r(*this);
			r += another;
			return r;
		}

		uint size() const { return Size; }

		~vector(){
			for (uint i = 0; i < size(); ++i) access(i).~X();
			free(content);
		}
	};

}

#endif