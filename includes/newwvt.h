#ifndef NEWWVT_T
#define NEWWVT_T
#include <wchar.h>
#include "..\..\Vt\includes\liked_list.h"

typedef struct
{
  POINT saveCursor;
  POINT cursor;
  wchar_t *glifo;
  char flag;
  Nodo *inputBuffer;
}Termv; // Terminal virtual

#define  saveCursor Emu.saveCursor
#define  cursor     Emu.cursor
#define  glifo      Emu.glifo
#define  flag       Emu.flag
#define  input      Emu.inputBuffer

#define ON_CURSIVE(ex)((ex) |(0x01))
#define OF_CURSIVE(ex)((ex)&(~0x01))
#define ON_UNDERLINE(ex)((ex)|(0x02))
#define OF_UNDERLINE(ex)((ex)&(~0x01))
#define MODIFIED 0x04
#define ISMODIFIED(ex)((ex)&(0x04))
typedef struct
{
  unsigned int fBackground;
  unsigned int fForeground;
  unsigned int fWeight;
  unsigned int fItalic;
  unsigned int fUnderline;
  unsigned int fMode;
}TextAtributes;
extern TextAtributes TextAtri;

Nodo * getInput();
wchar_t * getGlifo();
Nodo ** getRootInput();
void appendBuffer(const wchar_t *s, int len);
void freeBuffer();
void printBuffer();

#endif
