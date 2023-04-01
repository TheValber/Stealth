CC = gcc
LDFLAGS = -lMLV -lm
CFLAGS = -ansi -Wall
OBJPATH = ./bin/
VPATH = ./src/:./include/

.PHONY: Stealth all clean

Stealth: Main.o GUI.o Player.o Gameplay.o Direction.o Map.o Util.o Relics.o Spells.o Enemy.o Game.o Menu.o Data.o Collide.o Leaderboard.o LinkedLink.o
	$(CC) -o $@ $(addprefix $(OBJPATH), $^) $(LDFLAGS)

all: Stealth

Main.o: Main.c Game.h Menu.h

Game.o: Game.c Game.h GUI.h Gameplay.h

Menu.o: Menu.c Menu.h GUI.h Gameplay.h

GUI.o: GUI.c GUI.h Player.h Map.h Util.h Spells.h Enemy.h Data.h Relics.h Gameplay.h Leaderboard.h

Enemy.o: Enemy.c Enemy.h Player.h Gameplay.h Map.h Util.h Relics.h

Gameplay.o: Gameplay.c Gameplay.h Player.h Direction.h Collide.h

Player.o: Player.c Player.h Direction.h Util.h Relics.h

Relics.o: Relics.c Relics.h Map.h Util.h

Collide.o: Collide.c Collide.h Map.h Direction.h Util.h

Map.o: Map.c Map.h Spells.h LinkedLink.h

Data.o: Data.c Data.h

Direction.o: Direction.c Direction.h

Spells.o: Spells.c Spells.h

Leaderboard.o: Leaderboard.c Leaderboard.h

LinkedLink.o: LinkedLink.c LinkedLink.h

Util.o: Util.c Util.h

%.o: %.c
	$(CC) -o $(OBJPATH)$@ -c $< $(CFLAGS)

clean:
	rm -f ./bin/*.o
	rm -f Stealth
