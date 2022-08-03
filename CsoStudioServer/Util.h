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
	// @�ر�CSO������
	void CloseCsoSharedDict();

	// @������Ϸ��������Ϣ
	bool SetGameRegion(ULONG Region);

	// @ͨ���������ش�����Ϣ
	std::string GetServerRegionByIndexToStr(ULONG ulRegion);
	
	// @�����˺�״̬
	PTAG_GAME_USER_STATUS GetUserStatusByIndex(USER_STATUS Index);

	// @��������˵��
	PTAG_GAME_TASK_SRC GetTaskByIndex(TASK_MODE Mode);
	
	PTAG_GAME_TASK_SRC GetTaskByName(std::string TaskName);
	/*
	*	@���������ص���ID
	*	@return:�ҵ����ص���ID,δ�ҵ�����0 ���󷵻�-1
	*/
	int GetItemIDByName(std::string ItemName);

	// @����ID���ص�����
	std::string GetItemNameByID(int ItemID);

	// @��ʼ��
	void JsonInit();

	// @��ȡ����
	std::map<ULONG, std::string>& GetPaintMap();




private:
	void SetValue(LPVOID Info, int& Index, int Size, ULONG* Value);
	my_json	m_ItemJson;
	my_json	m_Paintson;
	std::map<ULONG, std::string> m_PaintMap;
};

extern Util g_Util;