#ifndef iw_bfile_h_
#define iw_bfile_h_

#define _CRT_SECURE_NO_DEPRECATE

#include "typedef.h"
#include "cast.h"
#include "vector.h"
#include <string>

#pragma warning(push)
#pragma warning(disable:4996)

namespace iw
{

	bool delfile(const char * filepath);
	bool delfile(const std::string & filepath);

	/*
	 * �o�C�i���t�@�C���̓ǂݏ������i��
	 *
	 * �����I�Ƀt�@�C�����I�[�v�����Ȃ��Ă��A�K�v�ɂȂ�����ÖٓI�Ƀt�@�C�����I�[�v�������
	 * open_to_read, open_to_write���g���Ė����I�ɃI�[�v�����邱�Ƃ��\
	 * 
	 * �����I�Ƀt�@�C�����N���[�Y���Ȃ��Ă��A�f�X�g���N�^�ɂ���ĈÖٓI�Ƀt�@�C�����N���[�Y�����
	 * close���g���Ė����I�ɃN���[�Y���邱�Ƃ��\
	 *
	 */
	class bfile
	{
	public:
		bfile();
		bfile(const char * path_);			// �C���X�^���X�����Ƀt�@�C���p�X���w�肷�邱�Ƃ��ł���B
		bfile(const std::string & path_);	// �C���X�^���X�����Ƀt�@�C���p�X���w�肷�邱�Ƃ��ł���B

		// path ********************************************************************************
		/*
		 * �C���X�^���X������t�@�C���p�X���w�肵����ύX������ł��邪�ύX����ꍇ�͒��ӂ��K�v
		 * path�֐����Ă񂾎��_�ŁA���݊J����Ă���t�@�C���̓N���[�Y�����
		 * �ǂݏo���ʒu���擪�ɖ߂�
		 *
		 */

		bfile & path(const char * path_);
		bfile & path(const std::string & path_);
		// *************************************************************************************

		// (���炩���ߓǂݍ���ł�����)�t�@�C���̓��e���A�w�肳�ꂽ�ϐ��ɃR�s�[���A���̃o�C�g�����ǂݏo���ʒu��i�߂�
		template<typename T>
		bfile & operator>> (T & req)
		{
			divide(req);
			return *this;
		}

		// ���݂̓ǂݏo���ʒu���A������z��(�k���ŏI���)�̐擪�A�h���X���Ɖ��肵�A
		// ���̃A�h���X��Ԃ��ĕ�����̎��̃f�[�^�܂œǂݏo���ʒu�����炷
		const char * as_string();

		const char * head();
		uint byte() const;
		bfile & clear();	// �t�@�C���̓��e����ɂ���

		// �t�@�C���ɏ�������
		template<typename T>
		const bfile & operator<< (const T & req) const
		{
			write(req);
			return *this;
		}

		bfile & move(int distance);		// �w�肵���o�C�g�����A�ǂݏo���ʒu�����炷
		bfile & position(uint point);	// �ǂݏo���ʒu(�o�C�g�P��)�𒼐ڎw�肷��
		uint position() const;			// ���݂̓ǂݏo���ʒu(�o�C�g�P��)���擾����

		bfile & open_to_read();		// �ǂݍ��ݗp�Ƃ��Ė����I�Ƀt�@�C�����J��
		bfile & open_to_write();	// �������ݗp�Ƃ��Ė����I�Ƀt�@�C�����J��
		void close();				// �����I�Ƀt�@�C�������

		// divide ***********************************************************************
		/*
		 * (���炩���ߓǂݍ���ł�����)�t�@�C���̓��e���A
		 * �w�肳�ꂽ�ϐ��ɃR�s�[���A
		 * ���̃o�C�g�����ǂݏo���ʒu��i�߂�
		 *
		 */

		bfile & divide(void * req, uint size);
		bfile & divide(std::string & req);

		template<typename T>
		bfile & divide(iw::vector<T> & req)
		{
			req.zerosize();
			uint byte(0);
			*this >> byte;
			req.requre(byte);
			for (uint i = 0; i < byte; ++i) *this >> req.next();
			return *this;
		}

		template<typename T>
		bfile & divide(T & req)
		{
			copy(req);
			move(sizeof(T));
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


		bfile & copy(void * req, uint size);
		bfile & copy(std::string & req);

		template<typename T>
		bfile & copy(iw::vector<T> & req)
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
		bfile & copy(T & req)
		{
			open_if_need(readingmode);
			req = *reinterpret_cast<const T *>(imutable_content().address(position()));
			return *this;
		}

		// ******************************************************************************

		// write ************************************************************************

		const bfile & write(const void * req, uint size) const;

		template<typename T>
		const bfile & write(const iw::vector<T> & req) const
		{
			*this << req.size();
			for (uint i = 0; i < req.size(); ++i) *this << req[i];
			return *this;
		}

		const bfile & write(const std::string & req) const;

		template<typename T>
		const bfile & write(const T & req) const
		{
			write(&req, sizeof(T));
			return *this;
		}

		// ******************************************************************************

		bool finished() const;



		~bfile();
	private:
		enum
		{
			readingmode,
			writingmode,
			addingmode
		};

		mutable iw::vector<uchar> content_;
		uint position_;
		std::string path_;
		mutable FILE * fp;

		void construct();
		iw::vector<uchar> & mutable_content();
		const iw::vector<uchar> & imutable_content() const;
		void open_if_need(uint mode) const;
		void open(uint mode) const;
		FILE * get_fp() const;
	};










}

#pragma warning(pop)


#endif