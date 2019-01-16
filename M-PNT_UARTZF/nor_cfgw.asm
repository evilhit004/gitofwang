	.ref boot2nd
    .sect ".nor_config_word"
config_word:
    .word 	0x00000F01  			
  	B 		boot2nd  	
  	NOP		5 

