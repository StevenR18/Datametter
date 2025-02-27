/*
  free_cursor

 */

  switch (key) {
  case ARROW_LEFT(0x5E00):
    {
      int ofx=(ROW_TYPE(arrRow,(cursor_y+ofset_y)) == ROW_MAIN) ? lenDirectory : 0;
      if((cursor_x+ofx) != ofx )
	{
	  cursor_x--;
	}
      else
	{
	  if(cursor_y != 0)
	    {
	      cursor_y--;
	      scrollUp(); // se hace escroll hacia arriba
	    }
	  /// en que posicion queda la cordenada x
	  ofx=(ROW_TYPE(arrRow,(cursor_y+ofset_y)) == ROW_MAIN) ? lenDirectory : 0;
	  cursor_x= ROW_LEN(arrRow,(cursor_y+ofset_y))-ofx;
	}
  }break;
  case ARROW_RIGHT(0x5E00):
    {
      if(cursor_x < width)cursor_x++; // modoel<
      //comand>as
      int  ofx = (ROW_TYPE(arrRow,(cursor_y+ofset_y)) == ROW_MAIN) ? lenDirectory : 0;
       if((cursor_x+ofx) != (ROW_LEN(arrRow,(cursor_y+ofset_y))))
	{
	  cursor_x++;
	}
    } break;
  case ARROW_UP(0x5E00):
    {
      if((cursor_y) != 0)
      {
	 cursor_y--;
	 scrollUp();	
      }
      int ofx=ofx = (ROW_TYPE(arrRow,(cursor_y+ofset_y)) == ROW_MAIN) ? lenDirectory : 0;
      cursor_x= ROW_LEN(arrRow,(cursor_y+ofset_y))-ofx;   
    }break;
    case ARROW_DOWN(0x5E00):
      {
	if((cursor_y) < (height-1)&&
	   ((cursor_y))< (caprow-1))
	{
	  cursor_y++;
	  if(ROW_TYPE(arrRow,(cursor_y+ofset_y)) == ROW_MAIN)
	    cursor_x=ROW_LEN(arrRow,(cursor_y+ofset_y)) - lenDirectory;
	  else
	    cursor_x=ROW_LEN(arrRow,(cursor_y+ofset_y));
	}
      else
	{
	  scrollDowWithCursor();
	}
	
      } break;
  }







