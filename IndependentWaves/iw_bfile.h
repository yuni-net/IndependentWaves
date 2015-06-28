#ifndef iw_bfile_h_
#define iw_bfile_h_

#define _CRT_SECURE_NO_DEPRECATE

#include <string>

#include "iw_typedef.h"
#include "iw_cast.h"
#include "iw_Array.h"

#pragma warning(push)
#pragma warning(disable:4996)

namespace iw
{
	/*
	 * filepathに指定したファイルを削除する
	 */
	bool delfile(const char * filepath);
	bool delfile(const std::string & filepath);

	/*
	 * バイナリファイルの読み書きを司る
	 *
	 * 明示的にファイルをオープンしなくても、必要になったら暗黙的にファイルがオープンされる
	 * open_to_read, open_to_writeを使って明示的にオープンすることも可能
	 * 
	 * 明示的にファイルをクローズしなくても、デストラクタによって暗黙的にファイルがクローズされる
	 * closeを使って明示的にクローズすることも可能
	 *
	 */
	class Binfile
	{
	public:
		Binfile();
		Binfile(const char * path_);			// インスタンス化時にファイルパスを指定することができる。
		Binfile(const std::string & path_);	// インスタンス化時にファイルパスを指定することができる。

		// path ********************************************************************************
		/*
		 * インスタンス化後もファイルパスを指定したり変更したりできるが変更する場合は注意が必要
		 * path関数を呼んだ時点で、現在開かれているファイルはクローズされる
		 * 読み出し位置も先頭に戻る
		 *
		 */

		Binfile & path(const char * path_);
		Binfile & path(const std::string & path_);
		// *************************************************************************************

		// (あらかじめ読み込んでおいた)ファイルの内容を、指定された変数にコピーし、そのバイト数分読み出し位置を進める
		template<typename T>
		Binfile & operator>> (T & req)
		{
			divide(req);
			return *this;
		}

		// 現在の読み出し位置が、文字列配列(ヌルで終わる)の先頭アドレスだと仮定し、
		// そのアドレスを返して文字列の次のデータまで読み出し位置をずらす
		const char * as_string();

		const char * head();
		uint byte() const;
		Binfile & clear();	// ファイルの内容を空にする

		// ファイルに書き込む
		template<typename T>
		const Binfile & operator<< (const T & req) const
		{
			write(req);
			return *this;
		}

		Binfile & move(int distance);		// 指定したバイト数分、読み出し位置をずらす
		Binfile & position(uint point);	// 読み出し位置(バイト単位)を直接指定する
		uint position() const;			// 現在の読み出し位置(バイト単位)を取得する

		Binfile & open_to_read();		// 読み込み用として明示的にファイルを開く
		Binfile & open_to_write();	// 書き込み用として明示的にファイルを開く
		void close();				// 明示的にファイルを閉じる

		// divide ***********************************************************************
		/*
		 * (あらかじめ読み込んでおいた)ファイルの内容を、
		 * 指定された変数にコピーし、
		 * そのバイト数分読み出し位置を進める
		 *
		 */

		Binfile & divide(void * req, uint size);
		Binfile & divide(std::string & req);

		template<typename T>
		Binfile & divide(iw::Array<T> & req)
		{
			req.zerosize();
			uint byte(0);
			*this >> byte;
			req.requre(byte);
			for (uint i = 0; i < byte; ++i) *this >> req.next();
			return *this;
		}

		template<typename T>
		Binfile & divide(T & req)
		{
			copy(req);
			move(sizeof(T));
			return *this;
		}
		// ******************************************************************************

		// copy *************************************************************************
		/*
		* (あらかじめ読み込んでおいた)ファイルの内容を、
		* 指定された変数にコピーする。
		* divideとは違い、読み出し位置は変化しない
		*
		*/


		Binfile & copy(void * req, uint size);
		Binfile & copy(std::string & req);

		template<typename T>
		Binfile & copy(iw::Array<T> & req)
		{
			req.zerosize();
			uint byte(0);
			*this >> byte;
			req.requre(byte);
			for (uint i = 0; i < byte; ++i) *this >> req.next();
			move(-(sizeof(uint) + sizeof(T)*byte));
			return *this;
		}

		template<typename T>
		Binfile & copy(T & req)
		{
			open_if_need(readingmode);
			req = *reinterpret_cast<const T *>(imutable_content().address(position()));
			return *this;
		}

		// ******************************************************************************

		// write ************************************************************************

		const Binfile & write(const void * req, uint size) const;

		template<typename T>
		const Binfile & write(const iw::Array<T> & req) const
		{
			*this << req.size();
			for (uint i = 0; i < req.size(); ++i) *this << req[i];
			return *this;
		}

		const Binfile & write(const std::string & req) const;

		template<typename T>
		const Binfile & write(const T & req) const
		{
			write(&req, sizeof(T));
			return *this;
		}

		// ******************************************************************************

		bool finished() const;



		~Binfile();
	private:
		enum
		{
			readingmode,
			writingmode,
			addingmode
		};

		mutable iw::Array<uchar> content_;
		uint position_;
		std::string path_;
		mutable FILE * fp;

		void construct();
		iw::Array<uchar> & mutable_content();
		const iw::Array<uchar> & imutable_content() const;
		void open_if_need(uint mode) const;
		void open(uint mode) const;
		FILE * get_fp() const;
	};










}

#pragma warning(pop)


#endif