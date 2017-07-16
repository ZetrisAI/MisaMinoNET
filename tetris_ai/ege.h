/*********************************************************
* EGE (Easy Graphics Engine)
* FileName      ege.h
* HomePage1     http://misakamm.github.com/xege
* HomePage2     http://misakamm.bitbucket.org/index.htm
* HomePage3     http://tcgraphics.sourceforge.net
* teiba1        http://tieba.baidu.com/f?kw=ege
* teiba2        http://tieba.baidu.com/f?kw=ege%C4%EF
* Blog:         http://misakamm.com
* E-Mail:       mailto:misakamm[at gmail com]
*
* FileName: ege.h
* �� VC ��ģ�� Borland BGI ��ͼ�⣬ʵ�ּ򵥵Ļ�ͼ֮�࣬��չ�˽ϸ��ӵĻ�ͼ����
*
* ������ʹ�ñ���ʱ����Ҫ����conio.hͷ�ļ�
* ��Щͷ�ļ���Ӧ���棬������ܻ�������
* ����getch��conio.h�ڵĸ��ǣ��ɰ����������������ע��
* ���蹲�棬��ʹ�ö��ļ��ֿ�������ģʽʹ�ã�
* ������һ��cppͬʱ�����������Էֿ�����
* ʹ�ñ��⣬������C++���룬��֧�ֵı�������
* VC6/VC2008/VC2010/MinGW3.4.5/MinGW4.4.1
*********************************************************/

/****************************************************************************
** ע�����
* �������Ҫ��ʾ����̨���ڣ����ڰ������ļ���ǰ���һ��define SHOW_CONSOLE
* �����Sleep���APIʱ�����ߵ���delay��ʵ�ʾ���ת��Ϊ����delay_ms����������API��ʹ��api_sleep
* ��delay_ms(0)�������ж���û�и��µı�Ҫ��������ε��������ĵ��ò��������֡�ʵ�Ӱ��
* �����delay_ms, delay_fps, getch, getkey, getmouse ʱ���������ݿ��ܻ���£���Щ�����൱��������delay_ms(0)��
*   �����ֻ��Ҫ���´��ڣ�������ȴ���������delay_ms(0)��ע��delayֻ��ʱ�������´���
* ������ʹ��delay_ms/delay_fps���������Լ�����ĳ���ռ�õ�CPU������һ����û�е���ͬʱҲû��getch/getmouse�Ļ�������ռ��һ��CPU��ʱ��
****************************************************************************/

#ifndef _EGE_H_
#define _EGE_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#ifdef _GRAPH_LIB_BUILD_
#ifndef WINVER
#define WINVER 0x0400           // Specifies that the minimum required platform is Windows 95/NT4.
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400     // Specifies that the minimum required platform is Windows 95/NT4.
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410   // Specifies that the minimum required platform is Windows 98.
#endif
#endif

#ifndef __cplusplus
#error You must use C++ compiler, or you need filename with '.cpp' suffix
#endif

#if defined(_INC_CONIO) || defined(_CONIO_H_)
#error can not include "conio.h" before "graphics.h"
#endif

#if defined(_MSC_VER)
#pragma warning(disable: 4355)
#ifndef _ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH
#define _ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH
#endif
#ifndef _ALLOW_RUNTIME_LIBRARY_MISMATCH
#define _ALLOW_RUNTIME_LIBRARY_MISMATCH
#endif
#endif

#if !defined(_GRAPH_LIB_BUILD_) && !defined(_GRAPH_NO_LIB_)
	#ifdef _MSC_VER
		#ifdef _WIN64
			#if (_MSC_VER >= 1700)
				#if 0 && defined(_DLL)
					#pragma comment(lib,"graphics1264d.lib")
				#else
					#pragma comment(lib,"graphics1264.lib")
				#endif
			#elif (_MSC_VER >= 1600)
				#if 0 && defined(_DLL)
					#pragma comment(lib,"graphics1064d.lib")
				#else
					#pragma comment(lib,"graphics1064.lib")
				#endif
			#elif (_MSC_VER >= 1500)
				#if 0 && defined(_DLL)
					#pragma comment(lib,"graphics0864d.lib")
				#else
					#pragma comment(lib,"graphics0864.lib")
				#endif
			#elif (_MSC_VER > 1200)
				#pragma comment(lib,"graphics05.lib")
			#else
				#pragma comment(lib,"graphics.lib")
			#endif
		#else
			#if (_MSC_VER >= 1700)
				#if 0 && defined(_DLL)
					#pragma comment(lib,"graphics12d.lib")
				#else
					#pragma comment(lib,"graphics12.lib")
				#endif
			#elif (_MSC_VER >= 1600)
				#if 0 && defined(_DLL)
					#pragma comment(lib,"graphics10d.lib")
				#else
					#pragma comment(lib,"graphics10.lib")
				#endif
			#elif (_MSC_VER >= 1500)
				#if 0 && defined(_DLL)
					#pragma comment(lib,"graphics08d.lib")
				#else
					#pragma comment(lib,"graphics08.lib")
				#endif
			#elif (_MSC_VER > 1200)
				#pragma comment(lib,"graphics05.lib")
			#else
				#pragma comment(lib,"graphics.lib")
			#endif
		#endif
		#if _MSC_VER >= 1700
			#ifdef _DEBUG
			#else
			#endif
		#elif _MSC_VER >= 1600
			#ifdef _DEBUG
			#else
			#endif
		#elif _MSC_VER > 1200
			#ifdef _DEBUG
				//#pragma comment(linker, "/NODEFAULTLIB:MSVCRTD.lib")
				//#pragma comment(linker, "/NODEFAULTLIB:libcmtd.lib")
				//#pragma comment(linker, "/NODEFAULTLIB:libcpmtd.lib")
			#else
			#endif
		#else
			#ifdef _DEBUG
				//#pragma comment(linker, "/NODEFAULTLIB:MSVCRTD.lib")
				//#pragma comment(linker, "/NODEFAULTLIB:libcd.lib")
				//#pragma comment(linker, "/NODEFAULTLIB:libcmtd.lib")
				//#pragma comment(linker, "/NODEFAULTLIB:libcpmtd.lib")
			#else
				//#pragma comment(linker, "/NODEFAULTLIB:MSVCRT.lib")
				//#pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
				//#pragma comment(linker, "/NODEFAULTLIB:libcp.lib")
			#endif
		#endif
	#endif
#endif

#if !defined(_GRAPH_LIB_BUILD_) && !defined(_GRAPH_NO_LIB_)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifndef _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif

#ifndef _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif
#endif

#if defined(_MSC_VER) && _MSC_VER <= 1200 && !defined(SetWindowLongPtr)
	#define SetWindowLongPtrW   SetWindowLongW
	#define GetWindowLongPtrW   GetWindowLongW
	#define GWLP_USERDATA       GWL_USERDATA
	#define GWLP_WNDPROC        GWL_WNDPROC
#endif

#if !defined(_GRAPH_LIB_BUILD_) && !defined(_GRAPH_NO_LIB_)
#if defined(_MSC_VER) && _MSC_VER > 1200
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' ""version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL                   0x020A
#endif

#ifdef _GRAPH_LIB_BUILD_
#include <cstdio>
#endif

#ifndef _Cdecl
#if __STDC__
#define _Cdecl  __cdecl
#else
#define _Cdecl  __cdecl
#endif
#endif

#ifdef _MSC_VER
	#if defined(_WIN64)
		#define EGEAPI
	#else
		#define EGEAPI _Cdecl
	#endif
#else
	#if defined(__WORDSIZE)
		#if __WORDSIZE > 32
			#define EGEAPI
		#else
			#define EGEAPI _Cdecl
		#endif
	#else
		#define EGEAPI
	#endif
#endif

#ifdef _GRAPH_LIB_BUILD_
	#define EGE_DEPRECATE(text)
#else
	#ifdef _MSC_VER
		#ifdef _CRT_DEPRECATE_TEXT
			#define EGE_DEPRECATE(text) _CRT_DEPRECATE_TEXT("This function is deprecated, more info visit http://tcgraphics.sourceforge.net/")
		#else
			#define EGE_DEPRECATE(text)
		#endif
	#else
		#define EGE_DEPRECATE(text) __attribute__((deprecated))
	#endif
#endif

#define SHOWCONSOLE             1       // ����ͼ��ģʽʱ����������̨����ʾ
#define RGBTOBGR(color)         ((((color) & 0xFF) << 16) | (((color) & 0xFF0000) >> 16) | ((color) & 0xFF00FF00))
#define EGERGB(r, g, b)         ( ((r)<<16) | ((g)<<8) | (b))
#define EGERGBA(r, g, b, a)     ( ((r)<<16) | ((g)<<8) | (b) | ((a)<<24) )
#define EGEARGB(a, r, g, b)     ( ((r)<<16) | ((g)<<8) | (b) | ((a)<<24) )
#define EGEACOLOR(a, color)     ( ((color) & 0xFFFFFF) | ((a)<<24) )
#define EGECOLORA(color, a)     ( ((color) & 0xFFFFFF) | ((a)<<24) )
#define EGEGET_R(c)             ( ((c)>>16) & 0xFF )
#define EGEGET_G(c)             ( ((c)>> 8) & 0xFF )
#define EGEGET_B(c)             ( ((c)) & 0xFF )
#define EGEGET_A(c)             ( ((c)>>24) & 0xFF )
#define EGEGRAY(gray)           ( ((gray)<<16) | ((gray)<<8) | (gray))
#define EGEGRAYA(gray, a)       ( ((gray)<<16) | ((gray)<<8) | (gray) | ((a)<<24) )
#define EGEAGRAY(a, gray)       ( ((gray)<<16) | ((gray)<<8) | (gray) | ((a)<<24) )

namespace ege {

const double PI = 3.1415926535897932384626;

enum graphics_drivers {     /* define graphics drivers */
	DETECT,         /* requests autodetection */
	CGA, MCGA, EGA, EGA64, EGAMONO, IBM8514,/* 1 - 6 */
	HERCMONO, ATT400, VGA, PC3270,          /* 7 - 10 */
	TRUECOLOR, TRUECOLORSIZE,
	CURRENT_DRIVER = -1
};

enum graphics_modes { /* graphics modes for each driver */
	CGAC0       = 0,  /* 320x200 palette 0; 1 page  */
	CGAC1       = 1,  /* 320x200 palette 1; 1 page  */
	CGAC2       = 2,  /* 320x200 palette 2: 1 page  */
	CGAC3       = 3,  /* 320x200 palette 3; 1 page  */
	CGAHI       = 4,  /* 640x200 1 page             */
	MCGAC0      = 0,  /* 320x200 palette 0; 1 page  */
	MCGAC1      = 1,  /* 320x200 palette 1; 1 page  */
	MCGAC2      = 2,  /* 320x200 palette 2; 1 page  */
	MCGAC3      = 3,  /* 320x200 palette 3; 1 page  */
	MCGAMED     = 4,  /* 640x200 1 page             */
	MCGAHI      = 5,  /* 640x480 1 page             */
	EGALO       = 0,  /* 640x200 16 color 4 pages   */
	EGAHI       = 1,  /* 640x350 16 color 2 pages   */
	EGA64LO     = 0,  /* 640x200 16 color 1 page    */
	EGA64HI     = 1,  /* 640x350 4 color  1 page    */
	EGAMONOHI   = 0,  /* 640x350 64K on card, 1 page - 256K on card, 4 pages */
	HERCMONOHI  = 0,  /* 720x348 2 pages            */
	ATT400C0    = 0,  /* 320x200 palette 0; 1 page  */
	ATT400C1    = 1,  /* 320x200 palette 1; 1 page  */
	ATT400C2    = 2,  /* 320x200 palette 2; 1 page  */
	ATT400C3    = 3,  /* 320x200 palette 3; 1 page  */
	ATT400MED   = 4,  /* 640x200 1 page             */
	ATT400HI    = 5,  /* 640x400 1 page             */
	VGALO       = 0,  /* 640x200 16 color 4 pages   */
	VGAMED      = 1,  /* 640x350 16 color 2 pages   */
	VGAHI       = 2,  /* 640x480 16 color 1 page    */
	PC3270HI    = 0,  /* 720x350 1 page             */
	IBM8514LO   = 0,  /* 640x480 256 colors         */
	IBM8514HI   = 1   /*1024x768 256 colors         */
};

enum graphics_errors {      /* graphresult error return codes */
	grOk                =   0,
	grNoInitGraph       =  -1,
	grNotDetected       =  -2,
	grFileNotFound      =  -3,
	grInvalidDriver     =  -4,
	grNoLoadMem         =  -5,
	grNoScanMem         =  -6,
	grNoFloodMem        =  -7,
	grFontNotFound      =  -8,
	grNoFontMem         =  -9,
	grInvalidMode       = -10,
	grError             = -11,   /* generic error */
	grIOerror           = -12,
	grInvalidFont       = -13,
	grInvalidFontNum    = -14,
	grInvalidVersion    = -18,
	grException         = 0x10,  /* ege error */
	grParamError        = 0x11,
	grInvalidRegion     = 0x12,
	grOutOfMemory       = 0x13,
	grNullPointer       = 0x14,
	grAllocError        = 0x15,
	grInvalidMemory     = 0xCDCDCDCD,
};

enum message_event {
	MSG_EVENT_UP            = 0x00,
	MSG_EVENT_DOWN          = 0x01,
	MSG_EVENT_CLICK         = 0x01,
	MSG_EVENT_DBCLICK       = 0x02,
	MSG_EVENT_MOVE          = 0x04,
	MSG_EVENT_WHEEL         = 0x10,
};

enum message_mouse {
	MSG_MOUSE_LEFT      = 0x01,
	MSG_MOUSE_RIGHT     = 0x02,
	MSG_MOUSE_MID       = 0x04,
};

// ��ɫ
enum COLORS {
	BLACK           = 0,
	BLUE            = EGERGB(0, 0, 0xA8),
	GREEN           = EGERGB(0, 0xA8, 0),
	CYAN            = EGERGB(0, 0xA8, 0xA8),
	RED             = EGERGB(0xA8, 0, 0),
	MAGENTA         = EGERGB(0xA8, 0, 0xA8),
	BROWN           = EGERGB(0xA8, 0xA8, 0),
	LIGHTGRAY       = EGERGB(0xA8, 0xA8, 0xA8),
	DARKGRAY        = EGERGB(0x54, 0x54, 0x54),
	LIGHTBLUE       = EGERGB(0x54, 0x54, 0xFC),
	LIGHTGREEN      = EGERGB(0x54, 0xFC, 0x54),
	LIGHTCYAN       = EGERGB(0x54, 0xFC, 0xFC),
	LIGHTRED        = EGERGB(0xFC, 0x54, 0x54),
	LIGHTMAGENTA    = EGERGB(0xFC, 0x54, 0xFC),
	YELLOW          = EGERGB(0xFC, 0xFC, 0x54),
	WHITE           = EGERGB(0xFC, 0xFC, 0xFC),
};

// ���ģʽ
enum fill_patterns {/* Fill patterns for get/setfillstyle */
	EMPTY_FILL,     /* fills area in background color */
	SOLID_FILL,     /* fills area in solid fill color */
	LINE_FILL,      /* --- fill */
	LTSLASH_FILL,   /* /// fill */
	SLASH_FILL,     /* /// fill with thick lines */
	BKSLASH_FILL,   /* \\\ fill with thick lines */
	LTBKSLASH_FILL, /* \\\ fill */
	HATCH_FILL,     /* light hatch fill */
	XHATCH_FILL,    /* heavy cross hatch fill */
	INTERLEAVE_FILL,/* interleaving line fill */
	WIDE_DOT_FILL,  /* Widely spaced dot fill */
	CLOSE_DOT_FILL, /* Closely spaced dot fill */
	USER_FILL       /* user defined fill */
};

enum text_just {        /* Horizontal and vertical justification
						for settextjustify */
	LEFT_TEXT   = 0,
	CENTER_TEXT = 1,
	RIGHT_TEXT  = 2,

	BOTTOM_TEXT = 0,
	/* CENTER_TEXT = 1,  already defined above */
	TOP_TEXT    = 2
};

enum line_styles {      /* Line styles for get/setlinestyle */
	SOLID_LINE      = PS_SOLID,
	CENTER_LINE     = PS_DASH,
	DOTTED_LINE     = PS_DOT,
	DASHED_LINE     = PS_DASHDOT,
	NULL_LINE       = PS_NULL,
	USERBIT_LINE    = PS_USERSTYLE,   /* User defined line style */
};

enum key_msg_flag {
	KEYMSG_CHAR_FLAG    = 2,
	KEYMSG_DOWN_FLAG    = 1,
	KEYMSG_UP_FLAG      = 1,

	KEYMSG_CHAR         = 0x40000,
	KEYMSG_DOWN         = 0x10000,
	KEYMSG_UP           = 0x20000,
	KEYMSG_FIRSTDOWN    = 0x80000,
};

enum music_state_flag {
	MUSIC_MODE_NOT_OPEN = 0x0,
	MUSIC_MODE_NOT_READY = 0x20C,
	MUSIC_MODE_PAUSE = 0x211,
	MUSIC_MODE_PLAY = 0x20E,
	MUSIC_MODE_STOP = 0x20D,
	MUSIC_MODE_OPEN = 0x212,
	MUSIC_MODE_SEEK = 0x210,
};

enum initmode_flag {
	INIT_NOBORDER       = 0x1,
	INIT_CHILD          = 0x2,
	INIT_TOPMOST        = 0x4,
	INIT_RENDERMANUAL   = 0x8,
	INIT_NOFORCEEXIT    = 0x10,
	INIT_WITHLOGO       = 0x100,
#if defined(_DEBUG) || defined(DEBUG)
	INIT_DEFAULT    = 0x0,
#else
	INIT_DEFAULT    = INIT_WITHLOGO,
#endif
	INIT_ANIMATION  = INIT_DEFAULT | INIT_RENDERMANUAL | INIT_NOFORCEEXIT,
};

enum rendermode_e {
	RENDER_AUTO,
	RENDER_MANUAL,
};

typedef enum key_code_e {
	key_mouse_l     = 0x01,
	key_mouse_r     = 0x02,
	key_mouse_m     = 0x04,
	key_back        = 0x08,
	key_tab         = 0x09,
	key_enter       = 0x0d,
	key_shift       = 0x10,
	key_control     = 0x11,
	key_menu        = 0x12,
	key_pause       = 0x13,
	key_capslock    = 0x14,
	key_esc         = 0x1b,
	key_space       = 0x20,

	key_pageup      = 0x21,
	key_pagedown    = 0x22,
	key_home        = 0x23,
	key_end         = 0x24,

	key_left        = 0x25,
	key_up          = 0x26,
	key_right       = 0x27,
	key_down        = 0x28,

	key_print       = 0x2a,
	key_snapshot    = 0x2c,
	key_insert      = 0x2d,
	key_delete      = 0x2e,

	key_0           = 0x30,
	key_1           = 0x31,
	key_2           = 0x32,
	key_3           = 0x33,
	key_4           = 0x34,
	key_5           = 0x35,
	key_6           = 0x36,
	key_7           = 0x37,
	key_8           = 0x38,
	key_9           = 0x39,

	key_A           = 0x41,
	key_Z           = 0x5a,
	key_win_l       = 0x5b,
	key_win_r       = 0x5c,

	key_sleep       = 0x5f,

	key_num0        = 0x60,
	key_num1        = 0x61,
	key_num2        = 0x62,
	key_num3        = 0x63,
	key_num4        = 0x64,
	key_num5        = 0x65,
	key_num6        = 0x66,
	key_num7        = 0x67,
	key_num8        = 0x68,
	key_num9        = 0x69,

	key_f1          = 0x70,
	key_f2          = 0x71,
	key_f3          = 0x72,
	key_f4          = 0x73,
	key_f5          = 0x74,
	key_f6          = 0x75,
	key_f7          = 0x76,
	key_f8          = 0x77,
	key_f9          = 0x78,
	key_f10         = 0x79,
	key_f11         = 0x7a,
	key_f12         = 0x7b,

	key_numlock     = 0x90,
	key_scrolllock  = 0x91,

	key_shift_l     = 0xa0,
	key_shift_r     = 0xa1,
	key_control_l   = 0xa2,
	key_control_r   = 0xa3,
	key_menu_l      = 0xa4,
	key_menu_r      = 0xa5,

	key_semicolon   = 0xba,
	key_plus        = 0xbb,
	key_comma       = 0xbc,
	key_minus       = 0xbd,
	key_period      = 0xbe,
	key_slash       = 0xbf,
	key_tilde       = 0xc0,
	key_lbrace      = 0xdb,
	key_backslash   = 0xdc,
	key_rbrace      = 0xdd,
	key_quote       = 0xde,

	key_ime_process = 0xe5,
}key_code_e;

typedef enum key_msg_e {
	key_msg_down    = 1,
	key_msg_up      = 2,
	key_msg_char    = 4,
}key_msg_e;
typedef enum key_flag_e {
	key_flag_shift  = 0x100,
	key_flag_ctrl   = 0x200,
}key_flag_e;

typedef enum mouse_msg_e {
	mouse_msg_down      = 0x10,
	mouse_msg_up        = 0x20,
	mouse_msg_move      = 0x40,
	mouse_msg_wheel     = 0x80,
}mouse_msg_e;
typedef enum mouse_flag_e {
	mouse_flag_left     = 1,
	mouse_flag_right    = 2,
	mouse_flag_mid      = 4,
	mouse_flag_shift    = 0x100,
	mouse_flag_ctrl     = 0x200,
}mouse_flag_e;

typedef enum pattern_type_e {
	pattern_none            = 0,
	pattern_lineargradient  = 1,
	pattern_pathgradient    = 2,
	pattern_texture         = 3,
}pattern_type_e;

typedef unsigned int color_t;

struct viewporttype {
	int left;
	int top;
	int right;
	int bottom;
	int clipflag;
};

struct textsettingstype {
	int font;
	int direction;
	int charsize;
	int horiz;
	int vert;
};

struct linestyletype {
	int linestyle;
	unsigned short upattern;
	int thickness;
};

typedef struct key_msg {
	int             key;
	key_msg_e       msg;
	unsigned int    flags;
}key_msg;

typedef struct mouse_msg {
	int             x;
	int             y;
	mouse_msg_e     msg;
	unsigned int    flags;
	int             wheel;
	bool is_left() {return (flags & mouse_flag_left) != 0;}
	bool is_right() {return (flags & mouse_flag_right) != 0;}
	bool is_mid() {return (flags & mouse_flag_mid) != 0;}
	bool is_down() {return msg == mouse_msg_down;}
	bool is_up() {return msg == mouse_msg_up;}
	bool is_move() {return msg == mouse_msg_move;}
	bool is_wheel() {return msg == mouse_msg_wheel;}
}mouse_msg;

typedef struct ege_point {
	float x;
	float y;
}ege_point;

typedef struct ege_rect {
	float x;
	float y;
	float w;
	float h;
}ege_rect;

typedef struct ege_colpoint {
	float   x;
	float   y;
	color_t color;
}ege_colpoint;

// �����Ϣ
EGE_DEPRECATE(MOUSEMSG)
struct MOUSEMSG {
	UINT uMsg;              // ��ǰ�����Ϣ
	bool mkCtrl;            // Ctrl ���Ƿ���
	bool mkShift;           // Shift ���Ƿ���
	bool mkLButton;         // �������Ƿ���
	bool mkMButton;         // ����м��Ƿ���
	bool mkRButton;         // ����Ҽ��Ƿ���
	short x;                // ��ǰ��� x ����
	short y;                // ��ǰ��� y ����
	short wheel;            // �����ֹ���ֵ(120Ϊ����)
};

struct msg_createwindow {
	HANDLE hEvent;
	HWND hwnd;
	LPCWSTR classname;
	DWORD style;
	DWORD exstyle;
	unsigned id;
	LPVOID param;
};


// ��ͼ������ʼ������
#define INITGRAPH(x, y) struct _initgraph_{_initgraph_(){initgraph(x, y);}\
	~_initgraph_(){closegraph();}}_g_initgraph_
#define INITGRAPH3(x, y, f) struct _initgraph_{_initgraph_(){initgraph(x, y, f);}\
	~_initgraph_(){closegraph();}}_g_initgraph_

//�������
#define MUSIC_ERROR  0xFFFFFFFF


typedef void (CALLBACK_PROC)();
typedef int (__stdcall MSG_KEY_PROC  )(void*, unsigned, int);
typedef int (__stdcall MSG_MOUSE_PROC)(void*, unsigned, int, int, int);
typedef CALLBACK_PROC       * LPCALLBACK_PROC;
typedef MSG_KEY_PROC        * LPMSG_KEY_PROC;
typedef MSG_MOUSE_PROC      * LPMSG_MOUSE_PROC;

/*
ע�⣺���º�����ע�ͺ��'###'�ĺ�����ʾδʵ��
*/

struct VECTOR3D;

// 3d ���㸨������
void EGEAPI rotate_point3d_x(VECTOR3D  * pt, float r); //���ȣ����ֶ���
void EGEAPI rotate_point3d_y(VECTOR3D  * pt, float r);
void EGEAPI rotate_point3d_z(VECTOR3D  * pt, float r);

struct VECTOR3D {
	float x, y, z;
	VECTOR3D() {
		x = 0; y = 0; z = 0;
	}
	VECTOR3D(float _x, float _y) {
		x = _x; y = _y; z = 0;
	}
	VECTOR3D(float _x, float _y, float _z) {
		x = _x; y = _y; z = _z;
	}
	VECTOR3D& operator = (const VECTOR3D& _fp)  {
		x = _fp.x; y = _fp.y; z = _fp.z;
		return *this;
	}
	VECTOR3D& operator += (const VECTOR3D& _fp);
	VECTOR3D& operator -= (const VECTOR3D& _fp);
	VECTOR3D operator + (const VECTOR3D& _fp) const;
	VECTOR3D operator - (const VECTOR3D& _fp) const;
	VECTOR3D& operator *= (float f); //����
	VECTOR3D operator * (float f) const; //����
	float operator * (const VECTOR3D& _fp) const; //���
	VECTOR3D operator & (const VECTOR3D& _fp) const; //���
	VECTOR3D& operator &= (const VECTOR3D& _fp); //���
	float GetModule() const;
	float GetSqrModule() const {
		return float(x*x + y*y + z*z);
	}
	VECTOR3D& SetModule(float m) {
		float t = m / GetModule();
		*this *= t;
		return *this;
	}
	VECTOR3D& Rotate(float rad, const VECTOR3D& v); //����������ת�����ֶ���radΪ����
	VECTOR3D& Rotate(float rad, float x, float y, float z) {
		VECTOR3D v(x, y, z);
		return Rotate(rad, v);
	}
	VECTOR3D& Rotate(const VECTOR3D& e, const VECTOR3D& s = VECTOR3D(0.0f, 0.0f, 1.0f)); //��s��e֮��ļн�ȷ����ת
	static float GetAngel(const VECTOR3D& e, const VECTOR3D& s = VECTOR3D(0.0f, 0.0f, 1.0f));
};


class IMAGE;
typedef IMAGE *PIMAGE;

// ��ͼ������غ���

void EGEAPI initgraph(int Width, int Height, int Flag = INIT_DEFAULT);    // ��ʼ��ͼ�λ���
void EGEAPI initgraph(int* gdriver, int* gmode, char* path);   // ���� Borland C++ 3.1 �����أ�ֻʹ�� 640x480x24bit
void EGEAPI closegraph();                                      // �ر�ͼ�λ���
bool EGEAPI is_run();   // �ж�UI�Ƿ��˳�
void EGEAPI setcaption(LPCSTR  caption);
void EGEAPI setcaption(LPCWSTR caption);

void EGEAPI setrendermode(rendermode_e mode);

// ��ͼ��������
PIMAGE  gettarget();
int     settarget(PIMAGE pbuf); // �� NULL ���ô���Ϊ��ͼĿ��

void EGEAPI cleardevice(PIMAGE pimg = NULL);                   // ����

void EGEAPI getviewport(int *pleft, int *ptop, int *pright, int *pbottom, int *pclip = 0, PIMAGE pimg = NULL); // ��ȡ��ͼ��Ϣ
void EGEAPI setviewport(int left, int top, int right, int bottom, int clip = 1, PIMAGE pimg = NULL);           // ������ͼ
void EGEAPI clearviewport(PIMAGE pimg = NULL);                                                                 // �����ͼ

EGE_DEPRECATE(setactivepage)
void EGEAPI setactivepage(int page); // ���õ�ǰ��ͼҳ������ͼ����Ĭ�ϵ�������壬��Χ0-1��Ĭ��Ϊ0
EGE_DEPRECATE(setvisualpage)
void EGEAPI setvisualpage(int page); // ���õ�ǰ��ʾҳ������������ʾ�������ϵ�ҳ����Χ0-1��Ĭ��Ϊ0
EGE_DEPRECATE(swappage)
void EGEAPI swappage();
void EGEAPI window_getviewport(struct viewporttype * viewport);
void EGEAPI window_getviewport(int* left, int* top, int* right, int* bottom);
void EGEAPI window_setviewport(int  left, int  top, int  right, int  bottom);

// ��ͼ����
EGE_DEPRECATE(setactivepage)
void EGEAPI getlinestyle(int *plinestyle, unsigned short *pupattern = NULL, int *pthickness = NULL, PIMAGE pimg = NULL); // ��ȡ��ǰ����
void EGEAPI setlinestyle(int linestyle, unsigned short upattern = 0, int thickness = 1, PIMAGE pimg = NULL);          // ���õ�ǰ����
void EGEAPI setlinewidth(float width, PIMAGE pimg = NULL);          // ���õ�ǰ�߿�
//EGE_DEPRECATE(setactivepage)
//void getfillstyle(color_t *pcolor, int *ppattern = NULL, PIMAGE pimg = NULL);           // ��ȡ������� ###
EGE_DEPRECATE(setactivepage)
void EGEAPI setfillstyle(int pattern, color_t color, PIMAGE pimg = NULL);  // �����������

void EGEAPI setwritemode(int mode, PIMAGE pimg = NULL);         // ���û�ͼλ����ģʽ

//void EGEAPI graphdefaults(PIMAGE pimg = NULL);                  // �������л�ͼ����ΪĬ��ֵ ###

// ɫ�ʺ���
color_t EGEAPI getcolor(PIMAGE pimg = NULL);                    // ��ȡ��ǰ��ͼǰ��ɫ
color_t EGEAPI getfillcolor(PIMAGE pimg = NULL);                // ��ȡ��ǰ��ͼ���ɫ
color_t EGEAPI getbkcolor(PIMAGE pimg = NULL);                  // ��ȡ��ǰ��ͼ����ɫ
void EGEAPI setcolor(color_t color, PIMAGE pimg = NULL);        // ���õ�ǰ��ͼǰ��ɫ
void EGEAPI setfillcolor(color_t color, PIMAGE pimg = NULL);    // ���õ�ǰ��ͼ���ɫ
void EGEAPI setbkcolor(color_t color, PIMAGE pimg = NULL);      // ���õ�ǰ��ͼ����ɫ�����ò�������ɫ�����滻��
void EGEAPI setbkcolor_f(color_t color, PIMAGE pimg = NULL);    // �������õ�ǰ��ͼ����ɫ��ֻ���ò��滭��
void EGEAPI setfontbkcolor(color_t color, PIMAGE pimg = NULL);  // ���õ�ǰ���ֱ���ɫ
void EGEAPI setbkmode(int iBkMode, PIMAGE pimg = NULL);         // ���ñ������ģʽ(0=OPAQUE, 1=TRANSPARENT)
void EGEAPI setinitmode(int mode = INIT_DEFAULT, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT); //���ó�ʼ��ģʽ��mode=0Ϊ��ͨ��1Ϊ�ޱ߿򴰿ڣ�xy�ǳ�ʼ��������
int  EGEAPI attachHWND(HWND hWnd);

// ���ݺ�
#define RGBtoGRAY   rgb2gray
#define RGBtoHSL    rgb2hsl
#define RGBtoHSV    rgb2hsv
#define HSLtoRGB    hsl2rgb
#define HSVtoRGB    hsv2rgb

// ��ɫģ��ת������
color_t     EGEAPI rgb2gray(color_t rgb);
void        EGEAPI rgb2hsl(color_t rgb, float *H, float *S, float *L);
void        EGEAPI rgb2hsv(color_t rgb, float *H, float *S, float *V);
color_t     EGEAPI hsl2rgb(float H, float S, float L);
color_t     EGEAPI hsv2rgb(float H, float S, float V);


// ������ͼ����

color_t     EGEAPI getpixel  (int x, int y, PIMAGE pimg = NULL);                // ��ȡ�����ɫ
void        EGEAPI putpixel  (int x, int y, color_t color, PIMAGE pimg = NULL); // ����
color_t     EGEAPI getpixel_f(int x, int y, PIMAGE pimg = NULL);                // ��ȡ�����ɫ
void        EGEAPI putpixel_f(int x, int y, color_t color, PIMAGE pimg = NULL); // ����
void        EGEAPI putpixels  (int nPoint, int* pPoints, PIMAGE pimg = NULL);   // ��������
void        EGEAPI putpixels_f(int nPoint, int* pPoints, PIMAGE pimg = NULL);   // ��������

void EGEAPI moveto(int x, int y, PIMAGE pimg = NULL);                      // �ƶ���ǰ��(��������)
void EGEAPI moverel(int dx, int dy, PIMAGE pimg = NULL);                   // �ƶ���ǰ��(�������)

void EGEAPI line(int x1, int y1, int x2, int y2, PIMAGE pimg = NULL);      // ����
void EGEAPI linerel(int dx, int dy, PIMAGE pimg = NULL);                   // ����(���������)
void EGEAPI lineto(int x, int y, PIMAGE pimg = NULL);                      // ����(����������)
void EGEAPI line_f(float x1, float y1, float x2, float y2, PIMAGE pimg = NULL);  // ����
void EGEAPI linerel_f(float dx, float dy, PIMAGE pimg = NULL);                   // ����(���������)
void EGEAPI lineto_f(float x, float y, PIMAGE pimg = NULL);                      // ����(����������)


void EGEAPI rectangle(int left, int top, int right, int bottom, PIMAGE pimg = NULL);   // ������

//void EGEAPI getarccoords(int *px, int *py, int *pxstart, int *pystart, int *pxend, int *pyend, PIMAGE pimg = NULL);    // ��ȡԲ��������Ϣ ###
void EGEAPI arc(int x, int y, int stangle, int endangle, int radius, PIMAGE pimg = NULL);                  // ��Բ��
void EGEAPI circle(int x, int y, int radius, PIMAGE pimg = NULL);                                          // ��Բ
void EGEAPI pieslice(int x, int y, int stangle, int endangle, int radius, PIMAGE pimg = NULL);             // �����Բ����
void EGEAPI ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius, PIMAGE pimg = NULL);// ����Բ����
void EGEAPI fillellipse(int x, int y, int xradius, int yradius, PIMAGE pimg = NULL);                       // �������Բ
void EGEAPI sector(int x, int y, int stangle, int endangle, int xradius, int yradius, PIMAGE pimg = NULL); // �������Բ����

void EGEAPI arcf(float x, float y, float stangle, float endangle, float radius, PIMAGE pimg = NULL);                    // ��Բ��
void EGEAPI circlef(float x, float y, float radius, PIMAGE pimg = NULL);                                                // ��Բ
void EGEAPI pieslicef(float x, float y, float stangle, float endangle, float radius, PIMAGE pimg = NULL);               // �����Բ����
void EGEAPI ellipsef(float x, float y, float stangle, float endangle, float xradius, float yradius, PIMAGE pimg = NULL);// ����Բ����
void EGEAPI fillellipsef(float x, float y, float xradius, float yradius, PIMAGE pimg = NULL);                           // �������Բ
void EGEAPI sectorf(float x, float y, float stangle, float endangle, float xradius, float yradius, PIMAGE pimg = NULL); // �������Բ����

void EGEAPI bar(int left, int top, int right, int bottom, PIMAGE pimg = NULL);                             // ���ޱ߿�������
void EGEAPI bar3d(int left, int top, int right, int bottom, int depth, int topflag, PIMAGE pimg = NULL);   // ���б߿���ά������

void EGEAPI drawpoly(int numpoints, const int *polypoints, PIMAGE pimg = NULL);     // �������
void EGEAPI drawlines(int numlines, const int *polypoints, PIMAGE pimg = NULL);     // �������������ߣ���չ������
void EGEAPI drawbezier(int numpoints, const int *polypoints, PIMAGE pimg = NULL);   // ��bezier���ߣ���չ������
void EGEAPI fillpoly(int numpoints, const int *polypoints, PIMAGE pimg = NULL);     // �����Ķ����
void EGEAPI fillpoly_gradient(int numpoints, const ege_colpoint* polypoints, PIMAGE pimg = NULL); // ���������Ķ����
void EGEAPI floodfill(int x, int y, int border, PIMAGE pimg = NULL);                // ���߽���ɫ�������
void EGEAPI floodfillsurface(int x, int y, color_t areacolor, PIMAGE pimg = NULL);  // ����ʼ����ɫ�������

// �߼���ͼ��������AA��
// ege new_api
void EGEAPI ege_enable_aa(bool enable, PIMAGE pimg = NULL);

void EGEAPI ege_line(float x1, float y1, float x2, float y2, PIMAGE pimg = NULL);
void EGEAPI ege_drawpoly(int numpoints, ege_point* polypoints, PIMAGE pimg = NULL);
void EGEAPI ege_drawcurve(int numpoints, ege_point* polypoints, PIMAGE pimg = NULL);
void EGEAPI ege_rectangle(float x, float y, float w, float h, PIMAGE pimg = NULL);
void EGEAPI ege_ellipse(float x, float y, float w, float h, PIMAGE pimg = NULL);
void EGEAPI ege_pie(float x, float y, float w, float h, float stangle, float sweepAngle, PIMAGE pimg = NULL);

void EGEAPI ege_arc(float x, float y, float w, float h, float stangle, float sweepAngle, PIMAGE pimg = NULL);
void EGEAPI ege_bezier(int numpoints, ege_point* polypoints, PIMAGE pimg = NULL);

void EGEAPI ege_fillpoly(int numpoints, ege_point* polypoints, PIMAGE pimg = NULL);
void EGEAPI ege_fillrect(float x, float y, float w, float h, PIMAGE pimg = NULL);
void EGEAPI ege_fillellipse(float x, float y, float w, float h, PIMAGE pimg = NULL);
void EGEAPI ege_fillpie(float x, float y, float w, float h, float stangle, float sweepAngle, PIMAGE pimg = NULL);

void EGEAPI ege_setpattern_none(PIMAGE pimg = NULL);
void EGEAPI ege_setpattern_lineargradient(float x1, float y1, color_t c1, float x2, float y2, color_t c2, PIMAGE pimg = NULL);
void EGEAPI ege_setpattern_pathgradient(ege_point center, color_t centercolor,
	int count, ege_point* points, int colcount, color_t* pointscolor, PIMAGE pimg = NULL);
void EGEAPI ege_setpattern_ellipsegradient(ege_point center, color_t centercolor,
	float x, float y, float w, float h, color_t color, PIMAGE pimg = NULL);
void EGEAPI ege_setpattern_texture(PIMAGE srcimg, float x, float y, float w, float h, PIMAGE pimg = NULL);

void EGEAPI ege_setalpha(int alpha, PIMAGE pimg = NULL);
void EGEAPI ege_gentexture(bool gen, PIMAGE pimg = NULL);
void EGEAPI ege_puttexture(PIMAGE srcimg, float x, float y, float w, float h, PIMAGE pimg = NULL);
void EGEAPI ege_puttexture(PIMAGE srcimg, ege_rect dest, PIMAGE pimg = NULL);
void EGEAPI ege_puttexture(PIMAGE srcimg, ege_rect dest, ege_rect src, PIMAGE pimg = NULL);
//

//int  EGEAPI Begin2d();
//void EGEAPI EndRender();

//ʱ�亯�������º��������ڶ��߳���ʹ�ã�ֻ�ܸ���ͼ���̵߳��ã�
void EGEAPI ege_sleep(long ms);     // �����ӳ�ms����
void EGEAPI delay(long ms);         // �����ӳ�ms����
void EGEAPI delay_ms(long ms);      // ƽ���ӳ�ms����
void EGEAPI delay_fps(int fps);    // ƽ���ӳ�1000/fps���룬�����ȶ�֡�ʿ���
void EGEAPI delay_fps(long fps);    // ƽ���ӳ�1000/fps���룬�����ȶ�֡�ʿ���
void EGEAPI delay_fps(double fps);  // ƽ���ӳ�1000/fps���룬�����ȶ�֡�ʿ���
void EGEAPI delay_jfps(int fps);   // ƽ���ӳ�1000/fps���룬�����ȶ��߼�֡�ʿ��ƣ���ͼ����֡
void EGEAPI delay_jfps(long fps);   // ƽ���ӳ�1000/fps���룬�����ȶ��߼�֡�ʿ��ƣ���ͼ����֡
void EGEAPI delay_jfps(double fps); // ƽ���ӳ�1000/fps���룬�����ȶ��߼�֡�ʿ��ƣ���ͼ����֡
// ���º������Զ��߳���ʹ�ã���ͼ��(worker)�̵߳�sleepʹ�����
void EGEAPI api_sleep(long dwMilliseconds);
double EGEAPI fclock(); // ��ȡ����Ϊ��λ�ĸ���ʱ�䣬ֻ���ڼ�ʱ�ã�����0.01�룬

// ������غ���
void EGEAPI outtext(LPCSTR  textstring, PIMAGE pimg = NULL);                   // �ڵ�ǰλ���������
void EGEAPI outtext(LPCWSTR textstring, PIMAGE pimg = NULL);                   // �ڵ�ǰλ���������
void EGEAPI outtext(CHAR  c, PIMAGE pimg = NULL);                              // �ڵ�ǰλ������ַ�
void EGEAPI outtext(WCHAR c, PIMAGE pimg = NULL);                              // �ڵ�ǰλ������ַ�
void EGEAPI outtextxy(int x, int y, LPCSTR  textstring, PIMAGE pimg = NULL);   // ��ָ��λ���������
void EGEAPI outtextxy(int x, int y, LPCWSTR textstring, PIMAGE pimg = NULL);   // ��ָ��λ���������
void EGEAPI outtextxy(int x, int y, CHAR c, PIMAGE pimg = NULL);               // ��ָ��λ������ַ�
void EGEAPI outtextxy(int x, int y, WCHAR c, PIMAGE pimg = NULL);              // ��ָ��λ������ַ�
void EGEAPI outtextrect(int x, int y, int w, int h, LPCSTR  textstring, PIMAGE pimg = NULL); // ��ָ�����η�Χ�������
void EGEAPI outtextrect(int x, int y, int w, int h, LPCWSTR textstring, PIMAGE pimg = NULL); // ��ָ�����η�Χ�������
void EGEAPI xyprintf(int x, int y, LPCSTR  fmt, ...); // ��ָ��λ�������ʽ���ַ�����ָ����ͼĿ�����settarget
void EGEAPI xyprintf(int x, int y, LPCWSTR fmt, ...); // ��ָ��λ�������ʽ���ַ�����ָ����ͼĿ�����settarget
void EGEAPI rectprintf(int x, int y, int w, int h, LPCSTR  fmt, ...); // ��ָ�����������ʽ���ַ�����ָ����ͼĿ�����settarget
void EGEAPI rectprintf(int x, int y, int w, int h, LPCWSTR fmt, ...); // ��ָ�����������ʽ���ַ�����ָ����ͼĿ�����settarget
int  EGEAPI textwidth(LPCSTR  textstring, PIMAGE pimg = NULL);                 // ��ȡ�ַ���ռ�õ����ؿ�
int  EGEAPI textwidth(LPCWSTR textstring, PIMAGE pimg = NULL);                 // ��ȡ�ַ���ռ�õ����ؿ�
int  EGEAPI textwidth(CHAR  c, PIMAGE pimg = NULL);
int  EGEAPI textwidth(WCHAR c, PIMAGE pimg = NULL);
int  EGEAPI textheight(LPCSTR  textstring, PIMAGE pimg = NULL);                // ��ȡ�ַ���ռ�õ����ظ�
int  EGEAPI textheight(LPCWSTR textstring, PIMAGE pimg = NULL);                // ��ȡ�ַ���ռ�õ����ظ�
int  EGEAPI textheight(CHAR  c, PIMAGE pimg = NULL);
int  EGEAPI textheight(WCHAR c, PIMAGE pimg = NULL);
void EGEAPI settextjustify(int horiz, int vert, PIMAGE pimg = NULL);

// ���õ�ǰ������ʽ(�������)
//      nHeight: �ַ���ƽ���߶ȣ�
//      nWidth: �ַ���ƽ�����(0 ��ʾ����Ӧ)��
//      lpszFace: �������ƣ�
//      nEscapement: �ַ�������д�Ƕ�(��λ 0.1 ��)��
//      nOrientation: ÿ���ַ�����д�Ƕ�(��λ 0.1 ��)��
//      nWeight: �ַ��ıʻ���ϸ(0 ��ʾĬ�ϴ�ϸ)��
//      bItalic: �Ƿ�б�壻
//      bUnderline: �Ƿ��»��ߣ�
//      bStrikeOut: �Ƿ�ɾ���ߣ�
//      fbCharSet: ָ���ַ�����
//      fbOutPrecision: ָ�����ֵ�������ȣ�
//      fbClipPrecision: ָ�����ֵļ������ȣ�
//      fbQuality: ָ�����ֵ����������
//      fbPitchAndFamily: ָ���Գ��淽ʽ�������������ϵ�С�
void EGEAPI setfont(int nHeight, int nWidth, LPCSTR lpszFace,  PIMAGE pimg = NULL);
void EGEAPI setfont(int nHeight, int nWidth, LPCWSTR lpszFace, PIMAGE pimg = NULL);
void EGEAPI setfont(int nHeight, int nWidth, LPCSTR lpszFace,  int nEscapement, int nOrientation,
					int nWeight, int bItalic, int bUnderline, int bStrikeOut, PIMAGE pimg = NULL);
void EGEAPI setfont(int nHeight, int nWidth, LPCWSTR lpszFace, int nEscapement, int nOrientation,
					int nWeight, int bItalic, int bUnderline, int bStrikeOut, PIMAGE pimg = NULL);
void EGEAPI setfont(int nHeight, int nWidth, LPCSTR lpszFace,  int nEscapement, int nOrientation,
					int nWeight, int bItalic, int bUnderline, int bStrikeOut, BYTE fbCharSet,
					BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality, BYTE fbPitchAndFamily, PIMAGE pimg = NULL);
void EGEAPI setfont(int nHeight, int nWidth, LPCWSTR lpszFace, int nEscapement, int nOrientation,
					int nWeight, int bItalic, int bUnderline, int bStrikeOut, BYTE fbCharSet,
					BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality, BYTE fbPitchAndFamily, PIMAGE pimg = NULL);
void EGEAPI setfont(const LOGFONTA *font, PIMAGE pimg = NULL); // ���õ�ǰ������ʽ
void EGEAPI setfont(const LOGFONTW *font, PIMAGE pimg = NULL); // ���õ�ǰ������ʽ
void EGEAPI getfont(LOGFONTA *font, PIMAGE pimg = NULL);       // ��ȡ��ǰ������ʽ
void EGEAPI getfont(LOGFONTW *font, PIMAGE pimg = NULL);       // ��ȡ��ǰ������ʽ


//ͼƬ����
#define getmaxx getwidth
#define getmaxy getheight

int EGEAPI getwidth(PIMAGE pimg = NULL);           // ��ȡͼƬ���
int EGEAPI getheight(PIMAGE pimg = NULL);          // ��ȡͼƬ�߶�
int EGEAPI getx(PIMAGE pimg = NULL);               // ��ȡ��ǰ x ����
int EGEAPI gety(PIMAGE pimg = NULL);               // ��ȡ��ǰ y ����

PIMAGE      EGEAPI newimage(); // ����PIMAGE
PIMAGE      EGEAPI newimage(int width, int height); // ����PIMAGE
void        EGEAPI delimage(PIMAGE pImg); // ����PIMAGE
color_t*    EGEAPI getbuffer(PIMAGE pImg);

int  EGEAPI resize(PIMAGE pDstImg, int width, int height); //����ߴ�
void EGEAPI getimage(PIMAGE pDstImg, int srcX, int srcY, int srcWidth, int srcHeight);             // ����Ļ��ȡͼ��
void EGEAPI getimage(PIMAGE pDstImg, const PIMAGE pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight);   // ����һ�� PIMAGE �����л�ȡͼ��
int  EGEAPI getimage(PIMAGE pDstImg, LPCSTR  pImgFile, int zoomWidth = 0, int zoomHeight = 0);     // ��ͼƬ�ļ���ȡͼ��(bmp/jpg/gif/emf/wmf)
int  EGEAPI getimage(PIMAGE pDstImg, LPCWSTR pImgFile, int zoomWidth = 0, int zoomHeight = 0);     // ��ͼƬ�ļ���ȡͼ��(bmp/jpg/gif/emf/wmf)
int  EGEAPI getimage(PIMAGE pDstImg, LPCSTR  pResType, LPCSTR  pResName, int zoomWidth = 0, int zoomHeight = 0);   // ����Դ�ļ���ȡͼ��(bmp/jpg/gif/emf/wmf)
int  EGEAPI getimage(PIMAGE pDstImg, LPCWSTR pResType, LPCWSTR pResName, int zoomWidth = 0, int zoomHeight = 0);   // ����Դ�ļ���ȡͼ��(bmp/jpg/gif/emf/wmf)
void EGEAPI putimage(int dstX, int dstY, const PIMAGE pSrcImg, DWORD dwRop = SRCCOPY);                         // ����ͼ����Ļ
void EGEAPI putimage(int dstX, int dstY, int dstWidth, int dstHeight, const PIMAGE pSrcImg, int srcX, int srcY, DWORD dwRop = SRCCOPY);    // ����ͼ����Ļ(ָ�����)
void EGEAPI putimage(int dstX, int dstY, int dstWidth, int dstHeight, const PIMAGE pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight, DWORD dwRop = SRCCOPY);   // ����ͼ����Ļ(ָ��Դ��ߺ�Ŀ���߽�������)
void EGEAPI putimage(PIMAGE pDstImg, int dstX, int dstY, const PIMAGE pSrcImg, DWORD dwRop = SRCCOPY);         // ����ͼ����һͼ����
void EGEAPI putimage(PIMAGE pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, const PIMAGE pSrcImg, int srcX, int srcY, DWORD dwRop = SRCCOPY);    // ����ͼ����һͼ����(ָ�����)
void EGEAPI putimage(PIMAGE pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, const PIMAGE pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight, DWORD dwRop = SRCCOPY);   // ����ͼ����һͼ����(ָ��Դ��ߺ�Ŀ���߽�������)
int  EGEAPI saveimage(PIMAGE pimg, LPCSTR  filename);
int  EGEAPI saveimage(PIMAGE pimg, LPCWSTR filename);
int  EGEAPI savepng(PIMAGE pimg, LPCSTR  filename, int bAlpha = 0);
int  EGEAPI savepng(PIMAGE pimg, LPCWSTR filename, int bAlpha = 0);
int  EGEAPI getimage_pngfile(PIMAGE pimg, LPCSTR  filename);
int  EGEAPI getimage_pngfile(PIMAGE pimg, LPCWSTR filename);

int EGEAPI putimage_transparent(
	PIMAGE imgdest,         // handle to dest
	PIMAGE imgsrc,          // handle to source
	int nXOriginDest,       // x-coord of destination upper-left corner
	int nYOriginDest,       // y-coord of destination upper-left corner
	color_t crTransparent,  // color to make transparent
	int nXOriginSrc = 0,    // x-coord of source upper-left corner
	int nYOriginSrc = 0,    // y-coord of source upper-left corner
	int nWidthSrc = 0,      // width of source rectangle
	int nHeightSrc = 0      // height of source rectangle
);
int EGEAPI putimage_alphablend(
	PIMAGE imgdest,         // handle to dest
	PIMAGE imgsrc,          // handle to source
	int nXOriginDest,       // x-coord of destination upper-left corner
	int nYOriginDest,       // y-coord of destination upper-left corner
	unsigned char alpha,    // alpha
	int nXOriginSrc = 0,    // x-coord of source upper-left corner
	int nYOriginSrc = 0,    // y-coord of source upper-left corner
	int nWidthSrc = 0,      // width of source rectangle
	int nHeightSrc = 0      // height of source rectangle
);
int EGEAPI putimage_alphatransparent(
	PIMAGE imgdest,         // handle to dest
	PIMAGE imgsrc,          // handle to source
	int nXOriginDest,       // x-coord of destination upper-left corner
	int nYOriginDest,       // y-coord of destination upper-left corner
	color_t crTransparent,  // color to make transparent
	unsigned char alpha,    // alpha
	int nXOriginSrc = 0,    // x-coord of source upper-left corner
	int nYOriginSrc = 0,    // y-coord of source upper-left corner
	int nWidthSrc = 0,      // width of source rectangle
	int nHeightSrc = 0      // height of source rectangle
);
int EGEAPI putimage_withalpha(
	PIMAGE imgdest,         // handle to dest
	PIMAGE imgsrc,          // handle to source
	int nXOriginDest,       // x-coord of destination upper-left corner
	int nYOriginDest,       // y-coord of destination upper-left corner
	int nXOriginSrc = 0,    // x-coord of source upper-left corner
	int nYOriginSrc = 0,    // y-coord of source upper-left corner
	int nWidthSrc = 0,      // width of source rectangle
	int nHeightSrc = 0      // height of source rectangle
);
int EGEAPI imagefilter_blurring (
	PIMAGE imgdest,         // handle to dest
	int intensity,
	int alpha,
	int nXOriginDest = 0,
	int nYOriginDest = 0,
	int nWidthDest = 0,
	int nHeightDest = 0
);
int EGEAPI putimage_rotate(
	PIMAGE imgdest,
	PIMAGE imgtexture,
	int nXOriginDest,
	int nYOriginDest,
	float centerx,
	float centery,
	float radian,
	int btransparent = 0,           // transparent (1) or not (0)
	int alpha = -1,                  // in range[0, 256], alpha== -1 means no alpha
	int smooth = 0
);

int EGEAPI putimage_rotatezoom(
	PIMAGE imgdest,
	PIMAGE imgtexture,
	int nXOriginDest,
	int nYOriginDest,
	float centerx,
	float centery,
	float radian,
	float zoom,
	int btransparent = 0,           // transparent (1) or not (0)
	int alpha = -1,                  // in range[0, 256], alpha== -1 means no alpha
	int smooth = 0
);

class PushTarget {
public:
	PushTarget() { m_target = gettarget(); }
	PushTarget(PIMAGE target) { m_target = gettarget(); settarget(target); }
	~PushTarget() { settarget(m_target); }
private:
	PIMAGE m_target;
};

#define CTL_PREINIT(classname, parent) struct preinit_obj { \
		preinit_obj(classname* This, int inheritlevel) { \
			This->pre_init(inheritlevel); \
		} \
	}_preinit_obj; \
	enum inherit_e { inherit_level_e = parent::inherit_level_e + 1, }; \
	static void firstinit(ege::egeControlBase* This) { \
		((classname*)This)->m_inheritlevel = 1; \
	} \
	void pre_init(int inheritlevel) {\

#define CTL_PREINITEND  }
#define CTL_DEFPARAM    int inherit = inherit_level_e, ege::egeControlBase* pParent = NULL
#define CTL_INITBASE(parent)    _preinit_obj(this, inherit_level_e), parent(inherit, (ege::egeControlBase*)pParent)
#define CTL_INIT        InitObject iobj(this, inherit_level_e);\
						ege::PushTarget _pushtarget(buf());

#define EGECTRL_INITEND()     }

class egeControlBase
{
public:
	enum ROP {
		COPY    = SRCCOPY,
		XOR     = SRCINVERT,
		AND     = SRCAND,
		OR      = SRCPAINT,
	};
	enum blendmode_e {
		SOLIDCOPY = 0,
		ALPHABLEND = 1,
	};
	enum inherit_e {
		inherit_level_e = 0,
	};
	// ���캯�������Զ��壬��Ҫ����Ҫѡ��ʹ��ʹ�ú꣬���ǰ���������ĵ�ʾ������
	egeControlBase();
	egeControlBase(int inherit, egeControlBase* pParent);
	~egeControlBase();

	// �����麯������Ҫֱ���໥����
	virtual LRESULT onMessage(UINT message, WPARAM wParam, LPARAM lParam) { return 0; }
	// ���º���������ط�0�����ӿؼ����ݼ��������Ϣ
	virtual int  onMouse(int x, int y, int flag) { return 0; }
	virtual int  onKeyDown(int key, int flag) { return 0; }
	virtual int  onKeyUp(int key, int flag) { return 0; }
	virtual int  onKeyChar(int key, int flag) { return 0; }
	// ��Ļ���º�ᱻ���ã����ڸ����߼�
	virtual int  onUpdate() { return 0; }
	// ����GetFocus��Ҫ��ý���ʱ���ã�����ֵһ�㷵��0��ʾ��ȡ�������뽹�㣬���ط�0����������뽹��
	virtual int  onGetFocus() { return 0; }
	// ʧȥ���뽹��ʱ����
	virtual void onLostFocus() { }
	// ���óߴ�ǰ���ã��Զ�����������
	virtual void onSizing(int *w, int *h) {}
	// ��Ӧ�ߴ�仯����
	virtual void onSize(int w, int h) {}
	// �ػ溯���������뻭��pimg�ϣ��Ա��ܿ��ƻ滭Ŀ��
	virtual void onDraw(PIMAGE pimg) const {}
	// �ߴ�仯ʱ���ã������ػ����˻���������
	virtual void onResetFilter() {
		setbkcolor(BLACK, m_mainFilter);
		cleardevice(m_mainFilter);
	}
	virtual void onAddChild(egeControlBase* pChild) {}
	virtual void onDelChild(egeControlBase* pChild) {}
	virtual void onIdle() {} // �����ӿڣ�δ��
	// �������ϵĺ����������ж��壨ע������Ҫһ�£���Ҫ©��OnDraw���const��
	// �������µ�public�������Ե��ã������Զ��壬�κ�Ԥ�����������Ҫֱ�ӷ��ʣ���ʹ��Ԥ���庯��������
public:
	PIMAGE buf() { return m_mainbuf; }
	PIMAGE filter() { return m_mainFilter; }
	egeControlBase* parent() { return m_parent; }
	PIMAGE buf() const { return m_mainbuf; }
	PIMAGE filter() const { return m_mainFilter; }
	const egeControlBase* parent() const { return m_parent; }

	void blendmode(int mode) { m_AlphablendMode = mode; }
	void setrop(int rop) { m_rop = rop; } // ����ö������ROP���������

	void directdraw(bool bdraw) { m_bDirectDraw = (bdraw ? 1 : 0); }
	bool isdirectdraw() const { return (m_bDirectDraw != 0); }
	void autoredraw(bool bvisable)  { m_bAutoDraw = (m_bAutoDraw ? 1 : 0); }
	bool isautoredraw() const       { return (m_bAutoDraw != 0);}
	void visable(bool bvisable)     { m_bVisable = (bvisable ? 1 : 0); }
	bool isvisable() const          { return (m_bVisable != 0);}
	void enable(bool benable)       { m_bEnable = (benable ? 1 : 0); }
	bool isenable() const           { return (m_bEnable != 0);}
	void capture(bool bcapture)     { m_bCapture = (bcapture ? 1 : 0); }
	bool iscapture() const          { return (m_bCapture != 0); }
	void capmouse(bool bcapmouse)   { m_bCapMouse = (bcapmouse ? 1 : 0); }
	bool iscapmouse() const         { return (m_bCapMouse != 0); }
	bool isfocus() const            { return (m_bInputFocus != 0); }
	void move(int x, int y)         { m_x = x; m_y = y; }
	void size(int w, int h) {
		onSizing(&w, &h);
		m_w = w; m_h = h;
		resize(m_mainbuf, w, h);
		resize(m_mainFilter, w, h);
		onSize(w, h);
		onResetFilter();
	}
	void zorderup();
	void zorderdown();
	void zorderset(int z);

	int getx()      const { return m_x; }
	int gety()      const { return m_y; }
	int getw()      const { return m_w; }
	int geth()      const { return m_h; }
	int width()     const { return m_w; }
	int height()    const { return m_h; }

	int  addchild(egeControlBase* pChild);
	int  delchild(egeControlBase* pChild);
	void draw(PIMAGE pimg);
	void update();
	void mouse(int x, int y, int flag);
	void keymsgdown(unsigned key, int flag);
	void keymsgup(unsigned key, int flag);
	void keymsgchar(unsigned key, int flag);
	bool operator < (const egeControlBase& pbase) const {
		if (m_zOrderLayer != pbase.m_zOrderLayer)
			return m_zOrderLayer < pbase.m_zOrderLayer;
		if (m_zOrder == pbase.m_zOrder)
			return this < &pbase;
		else
			return m_zOrder < pbase.m_zOrder;
	}
protected:
	int allocId();
	int allocZorder();
	class InitObject {
	public:
		InitObject(egeControlBase* pThis, int inherit_level);
		~InitObject();
	private:
		egeControlBase* m_this;
		int m_inherit_level;
	};
	void (* m_preinit_func )(egeControlBase*);
private:
	void init(egeControlBase* parent);
	void fixzorder();
	void sortzorder();
#if _MSC_VER <= 1200
public:
#endif
	void initok();
private:
	PIMAGE   m_mainbuf;      // ������
	PIMAGE   m_mainFilter;   // ������

private:
	int m_bVisable;     // �Ƿ�ɼ�
	int m_bEnable;      // �Ƿ�ɻ�����루���̺���꣩
	int m_bAutoDraw;    // �Ƿ��Զ��滭��������
	int m_bCapture;     // �Ƿ�ɻ�ü������뽹��
	int m_bInputFocus;  // �Ƿ��Ѿ�������뽹��
	int m_bCapMouse;    // �Ƿ�׽��꣨��ʹ�������������ڣ�
	int m_zOrderLayer;  // Z����㣨ֵ�ϴ�����ǰ��ֵ��С�߻ᱻ�����ؼ��ڵ���
	int m_zOrder;       // Z����ֵ�ϴ�����ǰ��ֵ��С�߻ᱻ�����ؼ��ڵ���
	int m_allocId;      // ����id
	int m_allocZorder;  // ����Z����

	egeControlBase* m_parent;
	static int s_maxchildid;   // ��һ���ӿؼ�����IDֵ

#ifdef _GRAPH_LIB_BUILD_
public:
#else
private:
#endif
	void* m_childmap;       // �ӿؼ�
	void* m_childzorder;    // �ӿؼ�����

protected:
	int m_x, m_y;       // ���Ͻ�����
	int m_w, m_h;       // ���

protected:
	DWORD   m_rop;              // ��Ϸ�ʽ
	int     m_AlphablendMode;   // �滭��Ϲ��˷�ʽ
	int     m_bDirectDraw;      // ����ֱ�ӻ滭
#if _MSC_VER <= 1200
public:
#endif
	int     m_inheritlevel;     // �̳в��
};

// ��������

HWND        EGEAPI getHWnd();         // ��ȡ��ͼ���ھ��
HINSTANCE   EGEAPI getHInstance();
PVOID       EGEAPI getProcfunc();
int         EGEAPI getGraphicsVer();   // ��ȡ��ǰ�汾 ###
float       EGEAPI getfps(); // ��ȡ��ǰ֡��

//�������
void            EGEAPI randomize();
unsigned int    EGEAPI random(unsigned int n);
double          EGEAPI randomf();

//�߼����뺯��
// title �Ի�����⣬ text �Ի�����ʾ���֣� buf�����������ݵ��ַ���ָ�룬 lenָ��buf����󳤶ȣ�Ҳͬʱ�������������ݳ���
int EGEAPI inputbox_getline(LPCSTR  title, LPCSTR  text, LPSTR  buf, int len);  //�����Ի������û����룬��ǰ����������ͣ�����ط�0��ʾ������Ч��0Ϊ��Ч
int EGEAPI inputbox_getline(LPCWSTR title, LPCWSTR text, LPWSTR buf, int len);  //�����Ի������û����룬��ǰ����������ͣ�����ط�0��ʾ������Ч��0Ϊ��Ч


//���̴�����
int     EGEAPI kbmsg();
key_msg EGEAPI getkey();
EGE_DEPRECATE(getchEx)
int     EGEAPI getchEx(int flag);
EGE_DEPRECATE(kbhitEx)
int     EGEAPI kbhitEx(int flag);
int     EGEAPI keystate(int key);       // ��ü���Ϊkey�ļ�����key_code_e���Ƿ��£����keyʹ����갴���ļ��룬���õ�������״̬
void    EGEAPI flushkey();              // ��ռ�����Ϣ������

//#ifndef _GRAPH_LIB_BUILD_
#if !defined(_INC_CONIO) && !defined(_CONIO_H_)
#define _INC_CONIO
#define _CONIO_H_
int EGEAPI getch();
int EGEAPI kbhit();
#else
#define getch getchEx
#define kbhit kbhitEx
#endif
//#endif

//��괦����
int         EGEAPI mousemsg();                  // ����Ƿ���������Ϣ
mouse_msg   EGEAPI getmouse();                  // ��ȡһ�������Ϣ�����û�У��͵ȴ�
EGE_DEPRECATE(GetMouseMsg)
MOUSEMSG    EGEAPI GetMouseMsg();               // �����Ƽ�ʹ�õĺ�������ȡһ�������Ϣ�����û�У��͵ȴ�

void        EGEAPI flushmouse();                // ��������Ϣ������
int         EGEAPI showmouse(int bShow);        // �����Ƿ���ʾ���
int         EGEAPI mousepos(int *x, int *y); // ��ȡ��ǰ���λ��

/*
callback function define as:
int __stdcall on_msg_key(void* param, unsigned msg, int key);
msg: see 'enum message_event'
key: keycode
return zero means process this message, otherwise means pass it and then process with 'getkey' function
*/
//int message_addkeyhandler(void* param, LPMSG_KEY_PROC func);        //���ü��̻ص�����
/*
callback function define as:
int __stdcall on_msg_mouse(void* param, unsigned msg, int key, int x, int y);
msg: see 'enum message_event'
key: see 'enum message_mouse', if msg==MSG_EVENT_WHELL, key is a int number that indicates the distance the wheel is rotated, expressed in multiples or divisions of WHEEL_DELTA, which is 120.
x,y: current mouse (x, y)
return zero means process this message, otherwise means pass it and then process with 'GetMouseMsg' function
*/
//int message_addmousehandler(void* param, LPMSG_MOUSE_PROC func);    //�������ص�����
int EGEAPI SetCloseHandler(LPCALLBACK_PROC func);


class MUSIC
{
public:
	MUSIC();
	virtual ~MUSIC();
	operator HWND()const{return (HWND)m_dwCallBack;}
public:
	int   IsOpen() {return (m_DID != MUSIC_ERROR) ? 1 : 0;}
	DWORD OpenFile(LPCSTR  filepath);
	DWORD OpenFile(LPCWSTR filepath);
	DWORD Play(DWORD dwFrom=MUSIC_ERROR, DWORD dwTo=MUSIC_ERROR);
	DWORD Pause();
	DWORD Seek(DWORD dwTo); //����λ�ö�λ����λΪms
	DWORD SetVolume(float value);
	DWORD Close();
	DWORD Stop();
	DWORD GetPosition();
	DWORD GetLength();
	// ���º���GetPlayStatus�ķ���ֵΪ����֮һ�����忴��׺����
	// MUSIC_MODE_NOT_OPEN   //û����ȷ��
	// MUSIC_MODE_NOT_READY  //�豸û׼���� ������ʹ�ã�
	// MUSIC_MODE_PAUSE  //��ͣ��
	// MUSIC_MODE_PLAY   //���ڲ���
	// MUSIC_MODE_STOP   //�ɹ��򿪺󣬻��߲����������״̬
	// MUSIC_MODE_OPEN   //���� ������ʹ�ã�
	// MUSIC_MODE_SEEK   //��λ�� ������ʹ�ã�
	DWORD GetPlayStatus();
private:
	DWORD       m_DID;
	PVOID       m_dwCallBack;
};

/* ѹ������ */
	/* ѹ��ʱdest������Ҫ��֤��С��СΪsourceLen * 1.001 + 16 */
	/* ����compress/compress2ǰ��*destLen������ֵ����ʾdest������������С������ʱ���ֵ��ʾʵ�ʴ�С */
	/* compress2 ��level ��0-9��0��ѹ����9���ѹ����compress����ʹ��Ĭ��ֵ6 */
int             EGEAPI ege_compress(void *dest, unsigned long *destLen, const void *source, unsigned long sourceLen);
int             EGEAPI ege_compress2(void *dest, unsigned long *destLen, const void *source, unsigned long sourceLen, int level);
int             EGEAPI ege_uncompress(void *dest, unsigned long *destLen, const void *source, unsigned long sourceLen);
unsigned long   EGEAPI ege_uncompress_size(const void *source, unsigned long sourceLen); /* ����0��ʾ����������ʾ��С */

} // namespace ege

#ifndef _GRAPH_LIB_BUILD_

#if defined(_MSC_VER) && (defined(HIDE_CONSOLE) || !defined(SHOW_CONSOLE))
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif

#define Sleep(ms) delay_ms(ms)

#endif

#if !defined(_MSC_VER)
#define WinMain(...) WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
#elif defined(_CONSOLE)
#if (_MSC_VER > 1200)
#define WinMain(...) main(int argc, char* argv[])
#else
#define WinMain main
#endif
#endif

#endif
