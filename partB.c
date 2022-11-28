#include <xmc_gpio.h>
#include<stdio.h>
#include<string.h>

#define TICKS_PER_SECOND 1000
#define TICKS_WAIT 100  
#define LED1 P1_1
#define LED2 P1_0
#define GPIO_BUTTON1  XMC_GPIO_PORT1, 14
#define GPIO_BUTTON2  XMC_GPIO_PORT1, 15




volatile int32_t myDelay;
volatile int32_t btnCnt;
volatile int32_t btn1Prssd = 0;



  /** lookup table for a-z and 0-9 Morse code */
    char lookupltr[26][5] = /** 0-25 == 1-26 */ 
        {
            { '.', '-', '*', '*', '*' }, /** a */
            { '-', '.', '.', '.', '*' }, /** b */          
            { '-', '.', '-', '.', '*' }, /** c */          
            { '-', '.', '.', '*', '*' }, /** d */          
            { '.', '*', '*', '*', '*' }, /** e */          
            { '.', '.', '-', '.', '*' }, /** f */          
            { '-', '-', '.', '*', '*' }, /** g */          
            { '.', '.', '.', '.', '*' }, /** h */          
            { '.', '.', '*', '*', '*' }, /** i */          
            { '.', '-', '-', '-', '*' }, /** j */          
            { '-', '.', '-', '*', '*' }, /** k */          
            { '.', '-', '.', '.', '*' }, /** l */          
            { '-', '-', '*', '*', '*' }, /** m */          
            { '-', '.', '*', '*', '*' }, /** n */          
            { '-', '-', '-', '*', '*' }, /** o */          
            { '.', '-', '-', '.', '*' }, /** p */          
            { '-', '-', '.', '-', '*' }, /** q */          
            { '.', '-', '.', '*', '*' }, /** r */          
            { '.', '.', '.', '*', '*' }, /** s */          
            { '-', '*', '*', '*', '*' }, /** t */          
            { '.', '.', '-', '*', '*' }, /** u */          
            { '.', '.', '.', '-', '*' }, /** v */          
            { '.', '-', '-', '*', '*' }, /** w */          
            { '-', '.', '.', '-', '*' }, /** x */          
            { '-', '.', '-', '-', '*' }, /** y */          
            { '-', '-', '.', '.', '*' }, /** z */
        };

    char lookupNum[10][5] =
        {           
            { '-', '-', '-', '-', '-' }, /** 0 */          
            { '.', '-', '-', '-', '-' }, /** 1 */          
            { '.', '.', '-', '-', '-' }, /** 2 */          
            { '.', '.', '.', '-', '-' }, /** 3 */          
            { '.', '.', '.', '.', '-' }, /** 4 */          
            { '.', '.', '.', '.', '.' }, /** 5 */          
            { '-', '.', '.', '.', '.' }, /** 6 */          
            { '-', '-', '.', '.', '.' }, /** 7 */          
            { '-', '-', '-', '.', '.' }, /** 8 */          
            { '-', '-', '-', '-', '.' }, /** 9 */                     
        
        };








void SysTick_Handler(void)
{
  	if( btnCnt < 0 )
	{ btnCnt = 0; }	
  	btnCnt++;
	if( myDelay > 0)
	{
		myDelay--;
	}	
/**
  static uint32_t ticks = 0;

  ticks++;
  if (ticks == TICKS_WAIT)
  {
    //XMC_GPIO_ToggleOutput(LED1); 
    //XMC_GPIO_ToggleOutput(LED2); 
    XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, 0);
    
   ticks = 0; 
  }
  if ( ticks == TICKS_WAIT*10 )
  {
    XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 0);
  }
 */
}

void Delay(int ms )
{
	myDelay = ms;
	while( myDelay != 0 );
}


void sendTime( unsigned int n)
{

  /** send morse for 0 */
        for ( int i = 0; i < 5; i++)
        {
          if ( lookupNum[n][i] == '.')
          {
            /* LED HIGH */
            XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 1);
            /** delay 100m */
            Delay(100);
            /** LED LOW */
            XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, 1);
            /** delay 100m */
            Delay(100);
          }
          else if ( lookupNum[n][i] == '-' )
          {
              /* LED HIGH */
              XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 1);
			        /** delay 300m */
              Delay(300);

			        /** LED LOW */
              XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, 1);

			        /** delay 100ms */
              Delay(100);
          }
          else
          {
            return;
          }

        }
}

int main(void) {

  volatile int32_t currentBtnCnt;
  volatile int32_t timeDelayBtwBtn1;
    char partB[250];	
  
  /** string of interest */
    char myString[] = "I CAN MORSE";
    
    int myStrLen = sizeof(myString)/sizeof(myString[0]);
	printf("\n %c\n", myString[0] );


  const XMC_GPIO_CONFIG_t LED_config = \
        {.mode=XMC_GPIO_MODE_OUTPUT_PUSH_PULL,\
         .output_level=XMC_GPIO_OUTPUT_LEVEL_LOW,\
         .output_strength=XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE};



const XMC_GPIO_CONFIG_t out_config = \
      {.mode=XMC_GPIO_MODE_OUTPUT_PUSH_PULL,\
       .output_level=XMC_GPIO_OUTPUT_LEVEL_LOW,\
       .output_strength=XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE};
const XMC_GPIO_CONFIG_t in_config = \
      {.mode=XMC_GPIO_MODE_INPUT_TRISTATE,\
       .output_level=XMC_GPIO_OUTPUT_LEVEL_LOW,\
       .output_strength=XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE};

  XMC_GPIO_Init(XMC_GPIO_PORT1, 1, &LED_config);
  XMC_GPIO_Init(GPIO_BUTTON1,  &in_config);
  XMC_GPIO_Init(GPIO_BUTTON2,  &in_config);
  SysTick_Config(SystemCoreClock / TICKS_PER_SECOND);
//  Delay(1000); //1000 == 1sec  

static int cnt = 0;
while(1) 
{

#if 0
if( cnt > 0)
{
	Delay(4900);
}
cnt++;
#endif 

if( XMC_GPIO_GetInput(GPIO_BUTTON1) == 0 ) /** == 0 ; pressed, not released */
{
    btn1Prssd = 1; /** set to 1 as soon as button 01 is pressed once */
    currentBtnCnt = btnCnt;
    btnCnt = 0;

    for ( int i = 0; i <= (myStrLen-2); i++ )
    {
        /** space == 700ms delay */
        if ( ( ( (int)myString[i] ) ==  32 ) || ( 0 == ( (int)myString[i] ) ) )
        {
            /** LOW */
	        {
            		Delay(600);
          	}
	}
        else
        {
          /** Delay bt 2 consecutive letters 100ms */
          if(i != 0 ) 
	      {
		      if(((  (int)myString[i-1] ) !=  32 ) )
	    	  {	
			      Delay(200);
	    	  }	
	      }
		/** 0-9 ASCII */
            if ( (( (int)myString[i] ) >= 48) && ( (int)myString[i] <= 57) )
            {
                int lookupIdx = ( (int)myString[i] % 47 ) - 1;
                for ( int j = 0; j < ( sizeof( lookupNum[0])/sizeof( lookupNum[0][0] )) - 1;\
                                j++)
		{
                	if ( lookupNum[lookupIdx][j] == '.' )
                	{
                    
                		/* LED HIGH */
                        	XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 1);
                        	/** delay 100m */
                        	Delay(100);
                        	/** LED LOW */
                        	XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, 1);
                        	/** delay 100m */
                        	Delay(100);
                	}
                	else if ( lookupNum[lookupIdx][j] == '-' )
                	{
			
                        	/* LED HIGH */
                        	XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 1);

				/** delay 300m */
                        	Delay(300);
                        
				/** LED LOW */
                        	XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, 1);
                        
				/** delay 100ms */
                        	Delay(100);
                	}
			else /** '*' is found */
			{
				/** no morse */
				break;
			}
            	}
	    }
            else if ( ( (97 <= (int)myString[i]) &&  ( (int)myString[i] <= 122)) ||\
		  	( ( ( 65 <= (int)myString[i]) && ( (int)myString[i] <= 90 ) ) ))
            {
                if ( (65 <= (int)myString[i]) &&  ( (int)myString[i] <= 90) )
                {
                    myString[i] = (int)myString[i] + 32; 
                }
                
                int lookupIdx = ( (int)myString[i] % 96 ) - 1;
                for ( int j = 0; j < ( sizeof( lookupltr[0])/sizeof( lookupltr[0][0] )) - 1;\
                                j++)
                {
                    if ( lookupltr[lookupIdx][j] == '.' )
                    {

                        /* LED HIGH */
                        XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 1);
                        /** delay 100m */
                        Delay(100);
                        /** LED LOW */
                        XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, 1);
                        /** delay 100m */
                        Delay(100);
                    }
                    else if ( lookupltr[lookupIdx][j] == '-' )
                    {

                        /* LED HIGH */
                        XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 1);

			                  /** delay 300m */
                        Delay(300);
                        
			                  /** LED LOW */
                        XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, 1);
                        
			                  /** delay 100ms */
                        Delay(100);
                        
                    }
                    else /** '*' */
                    {
                        /** no morse action further */
                        break;
                    }
                }
            }  
        }
    }
} /** button one 01 pressed */

  if( XMC_GPIO_GetInput(GPIO_BUTTON2)  == 0 ) /** button 2 pressed */
  {

    if ( btn1Prssd == 0 )
    {
        /** send morse for 0 */
        for ( int i = 0; i < 5; i++)
        {
          if ( lookupNum[0][i] == '.')
          {
            /* LED HIGH */
            XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 1);
            /** delay 100m */
            Delay(100);
            /** LED LOW */
            XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, 1);
            /** delay 100m */
            Delay(100);
          }  
          else if ( lookupNum[0][i] == '-' )
          {
              /* LED HIGH */
              XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 1);
			        /** delay 300m */
              Delay(300);
                      
			        /** LED LOW */
              XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, 1);
              
			        /** delay 100ms */
              Delay(100);
          }
          else
          {
              break;
          }
          
        }
    }
    else
    {
    
      /* send morse for time delay btw 2 btn1 count */
	timeDelayBtwBtn1 = currentBtnCnt;
#if 0
	sprintf( partB, "%d" , timeDelayBtwBtn1 );	
  	
	for( int i = 0; i < strlen(partB); i++)
	{
		for( int j = 0; j<5; j++ )
		{
			if ( lookupNum[ ((int)partB[0]) ][j] == '.')
          		{
            			/* LED HIGH */
            			XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 1);
            			/** delay 100m */
            			Delay(100);
            			/** LED LOW */
            			XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, 1);
            			/** delay 100m */
            			Delay(100);
          		}
          		else if ( lookupNum[ ((int)partB[i]) ][j] == '-' )
          		{
              			/* LED HIGH */
              			XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 1);
                                /** delay 300m */
              			Delay(300);

                                /** LED LOW */
              			XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, 1);

                                /** delay 100ms */
              			Delay(100);
          		}
		
		}
	}	
#endif 
#if 1 	    
      unsigned int digits = 1000000000; /* make this very big */
      unsigned int n10 = 10 * timeDelayBtwBtn1;

    while (digits > n10) { digits /= 10; }
    while (digits /= 10) {
      sendTime ( ( timeDelayBtwBtn1 / digits ) % 10 );
      Delay(200);
    }
#endif
#if 0
      digits = pow( 10, 1+ log10(timeDelayBtwBtn1));
      while ( digits /= 10 )
      {
        sendTime( (timeDelayBtwBtn1/ digits) % 10 );
      }   
#endif 
    
    
    }
      

    }
    

  }

  return 0;
} 



