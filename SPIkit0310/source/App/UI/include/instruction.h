#ifndef INSTRUCTION_H
#define INSTRUCTION_H


#include "platform.h"
#include <string.h>
#include "stdtypes.h"
#include <stdio.h>
#include <math.h>

#define CMDRIGHT   "00"
#define CMDERROR   "01"
#define CMDFORMATERR  "02"
#define EEERROR "03"

#define VERSION  "0.9"

void Instruction_vGetCmd(void);

#endif

