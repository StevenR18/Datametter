#ifndef PROCESS_KEYPRESS_H
#define PROCESS_KEYPRESS_H
#include <wchar.h>
#include "..\..\Vt\includes\static_list.h"
#include "..\..\Vt\includes\consol.h"

extern size_t lenDirectory;
extern ArrRow arrRow;

void delChar();
int isEscapeDelete(wchar_t * str,int len);
void processSequences(wchar_t * str, int len);
void processNewLine();
void adjustCursor();
void processChar(wchar_t *str, int len);
void proceesKeyPress(List *inp);
#endif
