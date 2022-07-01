//
//  fourColor.hpp
//  fourColor
//
//  Created by Dorian Tolman on 11/18/20.
//  Copyright © 2020 Dorian Tolman. All rights reserved.
//

#ifndef fourColor_hpp
#define fourColor_hpp
#include <FastLED.h>
#include <FastLED_RGBW.h>
#include <stdio.h>
    
int16_t scale8bitAccurate(uint16_t input ,uint16_t startWidth ){ //common positionFrac8 = (ledPosition,numleds)
    uint8_t endwidth = 255;  //could be an argument;
    
    float scaler = float(startWidth)/endwidth;
    uint16_t out = (input/scaler) + 0.5;
    
    return out;
}

CRGBW toCRGBW(CHSV color){
    CRGBW output;
    CRGB colorRGB = color;
    output.r = colorRGB.r;
    output.g = colorRGB.g;
    output.b = colorRGB.blue;

    
    return output;
}


//if amount is less than or greater than 1 it will subtract or add 1 to the return.
//will saturate return at 0 and 255
uint8_t un8multiFloat(uint8_t input, float amount){ //1 or greater would go up. .3 would reduce by a third
    
    float inputfloat = input;
    inputfloat =  float(input) * amount; //+ .5;
    
    if(amount < 1){ //going down //should use an && but what ever
      float difference = (float)input - inputfloat;
        
        if( difference < 1 ){ //did the output change by atleas a whole number?
          inputfloat = input;
           inputfloat -= 1;  //force whole number
          
        }
    }
  
        if(amount > 1){ //going up
          float difference = inputfloat - (float)input ; //swap sides
            
            if(inputfloat > 0){//if off stay off
            if(difference < 1) { //did the output change by atleas a whole number?
             inputfloat = input;
                inputfloat += 1; //force whole number
            }
            }
        }
    
    if(inputfloat < 0){
        inputfloat =0;
    }else if(inputfloat > 255){
        inputfloat =255;
    }
    
    input = inputfloat +.5;
    return input;
}



class RGBWstrip{
    /* to make
     fill gradient (color1,color2, shift, startpt, endpt) //fill left to right .shift if to rotate the gradient over the points
     
     fadeStrip(float fade, startpt, endpt);
     
     moontwinkle(color1,color2,float fade, startpt,endpt) //every time you do this function add 1 random led// so you could do a fade of 0 if you'd like to add more twinkles.
     
     randPlus(color1);
     
     blinking(color1, //this 1 each led has its own tempo
     
     fill stritp from pt1 to pt2. so you can draw between 2 points arbitraily.
     fill strip left to right (color, pt1, pt2) .. fill will wrap around strip
     */
    
   public:
    CRGBW * leds;
    uint16_t numLeds;
    
    RGBWstrip(CRGBW  input[] ,  uint16_t numLeds){ //or CRGBW * input
        leds = input;
        this->numLeds = numLeds;
    }
    
    void fillwith(CRGBW color){
        
        for (uint16_t i =0; i<numLeds; i++) {
            *(leds+i) = color;
        }
        
    }
    
    void fader(float amount){
        for (uint16_t i =0; i<numLeds; i++) {
            CRGBW fadeLed = *(leds+i);
            fadeLed.r = un8multiFloat(  fadeLed.r ,  amount);
            fadeLed.g = un8multiFloat(  fadeLed.g ,  amount);
            fadeLed.b = un8multiFloat(  fadeLed.b ,  amount);
            fadeLed.w = un8multiFloat(  fadeLed.w ,  amount);
       
            *(leds+i) = fadeLed;
        }
    }
    
    void rando8(CRGBW color){
        
       *(leds+random8(numLeds)) = color;
    }
    
    void rando16(CRGBW color){
        
        *(leds+random16(numLeds)) = color;
    }
    
    
    
};

class RGBW{
public:
    byte r;  byte g; byte b; byte w;
    uint32_t all; //for neopixel
    
    void setRGBW(byte r, byte g, byte b, byte w){
        this->r = r; this->g = g; this->b = b; this->w = w;
        setAll();
    }
    
    void fastRGBtoRGBW(CRGB fastIn){
        
        this->r = fastIn.r; this->g = fastIn.g; this->b = fastIn.b;
        this->w =0;
        setAll();
    }
    
    void fastHSVtoRGBW(CHSV fastInput){
        CRGB fastIn = fastInput;
        this->r = fastIn.r; this->g = fastIn.g; this->b = fastIn.b;
        this->w =0;
        setAll();
    }
    
    void setAll(){
        this->all =  this->w;
        this->all = (this->all<<8) +this->r;
        this->all = (this->all<<8) +this->g;
        this->all = (this->all<<8) +this->b;
        
    }
    
};



//Some functions below
////////////////////////////////////////////////////





#endif /* fourColor_hpp */
