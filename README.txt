
About the project
=================

The purpose of this project is to lock an interferometer to a fixed operating point for a long period of time using not much more than an Arduino. The achieved locking characteristics with this setup are not suitable for use in high precision measurements but could be used for educational purposes. Try using it as a high school project to show some advanced interferometric techniques or basics of electronic control systems.

Except for this README-file, the repository at hand contains two arduino sketches ('locking' and 'pwm_diagnose') details about the hardware to use ('hardware.txt'), about the tuning-routine ('tuning.txt') and about locking in general ('locking.txt') as well as a LICENSE about using, modifying and distributing these sketches. The repository is primarily about the 'locking' sketch which should help you lock your interferometer. 'pwm_diagnose' is an example sketch that may help you to find the hardware you need.


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

