#ifndef ROW_H
#define ROW_H
#include <wchar.h>
typedef struct
{
  wchar_t *s;
  int len;
  int rowType;
}Row;
// alias para  un puntero a Row
typedef Row* RowPtr;
// alias pra un caracter dentro de una row
typedef wchar_t RowChar;


void appendRow(Row **row,const wchar_t * s, int len, int *capr);
void deleteRow(Row **row, int indexRow, int *capr);
void freeRow(Row ** row, int *capr, int *ofset1, int *ofset2);
int isYiRowisEmpty(Row **row ,int indexRow, int *capr);
int findLastRowmain(Row **row, int cursor_y/*sumar el ofset_y*/);
void appendCharToRow(Row **row,const wchar_t *c, int yIndex, int len);
void reservMemoryRow(Row **row,int yIndex,int len);
void shiftAndInserChar(Row **row,
		       wchar_t c ,
		       int len,
		       int yIndex,
		       int dest,
		       int origin,
		       int moveCount);

void appendCharsRow(Row **row,
		    const wchar_t *c,
		    int cursorX,
		    int cursorY,
		    int ofsetY,
		    int len,
		    int *capExceeded,
		    int *capr,
		    int width,
		    int height);

int isYiRowisMainAndLenisEqual7(Row **row,int y,int x,int ofset);
int deleteOneChar(Row **row,int y,int x);
void delCharOfRow(Row **row,int x, int y,int *capr);
#endif
