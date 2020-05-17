/*Author : A7md Salama
*/

#ifndef CLOCK_H
#define CLOCK_H
#ifdef  CLOCK_H

#include "./SERVICES/Error_table.h"
#include "./SERVICES/std_types.h"

/*ClockInit
*Description : function that inits clock with desired frequency
*Input : None
*Output : ERROR if found
*/
ERROR_STATUS Clock_init(void);

#endif
#endif
