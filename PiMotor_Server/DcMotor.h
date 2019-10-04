class DcMotor
{
private:
  int pin; // gpio pin for motor
  int dir; // direction - forward or reverse
  int pwm_dc; // pwm duty_cycle
  int pwm_freq; // pwm frequency
  int state; // on or off;

public:
  DcMotor(int pin, int dir = 0, int pwm_freq = 0, int pwm_dc = 0);

  int get_pin();
  int get_dir();
  int get_pwm_dc();
  int get_pwm_freq();
  int get_state();

  void set_pin(int pin);
  void set_dir(int dir);
  void set_pwm_dc(int pwm_dc);
  void set_pwm_freq(int pwm_freq);

  int start();
  int stop();
};
