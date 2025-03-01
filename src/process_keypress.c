void term_process_keypress(List *inp)
{
  int capRowExceded=0;
  wchar_t * resto=NULL; 
  for(int x=0; x<(BUTTON_COUNT-1); x++)
    {
      
      wchar_t * str =(wchar_t *)inp->head[x].data;
      int len= wcslen(str);
      if(len == 0) return;
    
      switch(str[0])
	{
	case L'\x1b':
	  {
	    if(str[1] == L'[')
	      {
		int xof=0;
		
		if(ROW_TYPE(arrRow,(cursor_y+ofset_y)) == ROW_MAIN) xof=lenDirectory;
		delCharOfRow(arrRow,(cursor_x+xof), (cursor_y+ofset_y),&caprow);
		if((row[(cursor_y+ofset_y)].rowType == ROW_MAIN)&&
		   cursor_x+xof > lenDirectory)
		  {
		    cursor_x--;
		  }
		else
		  {
		    if((ROW_TYPE(arrRow,(cursor_y+ofset_y)) != ROW_MAIN)&&
		       ((cursor_x+xof)!=0))
		      {
			cursor_x--;
		      }
		    else
		      {
		        if(cursor_y != 0)
			  {
			    //TODO: corregir esta parte
			    cursor_y--;
			    ofset_x= (wBuffer- width);
			    // if((ROW_TYPE(arrRow,(cursor_y+ofset_y)) == ROW_MAIN))
			    cursor_x= (width-ofset_x);
			    // else cursor_x= (width-1);
			      
			  }
		      }
		  }
		
	      }
	  }break;
	  #include"ctrlk.c"
	case ARROW_UP(0x5E00):
	case ARROW_DOWN(0x5E00):
	case ARROW_LEFT(0x5E00):
	case ARROW_RIGHT(0x5E00):
	  {
	    term_move_cursor(str[0]);
	  }break;
	case L'\n':
	  {
	    /// aumentar capacity row +1
#define onlyReturnifisCommandRow (isRowMain((cursor_y+ofset_y)) && (getLastRowLen() == lenDirectory))
	    
	    if(onlyReturnifisCommandRow)
	      {
		
		if(!(scrollDown(height-1)))cursor_y++;
		//else //cursor_y= ofset_y;
		  
		appendRow(arrRow,CurrentDirectory,wcslen(CurrentDirectory),&caprow);
		
		row[cursor_y+ofset_y].rowType = ROW_MAIN;

		//// crear una funcion para procesar los comandos de la terminal
		
	      }
	    else
	      {
		    wchar_t command[500];
		    extractCommand(command);
		    procesingCommands(command);

		    // luego de la ejecucion se reinicia la terminal
		    appendRow(arrRow,CurrentDirectory,wcslen(CurrentDirectory),&caprow);
		    cursor_x=0;
		    if(!scrollDown(height))
		      {
			// crear una funcion que devuelva la posicion y
			// de la ultima fila
			cursor_y=(caprow-1);   
			
		      }
		    else
		      {
			 cursor_y= height -1;   
		          
		      }		   
		    row[cursor_y+ofset_y].rowType = ROW_MAIN;
	      }
	       
	  }break;
	default:
	  // AQUI TENDR�A QUE PROCESAR LAS TECLAS QUE SE SE VAN A MOSTRAR
	  // EN LA PANTALLA
          // arreglar para que funcionen las teclas espaciadoras
	  // y tambien para que funcionen las teclas wasd
	  
	  appendCharsRow(arrRow,&str[0],
			 (cursor_x+ofset_x),
			 (cursor_y+ofset_y),
			 ofset_y,
			 1,
			 &capRowExceded,
			 &caprow,
			 wBuffer,
			 height);
	  cursor_x++;

	   int ox = (ROW_TYPE(arrRow,(cursor_y+ofset_y)) == ROW_MAIN) ? lenDirectory : 0;
	   
	   if((cursor_x+ofset_x + ox) >= (wBuffer) || (capRowExceded))
	    {
	       if(!(scrollDown(height)))
		{
		 cursor_y++;
		 ofset_x=0;
		}
	      else
		{
		  if((cursor_x +ofset_x + ox) != wBuffer)
		    {
		       if(cursor_y != 0)cursor_y--;
		       ofset_x=0;
		    }
		  else
		    {
		      cursor_y = height-1;
		      ofset_x=0;
		    }
		}
	       if(capRowExceded != 1)cursor_x=0;
	       capRowExceded=0;
	    }
	   else
	     {
	       if((cursor_x+ ox) >= width)
		 {                   
		   if(ofset_x < (wBuffer - width))
		     {
		       ofset_x++;
		       cursor_x = (width-1)- ox;
		     }
		 }
	       
	     }
	  break;
	}
    }
}
