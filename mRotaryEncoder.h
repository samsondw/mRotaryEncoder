#ifndef MROTENC_H_INCLUDED
#define MROTENC_H_INCLUDED

#include "mbed.h"


/* This Class handles a rotary encoder with mechanical switches and an integrated pushbutton
 * It uses two pins, one creating an interrupt on change.
 * Rotation direction is determined by checking the state of the other pin.
 * Additionally a pushbutton switch is detected
 *
 * Operating the encoder changes an internal integer value that can be read
 * by Get() or the operator int() functions.
 * A new value can be set by Set(value) or opperator=.
 *
 * Autor: Thomas Raab (Raabinator)
 * Extendey by Karl Zweimueller (charly)
 *
 * Dent steady point     !     !     !
 *                    +-----+     +-----+
 * pinA (interrupt)   |     |     |     |
 *                  --+     +-----+     +---
 *                      +-----+     +-----+
 * pinB                 |     |     |     |
 *                  ----+     +-----+     +-
 *                           --> C.W
 * CW:  increases position value
 * CCW: decreases position value
 *
 * changelog:
 *
 * 09. Nov. 2010
 *     First version published Thomas Raab raabinator
 * 26.11.2010 extended by charly - pushbutton, pullmode, debounce, callback-system
 *
 */
class mRotaryEncoder {
public:
    /** Create a mechanical rotary encoder object connected to the specified pins
    *
    * @param pinA Switch A of quadrature encoder
    * @param pinB Switch B of quadrature encoder
    * @param pinSW Pin for push-button switch
    * @param pullmode mode for pinA pinB and pinSW like DigitalIn.mode
    * @param debounceTime_us time in micro-seconds to wait for bouncing of mechanical switches to end
    */
    mRotaryEncoder(PinName pinA, PinName pinB, PinName pinSW, PinMode pullMode=PullUp, int debounceTime_us=1000);

    /** destroy object
    *
    */
    ~mRotaryEncoder();

    /** Get the actual value of the rotary position
    *
    * @return position int value of position
    */
    int Get(void);
    inline operator int() {
        return Get();
    }

    /** Set the current position value
    *
    * @param value the new position to set
    *
    */
    void Set(int value);
    inline mRotaryEncoder& operator= ( int  value ) {
        Set(value);
        return *this;
    }

    /** attach a function to be called when switch is pressed
    * keep this function short, as no interrrupts can occour within
    *
    * @param fptr Pointer to callback-function
    */
    void attachSW(void (*fptr)(void)) {
        m_pinSW->fall(fptr);
    }

    template<typename T>
    /** attach an object member function to be called when switch is pressed
    *
    * @param tptr pointer to object
    * @param mprt pointer ro member function
    *
    */
    void attachSW(T* tptr, void (T::*mptr)(void)) {
        if ((mptr != NULL) && (tptr != NULL)) {
            m_pinSW->fall(tptr, mptr);
        }
    }

    /**  callback-System for rotation of shaft
    * attach a function to be called when the shaft is rotaded
    *  keep this function short, as no interrrupts can occour within
    *
    * @param fprt Pointer to callback-function
    */
    void attachROT(void (*fptr)(void)) {
        rotIsr.attach(fptr);
    }


    template<typename T>
    /** attach an object member function to be called when shaft is rotaded
    *
    * @param tptr pointer to object
    * @param mprt pointer ro member function
    *
    */
    void attachROT(T* tptr, void (T::*mptr)(void)) {
        if ((mptr != NULL) && (tptr != NULL)) {
            rotISR.attach(tptr, mptr);
        }
    }


private:
    InterruptIn     *m_pinA;
    DigitalIn       *m_pinB;
    volatile int    m_position;

    int             m_debounceTime_us;

    InterruptIn     *m_pinSW;

    void rise(void);
    void fall(void);

protected:
    /**
      * Callback system.
      * @ingroup INTERNALS
      */
    FunctionPointer rotIsr;


};


#endif
