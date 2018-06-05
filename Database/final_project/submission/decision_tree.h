/*
 * int.h
 *
 *  Created on: Sep 6, 2017
 *      Author: alexl
 */

#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

#include <stdio.h>
#include "data_structures/queue.h"
#include "data_structures/da.h"

extern QUEUE *infixToPostfix(QUEUE *infix_queue);
extern int calculatePostfix(QUEUE *postfix_queue,DA *columns,DA *values);
extern int isNumber(char *token);

#endif /* INT_H_ */
