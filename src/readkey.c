// funcion que le lo eventos del teclado
void term_readkey()
{
  /// devuelve el caracter leido  desde el teclado
  List *inputbuffer= getInput();
  List *com = &comand; // aqui se guardan los comandos
  if(inputbuffer == NULL) return; // la lista esta vacia
  for(int y=0; y<inputbuffer->count; y++)
    {
      wchar_t * str = (wchar_t *)inputbuffer->head->data;
      int len=sizeof(wchar_t)*2;
	      wchar_t up[2];
	      wchar_t down[2];
	      wchar_t right[2];
	      wchar_t left[2];
	      wchar_t * back=L"\x1b[3~";
	      up[1]=L'\0';
	      down[1]=L'\0';
	      right[1]=L'\0';
	      left[1]=L'\0';
	      *up=ARROW_UP(0x5E00);
	      *down=ARROW_DOWN(0x5E00);
	      *right=ARROW_RIGHT(0x5E00);
	      *left=ARROW_LEFT(0x5E00);
      if(str[0] == L'\x1b')
	{
	  if(str[1]== L'[')
	    {
	      switch(str[2])
		{
		case L'A':appendNodoToList(com,up,len);break;
		case L'B':appendNodoToList(com,down,len); break;
		case L'C':appendNodoToList(com,right,len); break;
		case L'D':appendNodoToList(com,left,len); break;
		case L'3':
		  {
		    appendNodoToList(com,back,wcslen(back));
		    //appendNodoToList(com,&left,2*sizeof(wchar_t));
		  } break;
		}
	    }
	}
      else
	{
	  if((str[0] >= L'\a') && (str[0] <= L'\r'))
	    {
	      appendNodoToList(com,&str[0],4);
	    }
	  else
	    {
	      int len= (wcslen(&str[0])+1)* sizeof(wchar_t);
	      switch(str[0])
		{
		case CTRL_KEY('m'):
		  {
		    MODO_CURSOR = MOD_FREE_CURSOR; 
		    
		  }break;
		case CTRL_KEY('i'):return;break;
		case CTRL_KEY('a'):return;break;
		case CTRL_KEY('f'):return;break;
		case CTRL_KEY('j'):return;break;
		  //case L'':return;break; ñ
		case CTRL_KEY('c'):return;break;
		case CTRL_KEY('v'):return;break;
		case CTRL_KEY('h'):return;break;
		case CTRL_KEY('q'):
		case CTRL_KEY('w'):
		case CTRL_KEY('e'):
		case CTRL_KEY('r'):
		case CTRL_KEY('t'):
		case CTRL_KEY('y'):
		case CTRL_KEY('u'):
		case CTRL_KEY('o'):
		case CTRL_KEY('p'):
		case CTRL_KEY('s'):
		case CTRL_KEY('d'):
		case CTRL_KEY('g'):
		case CTRL_KEY('k'):
		case CTRL_KEY('l'):
		case CTRL_KEY('z'):
		case CTRL_KEY('x'):
		case CTRL_KEY('b'):
		case CTRL_KEY('n'):
		  {
		    // como correspondiente ctrl+ el alfabeto
		    // tienen que ser mappeados
		    appendNodoToList(com,&str[0],len);
		    appendNodoToList(com,right,2);
		    appendNodoToList(com,right,2);
		  }break;
		default:
		  {
		    /// todo: aqui ingresan los caracteres al buffer de comos
		    appendNodoToList(com,&str[0],len);
		    // se agrega una secuencia para mover el cursor despues de cada
		    // ingreso de tecla
		    // appendNodoToList(com,&right,2*sizeof(wchar_t));
		  }break;
		}
	    }
	}
    }
}
