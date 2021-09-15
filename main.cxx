#include <QApplication>
#include <QIcon>
#include <QDir>	
#include <Windows.h>
#include <QTextCodec>
#include <sstream>
#include <io.h>
#include <fcntl.h>
#include "mainWindow.h"

#include <vtkAutoInit.h>
#include <vtkObject.h>

#include "json.hpp"
using json = nlohmann::json;

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);

std::string GBKToUTF8(const char* strGBK)
{
	int len = MultiByteToWideChar(CP_ACP, 0, strGBK, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, strGBK, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	std::string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
}
std::string UTF8ToGBK(const char* strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	std::string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}

void get_value(double* va) 
{
	va[0] = 1;
	va[1] = 2;
	va[2] = 3;
	va[3] = 0;
}


int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	app.setAttribute(Qt::AA_UseHighDpiPixmaps);
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	app.setWindowIcon(QIcon("images/app_icon.png"));

	if (true)
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
		int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
		FILE* hf_out = _fdopen(hCrt, "w");

		setvbuf(hf_out, NULL, _IONBF, 1);
		*stdout = *hf_out;

		freopen("CONIN$", "r", stdin);//necessary
		HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
		hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
		FILE* hf_in = _fdopen(hCrt, "r");
		setvbuf(hf_in, NULL, _IONBF, 128);
		*stdin = *hf_in;
	}

	mianWindow win;
	win.initiate();
	win.show();

	return app.exec();
}
