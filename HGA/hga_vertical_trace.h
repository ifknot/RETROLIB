/**
* HGA does not require vertical retrace timing for flicker free animation - unlike CGA cards.
* 
* However, there are occasions where large changes to the display eg swapping display buffers can benefit aesthetically 
* from timing such actions to occur in time with the vertical retrace.
*/

void hga_await_vertical_retrace();
