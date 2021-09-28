#ifndef Attitude_HINF_HPP
#define Attitude_HINF_HPP
// Attitude H-infinity Control Class
// Three axis attitude control for quad-rotor
#include <AP_Math/SimpleMatrix.h>
class AInf2{
private:    // Variables arranged in roll pitch yaw order



    float uk[3];    /* Desired moments (Mx,My,Mz) */
    const float ct = 2.0025e-6; /* Thrust coefficient (N/(rad/s)^2)*/
    const float cd = 5.2007e-8; /* Drag coefficient (N.m/(rad/s)^2)*/
    const float l  = 0.12;  /*Length from center of mass to motor in x and y directions */

    bool shouldInitYaw;
    float InitialYaw; // variable to start with zero intial yaw
    float InitialYawRef; // variable to start with zero intial yaw reference
    float yawPrev, yawRefPrev; // previous yaw and yaw references

    float yawRefShift, yawShift; // yaw shift and yaw reference shift ( to avoid discontuity between 0 & 2 pi )
    float yawAct, pitchAct, rollAct; // stores angles used by controller

    SimpleMatrix<float> A_Inf; /* H-infinity controller A Matrix */
    SimpleMatrix<float> B_Inf; /* H-infinity controller B Matrix */
    SimpleMatrix<float> C_Inf; /* H-infinity controller C Matrix */
    SimpleMatrix<float> D_Inf; /* H-infinity controller D Matrix */

    SimpleMatrix<float> X_Inf; /* H-infinity Controller States */
    SimpleMatrix<float> Y_Inf; /* Output Vector from H-infinity controller (Normalized Torques)*/
    SimpleMatrix<float> U_Inf; /* Input Vector to H-infinity Controller (Normalized references and measurements)*/

    SimpleMatrix<float> A_L; /* Lead controller A Matrix */
    SimpleMatrix<float> B_L; /* Lead controller B Matrix */
    SimpleMatrix<float> C_L; /* Lead controller C Matrix */
    SimpleMatrix<float> D_L; /* Lead controller D Matrix */

    SimpleMatrix<float> X_L; /* Lead Controller States */
    SimpleMatrix<float> Y_L; /* Output Vector from lead controller (Normalized Torques)*/
    SimpleMatrix<float> U_L; /* Input Vector to lead controller (Normalized measurements)*/



    SimpleMatrix<float> Y_Total; /*Total Normalized Torque Vector*/

    SimpleMatrix<float> C; /* Torque and force to (rad/s)^2*/
    SimpleMatrix<float> F; /* Torque and force vector*/
    SimpleMatrix<float> W; /* angular velocity squared vector*/

    SimpleMatrix<float> omega;// angular velocity vector
    float w2PWM = 6e-4;
    SimpleMatrix<float> PWM; // PWM vector

    SimpleMatrix<float> PWM2RPYT; // conversion matrix from PWM to rpyt
    SimpleMatrix<float> rpyt; // roll,pitch,yaw,thrust vector for ardupilot

public:
    AInf2();
    void updateController(float roll,float rollRef,float pitch,float pitchRef,float yaw,float yawRef);
    void updateRPYT(float thrustRef);
    void resetController();
    void setYawInit(float initVal);
    float getRoll();
    float getPitch();
    float getYaw();
    float getThrottle();

    float getRollAct();
    float getPitchAct();
    float getYawAct();

};
#endif // Attitude_HINF_HPP



