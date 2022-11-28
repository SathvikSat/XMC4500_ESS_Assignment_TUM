#include <xmc_gpio.h>


#define TICKS_PER_SECOND 1000
#define TICKS_WAIT 100  
#define LED1 P1_1
#define LED2 P1_0


volatile int32_t myDelay;

void SysTick_Handler(void)
{
	if( myDelay > 0)
	    myDelay--;
}


void Delay(int ms )
{
	myDelay = ms;
	while( myDelay != 0 );
}

int main(void) {

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

  /** string of interest */
    char myString[] = "I CAN MORSE";
    
    int myStrLen = sizeof(myString)/sizeof(myString[0]);
	printf("\n %c\n", myString[0] );


  const XMC_GPIO_CONFIG_t LED_config = \
        {.mode=XMC_GPIO_MODE_OUTPUT_PUSH_PULL,\
         .output_level=XMC_GPIO_OUTPUT_LEVEL_LOW,\
         .output_strength=XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE};

  XMC_GPIO_Init(XMC_GPIO_PORT1, 1, &LED_config);

  SysTick_Config(SystemCoreClock / TICKS_PER_SECOND);
//  Delay(1000); //1000 == 1sec  

static int cnt = 0;
while(1) 
{
	if( cnt > 0)
	    Delay(4900);

	cnt++;
   for ( int i = 0; i <= (myStrLen-2); i++ )
    {
        /** space == 700ms delay */
        if ( ( ( (int)myString[i] ) ==  32 ) || ( 0 == ( (int)myString[i] ) ) )
        {
            Delay(600);
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
                if ( lookupNum[lookupIdx][j] == '.' )
                {
                    /* code */
                }
                else if ( lookupNum[lookupIdx][j] == '-' )
                {


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
                        //char name[] = "dot";

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

			/** delay 300ms */
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
}
  return 0;
}
