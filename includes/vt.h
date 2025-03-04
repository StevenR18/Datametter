#ifndef VIRTUAL_T // Si MI_ARCHIVO_H no está definido
#define VIRTUAL_T // Defínelo ahora
#include <windows.h>
#include "..\..\Vt\includes\window64_platform.h"
#include "..\..\Vt\includes\keyboard.h"
#include "..\..\Vt\includes\liked_list.h"

// Aquí van las declaraciones, definiciones y prototipos
#define BMP_TYPE 0x4d42
#define HEIGHT_TERM  24
#define WIDTH_TERM   80
#define WIDTH_CHAR 8
#define HEIGHT_CHAR 16
#define _RGB 0
#pragma pack(push, 1) 
typedef struct
{
  unsigned short  bfType; // 2 byte
  unsigned int    bfSize; // 4 bytes
  unsigned short  bfReserved1; // 2 bytes
  unsigned short  bfReserved2; // 2 bytes
  unsigned int    bfOffBits; // 4 bytes
  // total de esta estrucgtura 14 bytes
}BMPFILEHEADER;

typedef struct
{
  unsigned long  biSize;  //4 bytes
  long  biWidth; // 4 bytes
  long  biHeight; // 4 bytes
  unsigned short  biPlanes; //2 bytes
  unsigned short  biBitCount; //2
  unsigned long biCompression;//4
  unsigned long  biSizeImage;//4
  long  biXPelsPerMeter; //4
  long  biYPelsPerMeter; //4
  unsigned long biClrUsed; //4
  unsigned long biClrImportant; //4 total = 42 bytes
  // despues de esta estrucutra biene la paleta si es que la tiene
}BMPINFOHEADER;
#pragma pack(pop)     

typedef struct{
  unsigned char  rgbBlue;
  unsigned char  rgbGreen;
  unsigned char  rgbRed;
  unsigned char  rgbReserved;
  // total 4 bytes
}RGB;

typedef struct
{
  unsigned char blue;
  unsigned char green;
  unsigned char red;
}BGR;

typedef struct
{
  BMPFILEHEADER BmpFile;
  BMPINFOHEADER BmpInfo;
  RGB *Rgb;
  //ESTA ESTRUCTURA SE USARÁ PRA CREAR UN NUEVO ARCHIVO BMP
  
}BMPFILE;
typedef struct
{
 
  long   bmHeight; // esto es la altura en pixeles
  long   bmPitch; // este es el ancho pero en bytes o la sancada
  unsigned short   bmBitsPixel; // bits por pixel
  void * bmBits; // puntero al primer pixele del bmp
  RGB  * palete; // puntero a la paleta  de colores  si tiene null no esta
  const char * name;
}BMP;

/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;BMP UTILITIES;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;*/
void load_bmp(BMP *Bmp, const char * path);
void flip_bmp(void * pixel, unsigned int dataType, unsigned int width, unsigned int height);
void save_bmp(BMP *Bmp,const char* name);
BMP create_bmp(const char *name, BGR * Color,unsigned int widthInPixel,unsigned int heightInPixel,unsigned char bytePerPixel);
/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;*/
/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; VIRTUAL TERMINAL;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;*/

typedef struct
{
  void * ptrBmpChar; // apunta al  primer caracter
  unsigned char asciiCode;
  unsigned char color;
  unsigned char width;
  unsigned char height;
}Cell;
typedef struct
{
  unsigned int x;
  unsigned int y;
  
}Point;

/*
typedef struct
{
  /// estructura principla de la terminal
  Point Cursor;
  int Width; // ancho en pixeles
  int Height; // alto en pixeles
  int MaxCol; // maximos de celdas por fila
  int MaxRow; // maximo de filas
  Line **ArrLine; /// array de linias
  unsigned char * NameFont;
}Vterm;
*/

typedef struct Line
{
  wchar_t * ptrStr;          // puntero a una cadena
  wchar_t * ptrEscape;      //  puntero a una secuencias de escape
  int dirEsc;               // poscion ESC esc<palabra>  / <palabra>esc
  POINT pxy;                     //   posicion de la cadena
  unsigned int len;             //    longitud de la cadena
  unsigned char isEnableEsc;   //     (1=Active ; 0= desacativado)
  struct Line * next;         //      siguiente nodo
}Line;

//Vterm *vt_inic_virtualt(int Width, int Height,unsigned char *NameFont);
//void vt_free_console(Vterm * consol);
//void load_arrascii(Cell *arr,BMP *fontSheet);
//void render_line(Line *line,HDC *hdc);
int vt_write(Line ** line, HDC *memdc);
void vt_reset_cursor();
int vt_add_line2(Line **head, const wchar_t *str, int len);
int vt_free_list(Line **head);
Nodo ** translate_keyevent_tovt_sequence(KeyboardEvent keyEvent);
void term_get_window_size();
/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;*/





#endif // Fin del include guard
