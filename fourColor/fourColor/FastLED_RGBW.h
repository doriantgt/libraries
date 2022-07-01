//
// * Original code by Jim Bumgardner (http://krazydad.com).
// * Modified by David Madison (http://partsnotincluded.com).

//  FastLED_RGBW_h FastLED_RGBW_h FastLED_RGBW.hpp
//  fourColor
//
//  Created by Dorian Tolman on 11/19/20.
//  Copyright © 2020 Dorian Tolman. All rights reserved.
//

#ifndef FastLED_RGBW_h
#define FastLED_RGBW_h
struct CRGBW  {
    union {
        struct {
            union {
                uint8_t g;
                uint8_t green;
            };
            union {
                uint8_t r;
                uint8_t red;
            };
            union {
                uint8_t b;
                uint8_t blue;
            };
            union {
                uint8_t w;
                uint8_t white;
            };
        };
        uint8_t raw[4];
    };
    
    CRGBW(){}
    
    CRGBW(uint8_t rd, uint8_t grn, uint8_t blu, uint8_t wht){
        r = rd;
        g = grn;
        b = blu;
        w = wht;
    }
    
    inline void operator = (const CRGB c) __attribute__((always_inline)){
        this->r = c.r;
        this->g = c.g;
        this->b = c.b;
        this->white = 0;
    }
    
    
    
    inline void operator = (const CHSV hsv){
        CRGB c = hsv;
        
        
        this->r = c.r;
        this->g = c.g;
        this->b = c.b;
        this->white = 0;
    }
    inline void operator+= (const CRGBW& rhs ) //add rgbw to rgbw
    {
        r = qadd8( r, rhs.r);
        g = qadd8( g, rhs.g);
        b = qadd8( b, rhs.b);
       
        return *this;
    }
    inline void operator+= (const CRGB& rhs ) //add rgb to rgbw
    {
        
        r = qadd8( r, rhs.r);
        g = qadd8( g, rhs.g);
        b = qadd8( b, rhs.b);
       // w = qadd8( b, rhs.w);
        return *this;
    }
   
    inline CRGBW& operator-= (const CRGBW& rhs )//subtract rgbw to rgbw
    {
        r = qsub8( r, rhs.r);
        g = qsub8( g, rhs.g);
        b = qsub8( b, rhs.b);
        w = qsub8( b, rhs.w);

        return *this;
    }
    inline CRGBW& operator-= (const CRGB& rhs )//subtract rgb to rgbw
    {
        r = qsub8( r, rhs.r);
        g = qsub8( g, rhs.g);
        b = qsub8( b, rhs.b);
        return *this;
    }
    

    
};

inline uint16_t getRGBWsize(uint16_t nleds){
    uint16_t nbytes = nleds * 4;
    if(nbytes % 3 > 0) return nbytes / 3 + 1;
    else return nbytes / 3;
}

#endif
