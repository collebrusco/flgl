/** 
 * al.h 
 * flgl
 * created 05/15/25 by frank collebrusco
 */
#ifndef AL_H
#define AL_H

#ifdef __APPLE__
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#else
	#include <AL/al.h>
	#include <AL/alc.h>
#endif


#endif /* AL_H */
