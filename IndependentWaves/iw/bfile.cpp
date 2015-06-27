#include "bfile.h"
#include <cstdio>
#include "filesize.h"

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



	bfile::bfile()
	{
		construct();
	}
	bfile::bfile(const char * path_)			// �C���X�^���X�����Ƀt�@�C���p�X���w�肷�邱�Ƃ��ł���B
	{
		construct();
		path(path_);
	}
	bfile::bfile(const std::string & path_)	// �C���X�^���X�����Ƀt�@�C���p�X���w�肷�邱�Ƃ��ł���B
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

	bfile & bfile::path(const char * path_)
	{
		this->path_ = path_;
		close();
		position(0);
		return *this;
	}
	bfile & bfile::path(const std::string & path_){ return path(path_.c_str()); }
	// *************************************************************************************


	const char * bfile::as_string()
	{
		open_if_need(readingmode);
		const char * pszStr = reinterpret_cast<const char *>(content_.head() + position());
		move(strlen(pszStr) + 1);
		return pszStr;
	}

	const char * bfile::head()
	{
		open_if_need(readingmode);
		const char * pszHead = reinterpret_cast<const char *>(content_.head());
		return pszHead;
	}
	uint bfile::byte() const
	{
		open_if_need(readingmode);
		return content_.size();
	}


	bfile & bfile::clear()	// �t�@�C���̓��e����ɂ���
	{
		close();
		open(writingmode);
		return *this;
	}

	bfile & bfile::move(int distance)		// �w�肵���o�C�g�����A�ǂݏo���ʒu�����炷
	{
		position(static_cast<uint>(static_cast<int>(position()) + distance));
		return *this;
	}
	bfile & bfile::position(uint point)	// �ǂݏo���ʒu(�o�C�g�P��)�𒼐ڎw�肷��
	{
		position_ = point;
		return *this;
	}
	uint bfile::position() const			// ���݂̓ǂݏo���ʒu(�o�C�g�P��)���擾����
	{
		return position_;
	}

	bfile & bfile::open_to_read()		// �ǂݍ��ݗp�Ƃ��Ė����I�Ƀt�@�C�����J��
	{
		close();
		open(readingmode);
		return *this;
	}
	bfile & bfile::open_to_write()	// �ǉ��������ݗp�Ƃ��Ė����I�Ƀt�@�C�����J��
	{
		close();
		open(addingmode);
		return *this;
	}

	void bfile::close()				// �����I�Ƀt�@�C�������
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

	bfile & bfile::divide(void * req, uint size)
	{
		copy(req, size);
		move(size);
		return *this;
	}
	bfile & bfile::divide(std::string & req)
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


	bfile & bfile::copy(void * req, uint size)
	{
		memcpy(req, imutable_content().address(position()), size);
		return *this;
	}

	bfile & bfile::copy(std::string & req)
	{
		req = reinterpret_cast<const char *>(imutable_content().address(position()));
		return *this;
	}


	// ******************************************************************************

	// write ************************************************************************

	const bfile & bfile::write(const void * req, uint size) const
	{
		fwrite(req, size, 1, get_fp());
		return *this;
	}


	const bfile & bfile::write(const std::string & req) const
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


	bool bfile::finished() const
	{
		open_if_need(readingmode);
		return position() >= byte();
	}


	bfile::~bfile()
	{
		close();
	}


	void bfile::construct()
	{
		fp = nullptr;
		content_.zerosize();
		position(0);
	}

	iw::vector<uchar> & bfile::mutable_content()
	{
		open_if_need(readingmode);
		return content_;
	}
	const iw::vector<uchar> & bfile::imutable_content() const
	{
		open_if_need(readingmode);
		return content_;
	}

	void bfile::open_if_need(uint mode) const
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
	void bfile::open(uint mode) const
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

	FILE * bfile::get_fp() const
	{
		open_if_need(addingmode);
		return fp;
	}

}