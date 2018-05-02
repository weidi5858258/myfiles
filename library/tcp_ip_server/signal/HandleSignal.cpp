//
// Created by root on 18-1-22.
//

#include "HandleSignal.h"

void handleSigIntSignal(int sign) {
    printf("HandleSignal::handleSigIntSignal():sign = %d\n", sign);
}

void handleSigPipeSignal(int sign) {
    printf("HandleSignal::handleSigPipeSignal():sign = %d\n", sign);
}

