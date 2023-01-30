ENTRY(_hw3_dance)
PHDRS
{ 
data PT_LOAD FLAGS ( 0x2 ) ;
text PT_LOAD FLAGS ( 0x3 ) ; 
rodata PT_LOAD FLAGS ( 0x5 );
}
SECTIONS
{
. = 0x20000;
    .data : 
  {
	*(.data)
  } :data

 .bss : 
  {
	*(.bss)
  } 
. = 0x400000;
  .text : 
  {
	*(.text)
  } :text

. = 0x90000000;
  
.rodata : 
  {
	*(.rodata)
  } :rodata
}
EXTERN(blacks greens)
