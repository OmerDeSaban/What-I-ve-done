Contributors:
Omer De-Saban - 315635441
Ariel Haim - 314615261

In order to execute do:
make
./tanks_game {input file name}

Input format:
rows cols
grid view of board:
1 - tankA
2 - tankB
# - wall
@ - mine

example:
10 8
##########
#        #
# 1    2 #
#        #
#   @    #
#        #
#   #    #
##########

Output format:
tankA action: {action}
tankB action: {action}
...
Game Over! tie/tankA won/tankB won
Cause of Death: ...
