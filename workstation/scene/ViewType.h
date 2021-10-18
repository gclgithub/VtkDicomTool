#pragma once
#include <QVariant>
//定义视口类型
enum   ViewType
{
	UNKOWN=-1,//未有视口
	VOL_M,//三维-主窗口  
	TRA_M,//横断面-主窗口   把人腰斩，看断面
	COR_M,//冠状面-主窗口   把人左右切开，从前面来看
	SAG_M,//矢状面-主窗口   把人从中间劈开，看侧面断面
	//VOL_D,//三维-副窗
	//TRA_D,//横断面-副窗
	//COR_D,//冠状面-副窗
	//SAG_D,//矢状面-副窗
	TUN_D//通道视口-副窗
};

Q_DECLARE_METATYPE(ViewType)

typedef enum {
	LAYOUT_NONE = -1, LAYOUT_TCS = 0, LAYOUT_TCSV
} Layout;