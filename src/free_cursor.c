/*
  free_cursor

 */


  switch (key) {
  case ARROW_LEFT(0x5E00):
    {
      int ofx=(ROW_TYPE(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop)) == ROW_MAIN) ? vEmu.lenDirectory : 0;
      if((vEmu.vCursor.x+ofx) != ofx )
	{
	  vEmu.vCursor.x--;
	}
      else
	{
	  if(vEmu.vCursor.y != 0)
	    {
	      vEmu.vCursor.y--;
	      scrollUp(); // se hace escroll hacia arriba
	    }
	  /// en que posicion queda la cordenada x
	  ofx=(ROW_TYPE(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop)) == ROW_MAIN) ? vEmu.lenDirectory : 0;
	  vEmu.vCursor.x= ROW_LEN(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop))-ofx;
	}
  }break;
  case ARROW_RIGHT(0x5E00):
    {
      if(vEmu.vCursor.x < vEmu.widthInChar)vEmu.vCursor.x++; // modoel<
      //comand>as
      int  ofx = (ROW_TYPE(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop)) == ROW_MAIN) ? vEmu.lenDirectory : 0;
       if((vEmu.vCursor.x+ofx) != (ROW_LEN(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop))))
	{
	  vEmu.vCursor.x++;
	}
    } break;
  case ARROW_UP(0x5E00):
    {
      if((vEmu.vCursor.y) != 0)
      {
	 vEmu.vCursor.y--;
	 scrollUp();	
      }
      int ofx=ofx = (ROW_TYPE(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop)) == ROW_MAIN) ? vEmu.lenDirectory : 0;
      vEmu.vCursor.x= ROW_LEN(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop))-ofx;   
    }break;
    case ARROW_DOWN(0x5E00):
      {
	if((vEmu.vCursor.y) < (vEmu.heightInChar-1)&&
	   ((vEmu.vCursor.y))< (vEmu.capacityRow-1))
	{
	  vEmu.vCursor.y++;
	  if(ROW_TYPE(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop)) == ROW_MAIN)
	    vEmu.vCursor.x=ROW_LEN(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop)) - vEmu.lenDirectory;
	  else
	    vEmu.vCursor.x=ROW_LEN(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop));
	}
      else
	{
	  scrollDowWithCursor();
	}
	
      } break;
  }







