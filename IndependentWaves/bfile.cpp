#include <cstdio>

#include "iw_bfile.h"
#include "iw_filesize.h"

#pragma warning(push)
#pragma warning(disable:4996)

namespace iw
{
	bool delfile(const char * filepath)
	{
		return remove(filepath) == 0;
	}
	bool delfile(const std::string & filepath)
	{
		return delfile(filepath.c_str());
	}




	/*
	 * std::stringの扱いについて。
	 * 文字列情報のバイト数などは保存せず、ヌル文字で終了する文字列をそのまま保存することで統一
	 *
	 */



	Binfile::Binfile()
	{
		construct();
	}
	Binfile::Binfile(const char * path_)			// インスタンス化時にファイルパスを指定することができる。
	{
		construct();
		path(path_);
	}
	Binfile::Binfile(const std::string & path_)	// インスタンス化時にファイルパスを指定することができる。
	{
		construct();
		path(path_);
	}

	// path ********************************************************************************
	/*
	* インスタンス化後もファイルパスを指定したり変更したりできるが変更する場合は注意が必要
	* path関数を呼んだ時点で、現在開かれているファイルはクローズされる
	* 読み出し位置も先頭に戻る
	*
	*/

	Binfile & Binfile::path(const char * path_)
	{
		this->path_ = path_;
		close();
		position(0);
		return *this;
	}
	Binfile & Binfile::path(const std::string & path_){ return path(path_.c_str()); }
	// *************************************************************************************


	const char * Binfile::as_string()
	{
		open_if_need(readingmode);
		const char * pszStr = reinterpret_cast<const char *>(content_.head() + position());
		move(strlen(pszStr) + 1);
		return pszStr;
	}

	const char * Binfile::head()
	{
		open_if_need(readingmode);
		const char * pszHead = reinterpret_cast<const char *>(content_.head());
		return pszHead;
	}
	uint Binfile::byte() const
	{
		open_if_need(readingmode);
		return content_.size();
	}


	Binfile & Binfile::clear()	// ファイルの内容を空にする
	{
		close();
		open(writingmode);
		return *this;
	}

	Binfile & Binfile::move(int distance)		// 指定したバイト数分、読み出し位置をずらす
	{
		position(static_cast<uint>(static_cast<int>(position()) + distance));
		return *this;
	}
	Binfile & Binfile::position(uint point)	// 読み出し位置(バイト単位)を直接指定する
	{
		position_ = point;
		return *this;
	}
	uint Binfile::position() const			// 現在の読み出し位置(バイト単位)を取得する
	{
		return position_;
	}

	Binfile & Binfile::open_to_read()		// 読み込み用として明示的にファイルを開く
	{
		close();
		open(readingmode);
		return *this;
	}
	Binfile & Binfile::open_to_write()	// 追加書き込み用として明示的にファイルを開く
	{
		close();
		open(addingmode);
		return *this;
	}

	void Binfile::close()				// 明示的にファイルを閉じる
	{
		if (fp != nullptr)
		{
			fclose(fp);
		}

		fp = nullptr;
	}

	// divide ***********************************************************************
	/*
	* (あらかじめ読み込んでおいた)ファイルの内容を、
	* 指定された変数にコピーし、
	* そのバイト数分読み出し位置を進める
	*
	*/

	Binfile & Binfile::divide(void * req, uint size)
	{
		copy(req, size);
		move(size);
		return *this;
	}
	Binfile & Binfile::divide(std::string & req)
	{
		copy(req);
		move(req.length() + 1);	// ヌル文字分余計に移動させる
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


	Binfile & Binfile::copy(void * req, uint size)
	{
		memcpy(req, imutable_content().address(position()), size);
		return *this;
	}

	Binfile & Binfile::copy(std::string & req)
	{
		req = reinterpret_cast<const char *>(imutable_content().address(position()));
		return *this;
	}


	// ******************************************************************************

	// write ************************************************************************

	const Binfile & Binfile::write(const void * req, uint size) const
	{
		fwrite(req, size, 1, get_fp());
		return *this;
	}


	const Binfile & Binfile::write(const std::string & req) const
	{
		if (req.empty() == false)
		{
			write(req.c_str(), req.length() + 1);
		}
		else
		{
			unsigned char null_str = '\0';
			write(null_str);
		}

		return *this;
	}


	// ******************************************************************************


	bool Binfile::finished() const
	{
		open_if_need(readingmode);
		return position() >= byte();
	}


	Binfile::~Binfile()
	{
		close();
	}


	void Binfile::construct()
	{
		fp = nullptr;
		content_.zerosize();
		position(0);
	}

	iw::Array<uchar> & Binfile::mutable_content()
	{
		open_if_need(readingmode);
		return content_;
	}
	const iw::Array<uchar> & Binfile::imutable_content() const
	{
		open_if_need(readingmode);
		return content_;
	}

	void Binfile::open_if_need(uint mode) const
	{
		if (mode == readingmode)
		{
			if (content_.size()==0)
			{
				open(readingmode);
			}
		}
		else
		{
			if (fp == nullptr)
			{
				open(mode);
			}
		}
	}
	void Binfile::open(uint mode) const
	{
		if (mode == readingmode)
		{
			FILE * rfp = fopen(path_.c_str(), "rb");
			content_.setsize(filesize(path_.c_str()));
			fread(content_.head(), content_.size(), 1, rfp);
			if (rfp)
			{
				fclose(rfp);
			}
		}

		if (mode == addingmode)
		{
			fp = fopen(path_.c_str(), "ab");
		}

		if (mode == writingmode)
		{
			fp = fopen(path_.c_str(), "wb");
		}
	}

	FILE * Binfile::get_fp() const
	{
		open_if_need(addingmode);
		return fp;
	}

}

#pragma warning(pop)