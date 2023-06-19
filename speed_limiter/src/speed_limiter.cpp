#include <gz/math/SpeedLimiter.hh>
#include <iostream>


using namespace gz;



int main(int argc, char * argv[])
{

  math::SpeedLimiter limiter;

  double maxvel = 200;  // m/s
  double maxacc = 15;    // m/s^2
  double maxjerk = 20;  // m/s^3

  limiter.SetMinVelocity(-maxvel);
  limiter.SetMaxVelocity( maxvel);
  limiter.SetMinAcceleration(-maxacc);
  limiter.SetMaxAcceleration( maxacc);
  limiter.SetMinJerk(-maxjerk);
  limiter.SetMaxJerk( maxjerk);
  
  double vel, prev_vel, prev_prev_vel;
  double velcmd;

  std::chrono::steady_clock::duration dt{10000000};
  const double dtSec = std::chrono::duration<double>(dt).count();
  std::cout << "dt: " << dtSec << std::endl;


  for (int t = 0; t < 1000; t++)
  {
    if (t < 10) {
      velcmd = 0;
    }
    else if (t < 500) {
      velcmd = 10;
    }
    else 
      velcmd = 0;
    
    vel = velcmd;

    limiter.Limit(vel, prev_vel, prev_prev_vel, dt);

    prev_prev_vel = prev_vel;
    prev_vel = vel;

    std::cout << velcmd << "; " << vel << std::endl;
  }

  return 0;
}
