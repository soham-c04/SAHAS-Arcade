# SAHAS Arcade

This Project is done by members:
- Aditya Dhananjay Singh
- Atharva Atul Penkar
- Harshil Singh
- Kumar Snehal
- Soham Chakraborty

This is a **multiplayer - online and offline** game.

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
- Usage of delimiters ('$') for information sent from server to clients, to detect multiple overlapped received messages on client side.
- Threading
  - **Server -** One thread for each ongoing game between clients, and deletion of thread after game is finished.
  - **Client -** Main thread which prints the visuals onto the screen and a child thread for receiving messages sent by server and updating all the Objects. This is necessary because otherwise the client gets overloaded from server messages and unable to process them, resulting in game crashes.
- Pairing up of unpaired opponents who are waiting to get paired.

### 2048:
- Array Rotation
- Addition of similar elements only
- Winning and losing conditions
- Reset and exit to main menu functionality

### Snake Game
- Movement of snake
- Checking collision of snake with boundary
- Counting and increasing length of snake when it eats food
- Exit to main menu functionality
<br>

# How to play:-

**Note:-** Make sure to have the resources folder in the same directory as "SAHAS-Arcade.exe".

## Air Hockey
- The controls for the player on the left side are - 'W', 'A', 'S', 'D'. The right side player controls are - UP, DOWN, LEFT and RIGHT arrow keys.
- The aim is to score more goals than your opponent.
- The game ends after 90-sec and whoever scores more goals in that time wins, or ends in a draw.

**Online:-**
1. Start server.exe and enter port_no for server.
2. Set the Gamemode to **Online**.
3. Enter ip of server in which server.exe was started, then enter the port no.
4. Wait for another player to join and matchmaking.

<img width="400" src="https://github.com/user-attachments/assets/54411ff8-bc40-4770-bc77-2c2aa3d353fa">
<img width="400" src="https://github.com/user-attachments/assets/6bb5e877-2ff3-49f2-a2a8-781b49b63dec">

### Main Menu:
Choose gamemode - Online or Offline or Quit to Home Screen.

<img width="400" src="https://github.com/user-attachments/assets/e13434e6-78e4-4660-ad46-70461135665f">


### Settings:
Choose the colors of Strikers.

<img width="400" src="https://github.com/user-attachments/assets/1619baab-f48f-4d31-b1d7-0cf0f6de2a22">
<img width="400" src="https://github.com/user-attachments/assets/eca1a608-01fe-49f8-95cb-7e32993c8561">


### Pause Screen (only for Offline mode):
- Pressing Escape between the game will pause the game and a dialogue box with menu options will be shown.
- Pressing Escape again or pressing enter on resume option will resume the game again.
- Options can be switched using arrow keys and to choose an option Enter Key is pressed.
- **Reset** - restarts the whole game including goals, initial position of strikers, ball and Timer.
- **Settings** - open the settings tab to change colors in between the game.
- **Quit** - goes back to the Main Menu page.

<img width="250" src="https://github.com/user-attachments/assets/4a2c87b3-64de-44ea-926c-d89f32df2a13">
<img width="250" src="https://github.com/user-attachments/assets/6c7c4f5c-c280-49a3-bcf4-93e15abae522">
<img width="250" src="https://github.com/user-attachments/assets/fbe075f5-b6f9-4626-a77a-3171dfe3a25a"><br><br>

## 2048
- The controls are - UP, DOWN, LEFT and RIGHT arrow keys.
- Pressing an ARROW key moves all the numbers to that side of board and then generates a new random number.
- Rules are same as that of [Original 2048](https://2048game.com/).

### Pause Screen:
- Pressing Escape between the game will pause the game and a dialogue box with menu options will be shown.
- Pressing Escape again will resume the game again.
- Options can be switched using arrow keys and to choose an option Enter Key is pressed.
- **Reset** - restarts the whole game score and board.
- **Quit** - goes back Home Screen.

<img width="300" src="https://github.com/user-attachments/assets/b3aad431-0080-4a09-8926-c7c3b0f07b05">
<img width="300" src="https://github.com/user-attachments/assets/c5b824c6-23f3-421b-a1f7-fb879d46a61a"><br><br>

### End Screen:
- If you manage to make the number 2048 then you win and a Menu to continue or not appears.
- If all squares are occupied before making the number 2048 and there are no more moves left then option appears to Reset or Quit to Home screen.

<img width="250" src="https://github.com/user-attachments/assets/a82602bc-00a2-433b-92af-05f1f2ae8d40">
<img width="250" src="https://github.com/user-attachments/assets/86dca166-297e-4453-91c2-850ba3a9b85c">
<img width="250" src="https://github.com/user-attachments/assets/abd69044-ffd2-46a6-ac99-db8c27b0184d"><br><br>

## Snake Game
- The controls are - UP, DOWN, LEFT and RIGHT arrow keys.
- The aim is to score as many points by increasing the length of your snake.
- The game ends if you bump into yourself or by pressing Escape.

<img width="400" src="https://github.com/user-attachments/assets/d7bc4c62-6cf0-44e9-9f4e-879cadc4623f"><br><br>

# Gameplay

### Home Screen
<img src="https://github.com/user-attachments/assets/5229cffe-dce4-4a8b-be02-9fc8741b32c4" width="400">
<br>

### Air Hockey
<img width="400" src="https://github.com/user-attachments/assets/b495f3a8-c58b-45bc-b1e1-35a1eb5fd6c4">
<img src="https://github.com/user-attachments/assets/307368e5-5f64-438d-a2f9-b605cc8a2f92" width="400"> 

<table>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/b9e1b921-99b3-4966-ba92-6fad0c21da10" width="300"></td>
    <td><img src="https://github.com/user-attachments/assets/c1ab815f-543a-4108-b544-ba596eefb9c3" width="300"></td>
    <td rowspan="2" align="center" valign="middle">
      <div align="center">
        <strong>Offline</strong><br><br><br>
        <img src="https://github.com/user-attachments/assets/db201711-ce2d-40f9-90c1-6d22c360a852" width="300"><br><br><br>
        <strong>Online</strong>
      </div>
    </td>
  </tr>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/e8637231-5a3f-42f2-9bd5-dd26dc7d2da2" width="300"></td>
    <td><img src="https://github.com/user-attachments/assets/45ec93ae-bcdb-494c-a99b-c10e01d1c591" width="300"></td>
  </tr>
</table>
<br>

### 2048
<img src="https://github.com/user-attachments/assets/75c05971-936e-40af-bf38-441a634a83bb" width="400">
<img width="400" src="https://github.com/user-attachments/assets/19dc24cf-b6df-4f90-ade1-7dc6536d3447">
<br>

### Snake Game
<img src="https://github.com/user-attachments/assets/c451eed5-7c43-417c-97f5-982b47190521" width="400">
<br><br>

$$ **- THE \space \space END -** $$
