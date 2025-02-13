#if (!defined VECTOR && !defined LIST)
	#error Invalid!.
#endif


#if (defined VECTOR && defined LIST)
	#error Invalid!.
#endif


#ifdef VECTOR
	#include "vector.h"
#endif


#ifdef LIST
	#include "list.h"
#endif
