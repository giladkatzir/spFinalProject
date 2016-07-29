/*
 * spPreProc.h
 *
 *  Created on: 29 αιεμι 2016
 *      Author: giladkatzir
 */

#ifndef SPPREPROC_H_
#define SPPREPROC_H_

#include <stdbool.h>
#include <stdio.h>
#include "SPLogger.h"
#include "SPConfig.h"

typedef enum sp_preproc_msg_t {
	SP_PREPROC_CANNOT_OPEN_FILE,
	SP_PREPROC_ALLOC_FAIL,
	SP_PREPROC_INVALID_ARGUMENT,
	SP_PREPROC_SUCCESS
} SP_PREPROC_MSG;

SP_PREPROC_MSG spPreProc

#endif /* SPPREPROC_H_ */
