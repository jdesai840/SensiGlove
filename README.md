# SensiGlove

**1st Place Winner of the 2021 Neosensory Developer Contest!**

SensiGlove is a low-cost prosthetic arm sensory feedback system. It uses haptic feedback to allow people with prosthetic arms to have a simulated sense of touch by using haptic feedback.

The system istself is built around a nrF52-based microcontroller. A machine learning algorithm (built around TFLite) built around C++ runs on the micronctorller and allows the user to "feel" distinct classes of materials (wood, metal, plastic) differently based on their capacitance.

The C++ machine learning model can be found in the associated "ML" folder. The C++ code for the base glove itself is found in "sense.ino".

![Quick Overview](https://cdn.discordapp.com/attachments/795811395002171392/797800958620073984/ezgif.com-gif-maker_1.gif)

Dependent on the Neosensory SDK for Arduino, TFLite for ML model
-Jay
