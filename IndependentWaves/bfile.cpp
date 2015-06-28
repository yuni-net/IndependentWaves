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
	 * std::string�̈����ɂ��āB
	 * ��������̃o�C�g���Ȃǂ͕ۑ������A�k�������ŏI�����镶��������̂܂ܕۑ����邱�Ƃœ���
	 *
	 */



	Binfile::Binfile()
	{
		construct();
	}
	Binfile::Binfile(const char * path_)			// �C���X�^���X�����Ƀt�@�C���p�X���w�肷�邱�Ƃ��ł���B
	{
		construct();
		path(path_);
	}
	Binfile::Binfile(const std::string & path_)	// �C���X�^���X�����Ƀt�@�C���p�X���w�肷�邱�Ƃ��ł���B
	{
		construct();
		path(path_);
	}

	// path ********************************************************************************
	/*
	* �C���X�^���X������t�@�C���p�X���w�肵����ύX������ł��邪�ύX����ꍇ�͒��ӂ��K�v
	* path�֐����Ă񂾎��_�ŁA���݊J����Ă���t�@�C���̓N���[�Y�����
	* �ǂݏo���ʒu���擪�ɖ߂�
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


	Binfile & Binfile::clear()	// �t�@�C���̓��e����ɂ���
	{
		close();
		open(writingmode);
		return *this;
	}

	Binfile & Binfile::move(int distance)		// �w�肵���o�C�g�����A�ǂݏo���ʒu�����炷
	{
		position(static_cast<uint>(static_cast<int>(position()) + distance));
		return *this;
	}
	Binfile & Binfile::position(uint point)	// �ǂݏo���ʒu(�o�C�g�P��)�𒼐ڎw�肷��
	{
		position_ = point;
		return *this;
	}
	uint Binfile::position() const			// ���݂̓ǂݏo���ʒu(�o�C�g�P��)���擾����
	{
		return position_;
	}

	Binfile & Binfile::open_to_read()		// �ǂݍ��ݗp�Ƃ��Ė����I�Ƀt�@�C�����J��
	{
		close();
		open(readingmode);
		return *this;
	}
	Binfile & Binfile::open_to_write()	// �ǉ��������ݗp�Ƃ��Ė����I�Ƀt�@�C�����J��
	{
		close();
		open(addingmode);
		return *this;
	}

	void Binfile::close()				// �����I�Ƀt�@�C�������
	{
		if (fp != nullptr)
		{
			fclose(fp);
		}

		fp = nullptr;
	}

	// divide ***********************************************************************
	/*
	* (���炩���ߓǂݍ���ł�����)�t�@�C���̓��e���A
	* �w�肳�ꂽ�ϐ��ɃR�s�[���A
	* ���̃o�C�g�����ǂݏo���ʒu��i�߂�
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
		move(req.length() + 1);	// �k���������]�v�Ɉړ�������
		return *this;
	}

	// ******************************************************************************

	// copy *************************************************************************
	/*
	* (���炩���ߓǂݍ���ł�����)�t�@�C���̓��e���A
	* �w�肳�ꂽ�ϐ��ɃR�s�[����B
	* divide�Ƃ͈Ⴂ�A�ǂݏo���ʒu�͕ω����Ȃ�
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