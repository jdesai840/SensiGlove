# SensiGlove

**1st Place Winner of the 2021 Neosensory Developer Contest!**


Full Project Write-up:
https://www.hackster.io/hjones6315/sensiglove-adding-sensory-perception-to-any-prosthetic-arm-56f47f

Video Overview of Project:
https://youtu.be/H9P8Iio1RB4


SensiGlove is a low-cost prosthetic arm sensory feedback system. It uses haptic feedback to allow people with prosthetic arms to have a simulated sense of touch by using haptic feedback.

The system istself is built around a nrF52-based microcontroller. A machine learning algorithm (TFLite based) built around C++ runs on the microcontroller and allows the user to "feel" distinct classes of materials (wood, metal, plastic) differently based on their capacitance and conductivities.

The machine learning model has been exported as a C++ library, and can be found in the ZIP folder. The library can be imported in the Arduino IDE to run this machine learning model on any nRF52 microcontroller. The C++ base code for the base glove is found in "sense.ino".

![Quick Overview](https://cdn.discordapp.com/attachments/795811395002171392/797800958620073984/ezgif.com-gif-maker_1.gif)

Dependent on the Neosensory SDK for Arduino, TFLite for ML model
-Jay
