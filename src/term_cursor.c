
switch (key) {
  case L'D':
    {
      // LEFT
      int ofx=0;
      cursor_x--;
      if(ROW_TYPE(arrRow,(cursor_y+ofset_y))== ROW_MAIN)ofx=lenDirectory;
      if((cursor_x+ofx+ofset_x) < 0)
	{
	  ofset_x =((wBuffer- width));
	  if(ROW_TYPE(arrRow,((cursor_y-1)+ofset_y))== ROW_MAIN) ofx=lenDirectory;
	  cursor_x= (width-ofx-1);
	  cursor_y--;
	  break;
	}

      if((cursor_x+ofx+ofset_x) < ofset_x)
	{
	  ofset_x--;
	  cursor_x=0;
	  break;
	}
      if((cursor_x+ofx+ofset_x) < (ofx) ) 
	{
	  cursor_x++;
	  break;
	}   
  }break;
  case L'C':
    {
      /// cursor inicia en 0
      //RIGH
      int ofx=0;
      // en primeras aumentamos el cursor
      cursor_x++;
      if(ROW_TYPE(arrRow,(cursor_y+ofset_y))== ROW_MAIN)ofx+=lenDirectory;
      //revisa si el cursor_x+ofset_x sea menor que el len de la fila actual
      if(!((ROW_LEN(arrRow,(cursor_y+ofset_y))+1) > (cursor_x+ofset_x+ofx)))
	{
	  // aqui el cursor no se puede mover y sale
	  cursor_x--;
	  break;
	}
      if(cursor_x+ofset_x+ofx > (width+ofset_x-1))
	{
	  cursor_x--;
	  ofset_x++;
	}
      
      if((cursor_x+ofset_x+ofx)>(wBuffer-1))
	{
	  ofset_x=0;
	  cursor_x=0;
	  cursor_y++;
	}
      
    } break;
  case L'A':
    {
      // ring buffer up
         
    }break;
    case L'B':
      {
	// ring buffer dow
	
      } break;
  }
