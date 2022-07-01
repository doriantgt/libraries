//
//  stripFunctions.cpp
//  fourColor
//
//  Created by Dorian Tolman on 11/18/20.
//  Copyright © 2020 Dorian Tolman. All rights reserved.
//

#ifndef stripFunctions_h
#define stripFunctions_h
#include "dayMillis.h"
#include "fourColor.hpp"
#include <stdio.h>


class Twinkler{
public:
    uint16_t fadeFrequency = 20;// in ms;
    float fadeAmount = .985;// in ms;
    
    //we should be able to create a frequency using math as opposed to a stored variable
    uint16_t twinkleFrequency =1000; //in milliseconds should be private
    uint16_t twinkleMin =1;
    uint16_t twinkleMax =10;
    
    uint16_t  lastTimefade;
    uint16_t  lastTimeTwinkle;
    //void minMaxfreq(min, max)
    // new func
    // fadeAmountFreq(float inFadeAmount, uint16_t inFadeFrequency)
    void twinkle( DayMillisSeconds timestuff,RGBWstrip stripIn, CRGBW colorIn ){
        
        // Serial.print(" last  " );Serial.print( lastTime );     Serial.print(" current  " );Serial.println( timestuff.millisTime );
        
        if( timestuff.timePassed16(lastTimefade, fadeFrequency ) ){
            stripIn.fader(fadeAmount);
            
        }
        if( timestuff.timePassed16(lastTimeTwinkle, twinkleFrequency )) {
            
            stripIn.rando16(colorIn);
            
            twinkleFrequency = random16(twinkleMin,twinkleMax);
            
        }
    }
};

struct variables{
    uint8_t time8;//time
    uint16_t position8;//position
    
    
    int8_t counter7; //counter
    int8_t i8y;
    bool hasStarPosChanged;
    bool no;
    int16_t speed16;  //speed
};
struct Dstar {
    
    CHSV hsv;
    CRGB rgb;
    variables var;
    uint16_t location;
    int16_t vector;
    
};

class RainbowChasers {
public:
    
    uint8_t numOfStars;
    Dstar *stars;
    
    RainbowChasers( Dstar inputStars[],byte numberStars, int8_t direction, uint8_t speed=3){
        //    RainbowChasers( Dstar inputStars[],byte numberStars,int  ,){
        
        stars = inputStars;
        numOfStars= numberStars;
        
        for(byte i=0; i<numberStars; i++){
            //Stars[i].hsv.s=random8();
            stars[i].hsv.s=255;
            stars[i].hsv.v=0;
            stars[i].var.position8=i+i;//create the stars in randome positions?
            
            if(random8(2)+direction<=0){
                stars[i].var.speed16=(i+1)*-speed;//random8(10,30);
            }else{
                stars[i].var.speed16=(i+1)*speed;
            }
            // myStars[i].var.i8y=random8(5,40);
            stars[i].hsv.h=random8();
        }
    }
    
    void starSpeed(RGBWstrip stripIn,uint8_t currentTime){
        
        uint8_t timeElapsed;
        uint8_t newPosition;
        int16_t positionChange;
        
        for(uint8_t i=0; i<numOfStars; i++){
            timeElapsed=currentTime-stars[i].var.time8;//last time minus current time
            //positionChange=(stars[i].var.speed16*timeElapsed)>>11; //divide by 1000 //doesn't work with negatives
            positionChange=(stars[i].var.speed16*timeElapsed)/1000;  //divide by 1000 means that speed is number of leds per second
            
            stars[i].var.hasStarPosChanged=false; // position change bool
            
            if(positionChange!=0){  //if position changes
                stars[i].var.hasStarPosChanged=true; //position has changed
                stars[i].var.time8=currentTime; //new time
                if(positionChange>0){ stars[i].var.position8+=positionChange;//add position change
                }else{stars[i].var.position8+=(stripIn.numLeds+positionChange); }//if position change is negative }
                
            
                stars[i].var.position8%=stripIn.numLeds; //keep in range
                
            }
            
            if(stars[i].var.hasStarPosChanged){
                
                CRGB tempRGB =  stars[i].hsv;
                stripIn.leds[(stars[i].var.position8)] += tempRGB;
                
            }
        }//for
    }
    
    
};
/*
int main(){
    
    

}*/
#endif
