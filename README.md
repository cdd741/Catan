# Catan

A simple version of the board game Settlers of Catan

### Prerequisite

 You need to run under Linux with the support of XWindow.

 ### Make and Run

 Type

 ```
 make
 ```

 and

 ```
 ./main
 ```

 to run the program

### Game Play

#### Beginning of Game 

At the beginning of the game, each builder chooses two initial basements to built.

#### Beginning of Turn 

At the beginning of a turn, a builder can then enter any of the following three commands: 

• load: sets the dice of the current builder to be loaded dice 

• fair: sets the dice of the current builder to be fair dice 

• roll: rolls the current builder’s dice. 

This ends the "Beginning of the turn" phase and moves the builder to "During the turn".

#### During the Turn 

During the turn, a builder can input any of the following commands: 

• board: prints the current board 

• status: prints the current status of all builders in order from builder 0 to 3 

• residences: prints the residences the current builder has currently completed 

• build-road <road#>: attempts to builds the road at <road#> 

• build-res <housing#>: attempts to builds a basement at <housing#> 

• improve <housing#>: attempts to improve the residence at <housing#> 

• trade <colour> <give> <take>: attempts to trade with builder <colour> giving one resource of type <give> and receiving one resource of type <take> 

• next: passes control onto the next builder in the game. This ends the "During the Turn" phase.

• save <file>: saves the current game state to <file> 

• help: prints out the list of commands 

#### End of Game 

The game ends when a builder has a total of 10 building points. The ﬁrst builder to have 10 building points is the winner.
