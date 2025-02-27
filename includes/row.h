#ifndef COPIA_ROW_H
#define COPIA_ROW_H
#include <wchar.h>
typedef struct
{
  wchar_t *s;
  int len;
  int rowType;
}Row;
// array de filas
typedef Row ** ArrRow;
// alias para  un puntero a Row
typedef Row * RowPtr;
// alias pra un caracter dentro de una row

#define ROW_STRING(array,indexr)((*array)[(indexr)].s)
#define ROW_CHARAT(array,indexr,cellindex)((*array)[(indexr)].s[(cellindex)])
#define ROW_LEN(array,indexr)((*array)[indexr].len)
#define NULL_TERMINATE(array,indexr,len)((*array)[(indexr)].s[(len)]=L'\0')
#define ROW_TYPE(array,indexr)((*array)[(indexr)].rowType)



void appendRow(ArrRow arr, wchar_t * c, int len, int *capr);
void deleteRow(ArrRow arr,int indexRow, int *capr);
void freeRow(ArrRow arr, int *capr, int *ofset1, int *ofset2);
int RowisEmpty(ArrRow arr,int indexRow);
int findLastRowmain(ArrRow arr, int indexRow/*sumar el ofset_y*/);
void appendCharToRow(ArrRow arr, wchar_t * c, int indexRow, int len);
void reservMemoryRow(ArrRow arr,int indexRow,int len);
void shiftAndInserChar(ArrRow arr,
		       wchar_t c ,
		       int len,
		       int indexRow,
		       int dest,
		       int origin,
		       int moveCount);

void appendCharsRow(ArrRow arr,
		    wchar_t * c,
		    int cursorX,
		    int cursorY,
		    int ofsetY,
		    int len,
		    int *capExceeded,
		    int *capr,
		    int width,
		    int height);

int RowisMainAndLenisEqualofset(ArrRow arr,int indexRow,int indexCell,int ofset);
int deleteOneChar(ArrRow arr,int indexRow, int indexCell);
void delCharOfRow(ArrRow arr,int indexCell, int indexRow,int *capr);
#endif
