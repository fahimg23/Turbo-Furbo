#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pigpio.h>

#include "DcMotor.h"

DcMotor::DcMotor(int pin, int dir, int pwm_freq, int pwm_dc)
{
  /* Initalizes pin for motor */
  this->pin = pin;
  this->dir = dir;
  this->pwm_dc = pwm_dc;
  this->pwm_freq = pwm_freq;
  state = 0;
}

int DcMotor::get_pin() { return pin; }
int DcMotor::get_dir() { return dir; }
int DcMotor::get_pwm_dc() { return gpioGetPWMdutycycle(pin); }
int DcMotor::get_pwm_freq() { return gpioGetPWMfrequency(pin); }
int DcMotor::get_state() { return state; }

void DcMotor::set_pin(int pin) { this->pin = pin; }

void DcMotor::set_dir(int dir)
{
  // put in code to program motor controller;
  this->dir = dir;
}

void DcMotor::set_pwm_dc(int pwm_dc) { this->pwm_dc = pwm_dc; }
void DcMotor::set_pwm_freq(int pwm_freq) { this->pwm_freq = pwm_freq; }

int DcMotor::start()
{
  int stat;
  int pwm_stat;

  pwm_stat = gpioHardwarePWM(pin, pwm_freq, pwm_dc);

  if (!pwm_stat) { // pwm_stat is 0 if OK

    if (pwm_freq > 0 && pwm_dc > 0) {
      state = 1; // motor state
      stat = 1;
    }
    else {
      printf("PWM frequency/duty-cycle is 0! \n");
      stat = 0;
    }
  }
  else {
    printf("Failed to start pwm on motor; Error: %d \n", pwm_stat);
    stat = 0;
  }

  return stat;
}

int DcMotor::stop()
{
  int stat;
  int pwm_stat;

  pwm_stat = gpioHardwarePWM(pin, 0, 0);

  if (!pwm_stat) { // pwm_stat is 0 if OK
      state = 0;
      stat = 1;
  }
  else {
    printf("Failed to stop pwm on motor; Error: %d \n", pwm_stat);
    stat = 0;
  }

  return stat;
}
