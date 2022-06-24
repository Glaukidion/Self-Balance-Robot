/* ******************************************************************* */
/* File name:        Self_balance_Final.ino                            */
/* File description: File dedicated to the ES070 prototype projects    */
/*                   involving the Arduino board. The project is       */
/*                   a self balancing robot.                           */
/*                                                                     */
/*                   Microchip ATmega328P characteristics              */
/*                   Clock speed: 16 MHz (20 MHx max)                  */
/*                   32 KB program flash memory                        */
/*                   2 KB SRAM                                         */
/*                   Input voltage: 5 to 12 V                          */
/*                   Operating voltage: 5 V                            */
/*                                                                     */
/* Author name:      Grupo 2 - André M., Gabriel K., Gidiones N., Ru S.*/
/* Creation date:    23nov2021                                         */
/* Revision date:    05dez2021                                         */
/* ******************************************************************* */
#include <Battery.h>
#include <Warning.h>
#include <TimedPID.h>
#include <TimerInterruptCtrl.h>
#include <StepMotorCrtl.h>
#include "Wire.h"
#include <TinyMPU6050.h>

#define DIR_PIN1 3 // Direção Motor 1
#define STEP_PIN1 6 //Passo Motor 1 (pino 6 - obrigatório)
#define DIR_PIN2 2 // Direção Motor 2
#define STEP_PIN2 11 //Passo Motor 2 (pino 11 - obrigatório)
#define ENABLE_PIN1 7 //Motor 1 Enable pin
#define ENABLE_PIN2 8 //Motor 2 Enable pin
/*Robot Wheel radius in m*/
#define WHEEL_RADIUS 0.0342f

/*Angle controller gains*/
#define KP_ANGLE -9.0f
#define KI_ANGLE -131.0f
#define KD_ANGLE 0.0f
/*Velocity controller gains*/
#define KP_VEL 6.0f
#define KI_VEL 4.6f
#define KD_VEL 0.0f
/*Battery monitoring definitions*/
#define BATTERY_PIN A7
#define TARGET_VOLTAGE 3.95f
#define BATTERY_LED_PIN 13
/*Falling warning definitions*/
#define FALLING_LED_PIN 12
/*defines the saturation step speed(RPM) for the PID controller*/
#define STEPMOTOR_SPD_SAT 300.0
/*defines the saturation angle(rad) for the PID controller*/
#define ANGLE_SAT 15.0f
/*Conversion factor rad/s to RPM*/
#define CONV_RAD_TO_RPM 9.5493f

float fRead_velocity = 0.0f;
float fSumTheta = 0.0f;
float fReadTheta = 0.0f;
float fTarget_velocity = 0.0f;
float fTheta_ref = 0.0f;
unsigned char ucSatFlag = false;
float fRPM_ref = 0.0f;
volatile unsigned char ucSystemLoopCrtl = false;
volatile unsigned char ucSampleFlag = 0;
volatile unsigned long int uliPeriods;
unsigned long int uliInterruptionMicrosecs = 10000;

/*Library Constructors*/
StepMotorCrtl my_StepMotor(STEP_PIN1, STEP_PIN2, DIR_PIN1, DIR_PIN2, ENABLE_PIN1, ENABLE_PIN2);
TimerInterruptCtrl my_Timer1_Interrupt;
// Create a TimedPID object instance
TimedPID pid_velocity(KP_VEL, KI_VEL, KD_VEL);
TimedPID pid_angle(KP_ANGLE, KI_ANGLE, KD_ANGLE);
Battery sys_battery(BATTERY_PIN, TARGET_VOLTAGE, BATTERY_LED_PIN);
Warning sys_warnings(FALLING_LED_PIN);
MPU6050 mpu (Wire);
 
void setup() {
  mpu.Initialize(0);
  mpu.Calibrate();
  my_StepMotor.step_motors_init();
  my_Timer1_Interrupt.TimerInterrupt_init(uliInterruptionMicrosecs);
  pid_angle.setCmdRange(-STEPMOTOR_SPD_SAT, STEPMOTOR_SPD_SAT);
  pid_velocity.setCmdRange(-ANGLE_SAT, ANGLE_SAT);
}

void loop(){
  /*check the battery voltage - warns if below 3.2V*/
  sys_battery.check_battery();
  mpu.Execute(uliPeriods*uliInterruptionMicrosecs/1000);
  ucSampleFlag++;
  fSumTheta += mpu.GetAngX();
  if(ucSystemLoopCrtl == true){
    ucSystemLoopCrtl = false;
    fReadTheta = fSumTheta/(float)ucSampleFlag;
    //checks if the robot falled off
    sys_warnings.check_angle(fReadTheta);
    fRead_velocity = (fRPM_ref/CONV_RAD_TO_RPM)*WHEEL_RADIUS;
    fTheta_ref = pid_velocity.getCmdStep(fTarget_velocity,
    fRead_velocity, (float)uliInterruptionMicrosecs/1000000.0);
    fRPM_ref = pid_angle.getCmdStep(fTheta_ref,
    fReadTheta, (float)uliInterruptionMicrosecs/1000000.0);
    my_StepMotor.run_motor1(fRPM_ref);
    my_StepMotor.run_motor2(fRPM_ref);
    fSumTheta = 0.0f;
    ucSampleFlag = 0;
  }
}
/*Interruption Service routine - Timer1 Compare in A*/
ISR(TIMER1_COMPA_vect){
  ucSystemLoopCrtl = true;
  uliPeriods ++;
}
