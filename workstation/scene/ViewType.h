#pragma once
#include <QVariant>
//�����ӿ�����
enum   ViewType
{
	UNKOWN=-1,//δ���ӿ�
	VOL_M,//��ά-������  
	TRA_M,//�����-������   ������ն��������
	COR_M,//��״��-������   ���������п�����ǰ������
	SAG_M,//ʸ״��-������   ���˴��м����������������
	//VOL_D,//��ά-����
	//TRA_D,//�����-����
	//COR_D,//��״��-����
	//SAG_D,//ʸ״��-����
	TUN_D//ͨ���ӿ�-����
};

Q_DECLARE_METATYPE(ViewType)

typedef enum {
	LAYOUT_NONE = -1, LAYOUT_TCS = 0, LAYOUT_TCSV
} Layout;