# Grass√s Battlebot
> Down, Up, Up

## Who are we?
Grass√s is a team of teenagers building a 150g battlebot. We strive to bring performance at a good cost. It can be seen in the chices made in the design of our robot

## Background
We made this team because we thought battlebots was a fun game. As we started to plan, we found that there are many hurdles to make a good bot. Those hurdles shaped us into the team we are now.

## 3D Design

## EDA
- Robot Controller

    Using the ESP32-C3 Super Mini, we get signal from the tx and distribute to 2 Brushed and 1 Brushless ESC each with a motor for wheels and the spinner respectively.

- Ground Station

    Using an ESP32-S2 LOLIN Board, we capture the input from a gamepad plugged in.

## Programming
We use the Arduino SDK for ESP32 to connect the esp32-s2 ground station to the esp32-c3 robot control. we use esp-now to achieve a fairly good rc link.