//
//  dayMillis.h
//  fourColor
//
//  Created by Dorian Tolman on 11/30/20.
//  Copyright © 2020 Dorian Tolman. All rights reserved.
//

#ifndef dayMillis_h
#define dayMillis_h

#include <stdio.h>


/*
 ------------------------------
 alarm timeline | alarm State Logic
 -------------------------------
                |
 Start/duration |  Ready->On
                |  Snooze->On  pressing Off turns to Off// Start cannot fire alarm from Off only Ready
 --------------------------------
                |
 running/glide  |  Off->Ready
 staying on after  Snooze->On
 activation     |
                |
if autoOff true|
On->Ready

 -------------------------------
 
Off->Ready->On->Off...
            |/\
            V|
            SnoozE
 
 */





class DayMillisSeconds {
    
    const uint32_t DAYMILLISECONDS = 86400*1000;
    const uint16_t thousdand = 1000;
    
    
private:
    
    uint32_t dayOffset = 0; //offsets may need to be created by the day in order to prevent dual rolloever problems
    uint32_t offSetMillis =0; //inputed negative offset will be inversed to positve
    uint32_t lastCurrentTime = 0; //used when currentTime() rolls over 86400*1000 get a new dayOffset to prevent millis rolloever errors
    
    
public:
    uint32_t millisTime;

    //set the day offset with internet time
    //
    //when currnt time rolls over it will use the stored dayOffset to move the dayOffset up 1 day.
    //this is to prevent 50ish day overflow problems.
    void setDayOffset(uint32_t currentMillis, uint32_t realTimeMillis){ //put millis(), and internet time);
        
        dayOffset = currentMillis - realTimeMillis;
        
    }
    bool timePassed16(uint16_t  &lastTime, uint16_t duration){
      
        if(  (uint16_t)(millisTime - lastTime) > duration){
            
            lastTime = millisTime;
            return true;

        }else{
            return false;
        }
    }
    
    //returns the current time in dayMilliseconds 86400*1000
    uint32_t currentTime(uint32_t currentMillis){ //put millis() as argument
        millisTime = currentMillis;
        uint32_t currentTime = (currentMillis - dayOffset)%DAYMILLISECONDS; //always get time from a real datatype
        
        if (currentMillis < lastCurrentTime) { //get a new dayOffset every 24 hours to prevent millis overflow problems
            setDayOffset(currentMillis ,currentTime);
        }
        
        lastCurrentTime = currentTime;
        return currentTime;
    }
};

enum AlarmState{
    Off,
    Ready,
    Snooze,
    On
    
};

class Alarm {
    const uint32_t DAYMILLISECONDS = 86400*1000;
    
    
public:
    
    //database variables
 //   String id; not used
//    String userName; not used
    uint32_t alarmTime =0;
    uint32_t duration =0;
    uint16_t alarmType = 0; // 0 is off
    uint32_t currentDayMillis;
    bool autoOff = false;
    
    //maybe make private
    //internal variables
    AlarmState alarmState = Off;
    uint32_t snoozeStartTime = 0;
    const uint32_t snoozeDuration = 3600*1000; //10min
    
    
    void updateTime(uint32_t updateCurrentDayMillis){
        currentDayMillis = updateCurrentDayMillis;
        
        if(alarmState == Ready){
            ReadyToOn();
            
        }else if (alarmState == On){
            OnToOffAuto();
            
        }else if (alarmState == Off){
            
        }
    }
    
    
    bool isAlarmTime(){
        
        //check for all alarm up until midnight
        if( currentDayMillis >= alarmTime &&  currentDayMillis < alarmTime+duration){ //this checks all alarms except
            // ones that roll over midnight
            return true;
        }
        
        if(alarmTime+duration > DAYMILLISECONDS){ //if alarm rolls over midnight
            //this will check of clock is between 0-n on the next day rolloever
            if( currentDayMillis+DAYMILLISECONDS < alarmTime+duration){ // and the clock has rolled over aswell
                
                return true;
            }
        }
        
        return false;
    }
    
    
    void ReadyToOn(){
        
        if(alarmState == Ready){
            if(isAlarmTime()){
                alarmState = On;
            }
        }
    }
    
    void OnToOffAuto(){ //turns off alarm if alarm if autoOff is false
        
        if(!isAlarmTime() && !autoOff){
            if(alarmState == On){
                alarmState = Off;
            }
        }
    }
    
    void OnToOffForce(bool buttonState){ //used when pushing a button to turn off
        
        if(buttonState){
            if(alarmState == On){
                alarmState = Off;
            }
        }
    }
    
    void OffToReady(){ //this code copied from readyToOn() but logic flipped
        
        if(alarmState == Off){
            if(!isAlarmTime()){
                alarmState = Ready;
            }
        }
    }
    
    void startSnooze(bool buttonState){ //OnToSnooze
        
        if(buttonState){
            // if (alarmState==On) {
            if (alarmState==On || alarmState == Snooze ) { //start and contiue snooze option
                snoozeStartTime = currentDayMillis;
                alarmState = Snooze;
                
            }
        }
        
    }
    
    void SnoozeToOn(uint32_t currentDayMillis){
        
        if(alarmState == Snooze){
            if (currentDayMillis - snoozeStartTime > snoozeDuration) {
                alarmState= On;
            }
            
        }
        
    }
    
};




#endif /* dayMillis_h */
