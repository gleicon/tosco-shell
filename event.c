/* tosco shell event handler 
 *
 */


#include "term.h"
#include "event_types.h"


int event (int wot) {

	static  str_t old_term; 
	int result;
	
	switch (wot) {
	
		case INIT_SH:
//		initterm(&old_term);
		result=0;
		break;
	
		case EXIT_SH:
//		finishterm(&old_term);
		result=0;
		break;
	
		case HUP_SH:
		result=0;
		break;
	
		default:
		result=0;
		break;
		
		}
		
	return result;
	
	}
