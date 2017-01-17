#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <terminal.h>
#include <commandLib.h>
#include <stdlib.h>
#include <ARCbus.h>


int example_command(char **argv,unsigned short argc){
  int i,j;
  //TODO replace printf with puts ? 
  printf("This is an example command that shows how arguments are passed to commands.\r\n""The values in the argv array are as follows : \r\n");
  for(i=0;i<=argc;i++){
    printf("argv[%i] = 0x%p\r\n\t""string = \"%s\"\r\n",i,argv[i],argv[i]);
    //print out the string as an array of hex values
    j=0;
    printf("\t""hex = {");
    do{
      //check if this is the first element
      if(j!='\0'){
        //print a space and a comma
        printf(", ");
      }
      //print out value
      printf("0x%02hhX",argv[i][j]);
    }while(argv[i][j++]!='\0');
    //print a closing bracket and couple of newlines
    printf("}\r\n\r\n");
  }
  return 0;
}

int example_timer_IR(char **argv,unsigned short argc){

  WDTCTL = WDTPW+WDTHOLD;                                       // Stop WDT
  TA0CTL |= TASSEL_2 + MC_2;                                    // Setting Timer_A to SMCLCK(TASSEL_2) to continuous mode(MC_2)
  P7DIR |= 0xFF;                                                // Setting port 7 to drive LED's (0xFF ==1111 1111)
  P7OUT = 0x00;                                                 // Set all LED's on port 7 to start all off
//P7REN|=0xFF;                                                  // Play with this, Sets the pull up/down resistors high
  TA0CCTL0 = CCIE;                                              // Capture/compare interrupt enable
  TA0CCR0 = 10000;                                              // Timer0_A3 Capture/Compare @ 10000 counts


  __enable_interrupt();
  __bis_SR_register(LPM0 + GIE); // LPM0 with interrupts enabled

   for(;;){} // inf loop to wait for timer to overflow and throw an interrupt 

  return 0;
}

__interrupt void Timer_A (void){     // Timer A0 interrupt service routine TA0IV_TA0IFG
  P7OUT=0xFF; // light LEDs
}


//table of commands with help
const CMD_SPEC cmd_tbl[]={{"help"," [command]",helpCmd},
                   {"ex","[arg1] [arg2] ...\r\n\t""Example command to show how arguments are passed",example_command},
                   {"timer_IR","[time]...\r\n\tExample command to show how the timer can be used as an interupt",example_timer_IR},
                   ARC_COMMANDS,CTL_COMMANDS,ERROR_COMMANDS,
                   //end of list
                   {NULL,NULL,NULL}};

