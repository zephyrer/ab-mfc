
namespace SIE
{
	class Config
	{
	public:
		enum UserID
		{
			UI_ANONYMOUS = 0,  // ����
			UI_CGB       = 1,  // �޹��
			UI_GCK       = 2,  // �߳���
			UI_ZH        = 3,  // ֣��
			UI_MAX
		};

	
		// ��ʼ�����������ļ�
		static void Init();
		// ��ȡ��ǰUserID
		static const int GetUserID() 
		{
			return iUserID;
		}
	private:
		static const char * strUserFileName;
		static const char * strUserName[UI_MAX];
		static int iUserID;
	private:
		Config();
	};

};