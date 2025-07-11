# SAHAS Arcade

This Project is done by members:
- Aditya Dhananjay Singh
- Atharva Atul Penkar
- Harshil Singh
- Kumar Snehal
- Soham Chakraborty

This is a multiplayer - online and offline game.

The project is a code written in C++ language which contains an arcade consisting of three games - 2048, Air Hockey and Snake. It uses **graphics.h** library to print visuals in a graphics window. All coding was done in DevC++ IDE and code editor and the compiler used was TDM-GCC 4.9.2 32-bit Release. A resources .zip folder is also included which contains images used in the program. All the images are made by the team members themselves.

[Link to Original Project](https://github.com/Atharva-Penkar/Capstone-Project-PDS)

## The concepts used in the program are:

Double Buffering to reduce flickering when printing objects.

Division of one main.cpp file into multiple .h and .cpp files for each individual game.<br>
Air_Hockey is further divided into multiple .h and .cpp for online and offline versions of the game.
<br>

### Air Hockey:
- **Object-Oriented Programming**
  - **Encapsulation and Abstraction -** Usage of structs and classes for different components of game such as Ball, Striker and Goal.
  - **Polymorphism -** Functional overloading of sendData() in client.cpp.
- Elastic Collision of bodies (along with InElastic Coefficient of Restitution and friction used in [Soccer Stars](https://github.com/soham-c04/Soccer-Stars)).
- Counting of score when player scores goal, along with slowed Goal scoring animation.
- Simultaneous keyboard inputs from both players via non-blocking functions (along with mouse inputs in [Soccer Stars](https://github.com/soham-c04/Soccer-Stars)).
- Time counter.
- Resume, reset, settings and exit to main menu functionality.

**Online:**
- Information transmission using **Socket Programming in C++.**
- Authoritative server based approach.
- Syncrhonization of all Objects for both players.
- Usage of delimiters ("-") for information sent from server to clients, to detect multiple overlapped received messages on client side.
- Threading
  - **Server -** One thread for each ongoing game between clients, and deletion of thread after game is finished.
  - **Client -** Main thread which prints the visuals onto the screen and a child thread for receiving messages sent by server and updating all the Objects. This is necessary because otherwise the client gets overloaded from server messages and unable to process them, resulting in game crashes.
- Pairing up of unpaired opponents who are waiting to get paired.
<br>

### 2048:
- Array Rotation
- Addition of similar elements only
- Winning and losing conditions
- Reset and exit to main menu functionality
<br>

### Snake Game
- Movement of snake
- Checking collision of snake with boundary
- Counting and increasing length of snake when it eats food
- Exit to main menu functionality
<br>

## Gameplay

<img src="https://github.com/user-attachments/assets/5229cffe-dce4-4a8b-be02-9fc8741b32c4" width="400"> <img src="https://github.com/user-attachments/assets/75c05971-936e-40af-bf38-441a634a83bb" width="400">

<img src="https://github.com/user-attachments/assets/307368e5-5f64-438d-a2f9-b605cc8a2f92" width="400"> <img src="https://github.com/user-attachments/assets/c451eed5-7c43-417c-97f5-982b47190521" width="400">


$\newline$

$$ - THE \space \space END - $$

