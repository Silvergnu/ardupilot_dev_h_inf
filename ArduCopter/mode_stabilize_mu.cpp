#include "Copter.h"

/*
 * Init and run calls for stabilize flight mode
 */

// stabilize_run - runs the main stabilize controller
// should be called at 100hz or more
bool ModeMuStabilize::init(bool ignore_checks)
{
    attitude_control->reset_H_inf();
/*    if (!ignore_checks)
    {
        if (!AP::ahrs().attitudes_consistent())
            return false;
    }*/

    return true;
}

void ModeMuStabilize::run()
{
    // apply simple mode transform to pilot inputs
    update_simple_mode();

    // convert pilot input to lean angles
    float target_roll, target_pitch;
    get_pilot_desired_lean_angles(target_roll, target_pitch, copter.aparm.angle_max, copter.aparm.angle_max);

    // get pilot's desired yaw rate
    float target_yaw_rate = get_pilot_desired_yaw_rate(channel_yaw->get_control_in());

 /*   AP::logger().Write("REFS","TimeUS,RollRef,PitchRef,YawRef","Qfff", AP_HAL::micros64(),
    (double)target_roll,(double)target_pitch,(double)target_yaw_rate);
*/
    if (!motors->armed()) {
        // Motors should be Stopped
        motors->set_desired_spool_state(AP_Motors::DesiredSpoolState::SHUT_DOWN);
    } else if (copter.ap.throttle_zero) {
        // Attempting to Land
        motors->set_desired_spool_state(AP_Motors::DesiredSpoolState::GROUND_IDLE);
    } else {
        motors->set_desired_spool_state(AP_Motors::DesiredSpoolState::THROTTLE_UNLIMITED);
    }
    
    switch (motors->get_spool_state()) {
    case AP_Motors::SpoolState::SHUT_DOWN:
        // Motors Stopped
        attitude_control->reset_H_inf();
        attitude_control->set_yaw_target_to_current_heading();
        attitude_control->reset_rate_controller_I_terms();
        break;

    case AP_Motors::SpoolState::GROUND_IDLE:
        // Landed
        attitude_control->reset_H_inf();
        attitude_control->set_yaw_target_to_current_heading();
        attitude_control->reset_rate_controller_I_terms_smoothly();
        break;

    case AP_Motors::SpoolState::THROTTLE_UNLIMITED:
        // clear landing flag above zero throttle
        if (!motors->limit.throttle_lower) {
            set_land_complete(false);
        }
        break;

    case AP_Motors::SpoolState::SPOOLING_UP:
    case AP_Motors::SpoolState::SPOOLING_DOWN:
        // do nothing
        break;
    }


    count++;
    if(count == 2)
    { 
        float roll =  radians(AP::ahrs().roll_sensor * 0.01f);
        float pitch = radians(AP::ahrs().pitch_sensor * 0.01f);
        float yaw = radians(wrap_360_cd(AP::ahrs().yaw_sensor)*0.01f);
        // call mu-synthesis attitude controller must be executed at a rate of 200 Hz
//        attitude_control->input_euler_angle_roll_pitch_euler_rate_yaw_mu(target_roll, target_pitch, target_yaw_rate, ahrs.get_roll(), ahrs.get_pitch(), ahrs.get_yaw(), get_pilot_desired_throttle());
        attitude_control->input_euler_angle_roll_pitch_euler_rate_yaw_mu(target_roll, target_pitch, target_yaw_rate, roll, pitch, yaw, get_pilot_desired_throttle());

        count = 0;

        AP::logger().Write("RPYC","TimeUS,RollRef,Roll,PitchRef,Pitch,YawRef,Yaw","Qffffff",AP_HAL::micros64(),target_roll,roll,target_pitch,pitch,target_yaw_rate,yaw);
    }


    // get commands to send to motors
    float roll_com, pitch_com, yaw_com, thrust_com;
    attitude_control->get_rpyt_mu(roll_com, pitch_com, yaw_com, thrust_com);
    
    // set commands to the motor object
    motors->set_roll(roll_com);
    motors->set_pitch(pitch_com);
    motors->set_yaw(yaw_com);

    // output pilot's throttle
    attitude_control->set_throttle_out(thrust_com,
                                       true,
                                       g.throttle_filt);


}
