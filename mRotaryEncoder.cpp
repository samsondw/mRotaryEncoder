#include "mbed.h"
#include "mRotaryEncoder.h"


mRotaryEncoder::mRotaryEncoder(PinName pinA, PinName pinB, PinName pinSW, PinMode pullMode, int debounceTime_us) {
    m_pinA = new InterruptIn(pinA);                    // interrrupts on pinA
    m_pinB = new DigitalIn(pinB);                      // only digitalIn for pinB

    //set pins with internal PullUP-default
    m_pinA->mode(pullMode);
    m_pinB->mode(pullMode);

    // attach interrrupts on pinA
    m_pinA->rise(this, &mRotaryEncoder::rise);
    m_pinA->fall(this, &mRotaryEncoder::fall);

    // Switch on pinSW
    m_pinSW = new InterruptIn(pinSW);                 // interrupt on press switch
    m_pinSW->mode(pullMode);

    m_position = 0;

    m_debounceTime_us = debounceTime_us;
}

mRotaryEncoder::~mRotaryEncoder() {
    delete m_pinA;
    delete m_pinB;
    delete m_pinSW;
}

int mRotaryEncoder::Get(void) {
    return m_position;
}



void mRotaryEncoder::Set(int value) {
    m_position = value;
}


void mRotaryEncoder::fall(void) {
    //no interrupts
    m_pinA->rise(NULL);
    m_pinA->fall(NULL);
    wait_us(m_debounceTime_us);            // wait while switch is bouncing
    //pinA still low?
    if (*m_pinA == 0) {
        if (*m_pinB == 1) {
            m_position++;
        } else {
            m_position--;
        }
    }
    //reenable interrupts
    m_pinA->rise(this, &mRotaryEncoder::rise);
    m_pinA->fall(this, &mRotaryEncoder::fall);
    rotIsr.call();                        // call the isr for rotation
}

void mRotaryEncoder::rise(void) {
    //no interrupts
    m_pinA->rise(NULL);
    m_pinA->fall(NULL);
    wait_us(m_debounceTime_us);            // wait while switch is bouncing
    //pinA still high?
    if (*m_pinA == 1) {
        if (*m_pinB == 1) {
            m_position--;
        } else {
            m_position++;
        }
    }
    //reenable interrupts
    m_pinA->rise(this, &mRotaryEncoder::rise);
    m_pinA->fall(this, &mRotaryEncoder::fall);
    rotIsr.call();                        // call the isr for rotation
}

