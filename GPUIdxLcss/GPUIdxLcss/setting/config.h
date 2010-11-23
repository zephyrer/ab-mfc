
namespace SIE
{
	class Config
	{
	public:
		enum UserID
		{
			UI_ANONYMOUS = 0,  // 匿名
			UI_CGB       = 1,  // 崔广斌
			UI_GCK       = 2,  // 高成锴
			UI_ZH        = 3,  // 郑虎
			UI_MAX
		};

	
		// 初始化，读配置文件
		static void Init();
		// 获取当前UserID
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