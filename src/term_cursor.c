
switch (key) {
  case ARROW_LEFT(0x5E00):
    {
      int ofx=(ROW_TYPE(arrRow,(cursor_y+ofset_y)) == ROW_MAIN) ? lenDirectory : 0;
      if((cursor_x+ofx) != ofx )
	{
	  cursor_x--;
	}
  }break;
  case ARROW_RIGHT(0x5E00):
    {
     
      int  ofx = (ROW_TYPE(arrRow,(cursor_y+ofset_y)) == ROW_MAIN) ? lenDirectory : 0;
       if((cursor_x+ofx) != (ROW_LEN(arrRow,(cursor_y+ofset_y))))
	{
	  cursor_x++;
	}
    } break;
  case ARROW_UP(0x5E00):
    {
      // ring buffer up
         
    }break;
    case ARROW_DOWN(0x5E00):
      {
	// ring buffer dow
	
      } break;
  }
