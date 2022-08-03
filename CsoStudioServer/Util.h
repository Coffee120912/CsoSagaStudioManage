#pragma once
#include "QtConnect.h"
#include "nlohmann/json.hpp"
#include "nlohmann/fifo_map.hpp"
#include <fstream>

template<class K, class V, class dummy_compare, class A>
using my_workaround_fifo_map = nlohmann::fifo_map<K, V, nlohmann::fifo_map_compare<K>, A>;
using my_json = nlohmann::basic_json<my_workaround_fifo_map>;

typedef struct TAG_LAUNCHER_STRING
{
	int		Size;
	char	Name[MAX_PATH];
	TAG_LAUNCHER_STRING(const char* Value) : Size(0)
	{
		Size = wsprintfA(Name, "%s", Value);
	}
}*PTAG_LAUNCHER_STRING;

typedef struct TAG_LAUNCHER_PARAM
{
	TAG_LAUNCHER_STRING	Name;
	TAG_LAUNCHER_STRING	Value;
}*PTAG_LAUNCHER_PARAM;



class Util
{
public:
	// @关闭CSO共享句柄
	void CloseCsoSharedDict();

	// @设置游戏服务器信息
	bool SetGameRegion(ULONG Region);

	// @通过索引返回大区信息
	std::string GetServerRegionByIndexToStr(ULONG ulRegion);
	
	// @返回账号状态
	PTAG_GAME_USER_STATUS GetUserStatusByIndex(USER_STATUS Index);

	// @返回任务说明
	PTAG_GAME_TASK_SRC GetTaskByIndex(TASK_MODE Mode);
	
	PTAG_GAME_TASK_SRC GetTaskByName(std::string TaskName);
	/*
	*	@道具名返回道具ID
	*	@return:找到返回道具ID,未找到返回0 错误返回-1
	*/
	int GetItemIDByName(std::string ItemName);

	// @道具ID返回道具名
	std::string GetItemNameByID(int ItemID);

	// @初始化
	void JsonInit();

	// @获取彩漆
	std::map<ULONG, std::string>& GetPaintMap();




private:
	void SetValue(LPVOID Info, int& Index, int Size, ULONG* Value);
	my_json	m_ItemJson;
	my_json	m_Paintson;
	std::map<ULONG, std::string> m_PaintMap;
};

extern Util g_Util;