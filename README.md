# CY-Connect

CY-Connect is a C terminal game inspired by Connect 5.

## Build

```sh
make
```

## Run

```sh
./cyconnect
```

or:

```sh
make run
```

## Files

- `main.c`: program entry point and main menu.
- `board.c` / `board.h`: board allocation, initialization, piece drop and gravity.
- `game.c` / `game.h`: game loop, rotations and win detection.
- `input.c` / `input.h`: secure integer input.
- `ui.c` / `ui.h`: terminal display, colors and screen refresh.
- `save.c` / `save.h`: save and load system.

## Notes

Run the program in a real terminal to see ANSI colors.
The VS Code "Output" panel may display text without interpreting colors.
