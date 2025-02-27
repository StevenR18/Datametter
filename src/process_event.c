
void bubleSort(KeyboardEvent *k)
{
  
  int n=BUTTON_COUNT; // Número de botones en el evento
  for (int i = 0; i < n - 1; i++)
    {
      for (int j = 0; j < n - i - 1; j++)
	{
	  // Comparar el campo `ordinal`
	  if (k->orderButtons[j].ordinal > k->orderButtons[j + 1].ordinal)
	    {
	      // Intercambiar los elementos si están en el orden incorrecto
	      Button temp = k->orderButtons[j];
	      k->orderButtons[j] = k->orderButtons[j + 1];
	      k->orderButtons[j + 1] = temp;
            }
        }
    }
}

void fill255OrderButtons(KeyboardEvent *k)
{
  int x=0;
  while((k->orderButtons[x].isDown != 255) && (x <=255))
    {
      k->orderButtons[x].vk = 255;
      k->orderButtons[x].processed = 255;
      k->orderButtons[x].ordinal = 255;
      k->orderButtons[x].isDown = 255;
      k->orderButtons[x].changed = 255;
      x++;
    }
  
}
void addEventToOrderButtons(KeyboardEvent *k)
{
  for (int x = 0; x < 255; x++)
    {
      if(k->buttons[x].isDown == 1)
	{
	  k->orderButtons[x].vk = k->buttons[x].vk;
	  k->orderButtons[x].processed =k->buttons[x].processed;
	  k->orderButtons[x].ordinal = k->buttons[x].ordinal;
	  k->orderButtons[x].isDown = k->buttons[x].isDown;
	  k->orderButtons[x].changed = k->buttons[x].changed;
	  }
    }
}

#define CTRL_CODE wchar_t 
CTRL_CODE * ctrlCode(int vk , char * ks)
{
  CTRL_CODE *code= (CTRL_CODE *) malloc(sizeof(CTRL_CODE)*2);
  code[1]=L'\0';
  int len= ToUnicode(vk,0,ks,code,2,0);
  return code;
}
  
void translate_keyevent_tovt_sequence(KeyboardEvent keyEvent)
{
  /*
    Navegación: Enter, Tab, Esc, Backspace.
    // Modificación: Shift, Ctrl, Alt, Caps Lock.
    Navegación avanzada: Arrow Keys, Home, End, Page Up, Page Down.
    Funcionales: F1 a F12.
    Sistema: PrtSc, Pause, Insert, Delete.
    ORDENAR LOS EVENTOS DE TECLADO
   */
  LARGE_INTEGER hz={0};
  LARGE_INTEGER t0={0};
  LARGE_INTEGER tf={0};
  static int count=1;
 
  if(ispressedAnyKey()|| isdownAnyKey())
    {
      QueryPerformanceCounter(&t0);
      fill255OrderButtons(&keyEvent);
      addEventToOrderButtons(&keyEvent);
      bubleSort(&keyEvent); // ordena los eventos segun el orden de llegada
      
       #if 0
	debugKeyboard(keyEvent);
#endif
#define MAX_EVENT ((x < BUTTON_COUNT )&& (keyEvent.orderButtons[x].vk!=255))
	for(int x=0; MAX_EVENT; x++)
	{
	  if(ispressedCtrlAt(keyEvent.orderButtons[x])||
	     isdownCtrlAt(keyEvent.orderButtons[x]))
	    {
	      // Modificación: Shift, Ctrl, Alt,.
	      /// Obtener el carcter alfabetico que se extrajo CTRL+A
	      //ctrCode esta funcio convierte una conbinacion de teclas CTRL+LETRA
	      // a su corresponedinte caracter
	      CTRL_CODE *code=ctrlCode(keyEvent.orderButtons[x+1].vk,
				       keyEvent.keystate);
	      int len= wcslen(code);
	      if(ispressedPrintable(keyEvent.orderButtons[x+1])||
		 ispressedOemButton(keyEvent.orderButtons[x+1]))
		{
		  // se devería agregar aparte la secuencias de escape qu esta-
		  // genera
		  appendNodoToList(&input,(wchar_t *)code,len);
		  
		  x++;
		}
	      if(code != NULL)
		{
		  free(code);
		  code=NULL;
		}
	      /// si se presiona una tecla de flecha
	      if(ispressedArrowKeyAt(keyEvent.orderButtons[x]))
		{
		  // obtener todos los botones arrowkey
		  /// si quieres procesar mas de una pulsacion las teclas flecha
		  /// deves de generar 2 secuencias de escape
		  int len= wcslen(L"\x1b[A");
		  switch(keyEvent.orderButtons[x].vk)
		    {
		    case BUTTON_UP:appendNodoToList(&input,L"\x1b[A",len);break;
		    case BUTTON_DOWN:appendNodoToList(&input,L"\x1b[B",len);break;
		    case BUTTON_RIGHT:appendNodoToList(&input,L"\x1b[C",len);break;
		    case BUTTON_LEFT:appendNodoToList(&input,L"\x1b[D",len);break;
		    default: break;
		    }
		}
	    }
	  else
	    {
	      if(ispressedPrintable(keyEvent.orderButtons[x])||
		 ispressedOemButton(keyEvent.orderButtons[x]))
		{
		  wchar_t alpha[2];
		  alpha[1]=L'\0';
		  int result= ToUnicode(keyEvent.orderButtons[x].vk,0,keyEvent.keystate,alpha,2,0);
		  if(result >0)appendNodoToList(&input,alpha,2);
		}
	      else
		{
		  if(ispressedAt(keyEvent.orderButtons[x],BUTTON_RETURN))
		    {
		      appendNodoToList(&input,L"\n",1);
		      
		    }
		  QueryPerformanceFrequency(&hz);
		  if(DOWN_AND_RELEASED(x,BUTTON_BACK))
		    {
		      int len = wcslen(L"\x1b[3");
		      appendNodoToList(&input, L"\x1b[3", len);
		    }
		}
	    }
	  
	}// END_FOR
    }// END IF
}// EN FUNC
