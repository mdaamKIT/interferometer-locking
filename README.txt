
About the project
=================

The purpose of this project is to lock an interferometer to a fixed operating point for a long period of time using not much more than an Arduino. The achieved locking characteristics with this setup are not suitable for use in high precision measurements but could be used for educational purposes. Try using it as a high school project to show some advanced interferometric techniques or basics of electronic control systems.

Except for this README-file, the repository at hand contains two arduino sketches ('locking' and 'pwm_diagnose') details to the hardware to use ('hardware.txt') and a LICENSE on how you can use these sketches. The repo is primarily about the 'locking' sketch which should help you lock your interferometer. 'pwm_diagnose' is an example sketch that may help you to find the hardware you need.


Hardware and Setup
==================

For this project you obviously need an interferometer and a microcontroller (such as an Arduino). Moreover a piezo glued to a mirror, a resistor and a capacitor are used. More details can be found in the file 'hardware.txt'.


Code
====

There is not much to explain about the code found in 'locking' in addition to what is mentioned in the comments therein.

The sensitivity of the two parts of the PI-control, P (proportional) and I (integral), needs to be tuned to the system it controls. This is done by the two floats p and i which are defined in the first few lines of the code. The greater these floats are the more sensible the respective mechanism will react to the measured difference to the operating point.
Tuning these parameters means changing their value inside the code and re-uploading the code to the Arduino. You could set this up more elegant with only minor changes to the code, e.g. by setting the parameters using a potentiometer as a voltage divider on an analog input.

In the file tuning.txt you find hints on how to tune the parameters efficiently.


Contact
=======

If you have any more questions about the contents of this repository or its use, feel free to e-mail me: michael.daam@kit.edu


About locking
=============

Why locking?
------------
In an unstabilized environment, a usual interferometer is sensible enough to react to the slightest environmental changes to a noticeable extent. If not carefully prevented, the most common longterm disturbances are miniscule temperature fluctuations e.g. caused by tiny air currents and changes in stress of the breadboard.
These would prevent us from making precise observations over longer periods of time. The best solution would be avoiding these disturbances by thermal and mechanical isolation but sometimes that is not possible to the desired extend with given ressources. At that point locking comes in handy.

Locking: principle of operation
-------------------------------
The basic principle of locking is to monitor the effects of the disturbances on the interferometer and to counter those effects. You need a photodiode measuring the change in the intensity of the interference pattern at one point and a piezo-electric actuator to move one of the mirrors. If let's say the intensity of the pattern at the measuring point decreases due to some miniscule thermal expansion, you would want to change the voltage of the piezo so it moves the mirror back to a position where the pattern is as it was before the expansion (setpoint).

PID controler
-------------
It is not trivial to determine the exact voltage needed to move the piezo to the desired position. The common way to do this is using a PID controller. It adds up three different corrections: 
P stands for proportional. This term is proportional to the error. With error we mean the difference between the actual intesity at the photodiode and the original intensity one wants to lock to.
I means integral. It reacts to the integral of the error over the most recent past. This helps really pushing the error to zero. Using only the proportional correction one could imagine moving to a stable position with a steady non-zero error and a constant correction.
D is for derivative. It is (anti-)porportional to the derivative of the error. This will damp the correction and can help to prevent overshooting.
The three parts have to be balanced out by weighting them with carefully choosen parameters before adding them up. There is no global optimum for those parameters; they have to be manually tuned to the system under control. (See the tuning.txt file for details about the tuning procedure.)
In our system, we omit the derivative part, but you could easily add it to the code.
Check out the entry on wikipedia for more information: https://en.wikipedia.org/wiki/PID_controller

Extracting signals
------------------
One could ask how it is still possible to extract meaningful signal from a locked instrument. After all, the locking seeks to keep the output constant, no matter the circumstances.
First I want to re-emphasize that our setup is not really suitable for real quantitative measurements. It is just not very sensible. (Although you could try to read out the photodiode with a more sensible instrument parallel to locking it with the Arduino.) There are two ways to get meaningful data from the locked interferometer, depending on the situation:
If the signals you want to measure are changing slower than your locking is, you could just extract the relevant information from the correction. (You may need to calibrate it somehow.) An example could be trying to monitor the expansion of another instrument over the course of a lab session with an interferometer.
If your signals are changing considerably faster than your locking, you could just ignore the fact you are locking the interferometer and still draw your conclusions from the signal of the photodiode. (Maybe after filtering with a high-pass filter to get rid of the error and corrections.) An example could be measuring effects of acoustic waves on our locked interferometer. The locking would still be useful as it guarantees your interferometer to keep the optimal operating point over a long period of time.

Application
-----------
The previous example is where the inspiration to try locking with an Arduino originally came from. In our student lab we teach some principles of gravitational wave detection. We are detecting sound waves with a Michelson interferometer as an analogy experiment.
Locking is also one of the techniques allowing for the extreme precise measurements needed to detect gravitational waves e.g. in the LIGO experiment. Their locking is quick enough to keep the interferometer at its desired operating point at all times and get the information out of the correction (called 'error signal').

