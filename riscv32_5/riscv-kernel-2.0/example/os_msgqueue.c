/*
 * Copyright (c) 2012 Peter Eckstrand
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the
 * disclaimer below) provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
 * GRANTED BY THIS LICENSE.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the cocoOS operating system.
 * Author: Peter Eckstrand <info@cocoos.net>
 */




#include "cocoos.h"


typedef struct {
    Msg_t *msgPool;
    taskproctype taskproc;
    uint8_t head;
    uint8_t tail;
    uint8_t size;
    Sem_t mutex;
    Evt_t change;
    uint8_t pad;
    uint16_t messageSize;
} OSMsgQ_t;


#if( N_QUEUES > 0 )
static uint8_t MsgQEmpty( OSMsgQ_t *q );
static uint8_t MsgQFull( OSMsgQ_t *q );
static uint8_t MsgQAllDelayed( OSMsgQ_t *q );

/* List of task message queues */
static OSMsgQ_t msgQList[ N_QUEUES ];
static MsgQ_t nQueues = 0;
#endif


MsgQ_t os_msgQ_create( Msg_t *buffer, uint8_t size, uint16_t msgSize, taskproctype taskproc ) {
#if( N_QUEUES > 0 )
    os_assert( nQueues < N_QUEUES );
    msgQList[ nQueues ].msgPool = buffer;
    msgQList[ nQueues ].taskproc = taskproc;
    msgQList[ nQueues ].head = size - 1;
    msgQList[ nQueues ].tail = size - 1;
    msgQList[ nQueues ].size = size;
    msgQList[ nQueues ].mutex = sem_bin_create( 1 );
    msgQList[ nQueues ].change = event_create();
    msgQList[ nQueues ].messageSize = msgSize;
    nQueues++;
    return nQueues - 1;
#else
    return 0;
#endif
}


// Find the queue belonging to this taskproc
MsgQ_t os_msgQ_find(taskproctype taskproc) {
#if( N_QUEUES > 0 )
    MsgQ_t queue;

    for (queue = 0; queue < nQueues; queue++) {
        if ( msgQList[ queue ].taskproc == taskproc ) {
            return queue;
        }
    }
#endif

    return NO_QUEUE;
}


Sem_t os_msgQ_sem_get( MsgQ_t queue ) {
#if( N_QUEUES > 0 )
    return msgQList[ queue ].mutex;
#else
    return NO_SEM;
#endif
}


Evt_t os_msgQ_event_get( MsgQ_t queue ) {
#if( N_QUEUES > 0 )
    return msgQList[ queue ].change;
#else
    return NO_EVENT;
#endif
}


uint8_t os_msg_post( Msg_t *msg, MsgQ_t queue, uint16_t delay, uint16_t period ) {
    #if( N_QUEUES > 0 )
    OSMsgQ_t *temp;

    uint8_t *data;
    uint8_t i;

    temp = &msgQList[ queue ];

    if ( MsgQFull( temp ) == 1 ) {
        return MSG_QUEUE_FULL;
    }

    data = (uint8_t*)( (Mem_t)temp->msgPool + (temp->head) * (temp->messageSize) );

    msg->delay = delay;
    msg->reload = period;

    for ( i = 0; i < temp->messageSize; ++i ) {
        data[ i ] = ((uint8_t*)msg)[i];
    }


    /* Look for buffer wrap around */
    if ( temp->head == 0 ) {
        temp->head = temp->size - 1;
    }
    else {
        temp->head--;
    }

    /* Signal the change event */
    os_signal_event(temp->change);
    os_event_set_signaling_tid( temp->change, running_tid );

    return MSG_QUEUE_POSTED;
#else
    return 0;
#endif
}


uint8_t os_msg_receive( Msg_t *msg, MsgQ_t queue ) {
    #if( N_QUEUES > 0 )

    OSMsgQ_t *temp;
    uint8_t *data;
    uint8_t i;
    uint8_t found;

    temp = &msgQList[ queue ];

    if ( MsgQEmpty( temp ) == 1 ) {
        return MSG_QUEUE_EMPTY;
    }

    /* If all messages have a delay > 0 we consider the queue as empty */
    if ( MsgQAllDelayed( temp ) == 1 ) {
        return MSG_QUEUE_EMPTY;
    }

    /* At least one message is ready to be delivered. Find it! */
    found = 0;
    while ( found == 0 ) {

        data = (uint8_t*)( (Mem_t)temp->msgPool + (temp->tail) * (temp->messageSize) );
        for ( i = 0; i < temp->messageSize; ++i ) {
            ((uint8_t*)msg)[ i ] = data[ i ];
        }

        /* Look for buffer wrap around */
        if ( temp->tail == 0 ) {
            temp->tail = temp->size - 1;
        }
        else {
            temp->tail--;
        }

        uint8_t messagePeriodic = ( msg->reload > 0 );
        uint8_t messageTimedOut = ( msg->delay == 0 );

        if ( messageTimedOut ){
            found = 1;
            if ( messagePeriodic ) {
                msg->delay = msg->reload;
            }
        }

        /* Put the message back at head position if delay > 0, or if it is a periodic message that timed out */
        if (( !messageTimedOut ) || ( messagePeriodic && messageTimedOut )) {
            data = (uint8_t*)( (Mem_t)temp->msgPool + (temp->head) * (temp->messageSize) );

            for ( i = 0; i < temp->messageSize; ++i ) {
                data[ i ] = ((uint8_t*)msg)[i];
            }

            /* Look for buffer wrap around */
            if ( temp->head == 0 ) {
                temp->head = temp->size - 1;
            }
            else {
                temp->head--;
            }
        }
    }
    /* Signal the change event */
    os_signal_event(temp->change);
    os_event_set_signaling_tid( temp->change, running_tid );

    return MSG_QUEUE_RECEIVED;
#else
    return 0;
#endif
}


void os_msgQ_tick( MsgQ_t queue ) {
#if( N_QUEUES > 0 )
    uint16_t nextMessage;
    Msg_t *pMsg;
    OSMsgQ_t *temp = &msgQList[ queue ];

    nextMessage = temp->tail;

    while ( nextMessage != temp->head ) {
        pMsg = (Msg_t*)( (Mem_t)temp->msgPool + (nextMessage) * (temp->messageSize) );
        if ( pMsg->delay > 0 ) {
            --(pMsg->delay);
            if ( pMsg->delay == 0 ) {
                event_ISR_signal( temp->change );
            }
        }
        if ( nextMessage == 0 ) {
            nextMessage = temp->size - 1;
        }
        else {
            --nextMessage;
        }
    }
#endif    
}


#if( N_QUEUES > 0 )
static uint8_t MsgQEmpty( OSMsgQ_t *q ) {
    return ( q->head == q->tail );
}


static uint8_t MsgQFull( OSMsgQ_t *q ) {
    uint8_t nextHead;

    nextHead = q->head;

    if ( nextHead == 0 ) {
        nextHead = q->size - 1;
    }
    else {
        nextHead--;
    }

    return ( nextHead == q->tail );
}


static uint8_t MsgQAllDelayed( OSMsgQ_t *q ) {
    uint8_t nextMessage;
    Msg_t *pMsg;
    uint8_t result;

    result = 1;
    nextMessage = q->tail;

    while ( nextMessage != q->head ) {
        pMsg = (Msg_t*)( (Mem_t)q->msgPool + (nextMessage) * (q->messageSize) );
        
        if ( pMsg->delay == 0 ) {
            result = 0;
            break;
        }

        if ( nextMessage == 0 ) {
            nextMessage = q->size - 1;
        }
        else {
            --nextMessage;
        }
    }
    return result;
}

#endif
