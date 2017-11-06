/*
 * evalCirc.h
 *
 *  Created on: 21/05/2017
 *      Author: ctho065
 */

#ifndef EVALCIRC_H_
#define EVALCIRC_H_

// evaluate circuit, return index of circuit-output area of vm[]
long evalOrCreateCirc8211(void);  // g8221 format (lgw <= 6, at most two gate-types)
long evalOrCreateCirc8221(void);  // g8221 format (lgw <= 14, at most two gate-types)
long evalOrCreateCirc8231(void);  // g8231 format (lgw <= 22, at most two gate-types)
long evalOrCreateCirc8241(void);  // g8241 format (lgw <= 30, at most two gate-types)

void createCirc8214(void);  // g8214 format (lgw <= 6, at most two gate-types)
long evalCirc8214(void); // tuned for evals
void createCirc8224(void); // creates a g8224 file (lgw <= 14, at most two gate-types)
long evalCirc8224(void); // tuned for evals
void createCirc8234(void); // creates a g8234 file (lgw <= 22, at most two gate-types)
long evalCirc8234(void); // tuned for evals
void createCirc8244(void); // creates a g8244 file (lgw <= 30, at most two gate-types)
long evalCirc8244(void); // tuned for evals


#endif /* EVALCIRC_H_ */
