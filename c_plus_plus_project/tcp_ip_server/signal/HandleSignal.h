//
// Created by root on 18-1-22.
//

#ifndef TCP_IP_SERVER_HANDLESIGNAL_H
#define TCP_IP_SERVER_HANDLESIGNAL_H

#include <stdio.h>

void handleSigIntSignal(int sign) {
    printf("HandleSignal::handleSigIntSignal():sign = %d\n", sign);
}

void handleSigPipeSignal(int sign) {
    printf("HandleSignal::handleSigPipeSignal():sign = %d\n", sign);
}

class HandleSignal {

};


#endif //TCP_IP_SERVER_HANDLESIGNAL_H
