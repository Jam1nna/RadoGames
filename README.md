# RadoGames
### *A collection of broken games*

Built in **C** with **raylib**. No engine. No excuses.

These games work — just not correctly. The rules are off. Something familiar is slightly wrong in a way you can't quite ignore.

---

## Games

| Game | What's wrong |
|------|-------------|
| 🐍 **Snake** | Eating food makes you shorter |
| 💣 **MineSweeper** | The numbers are wrong |
| ❌ **TikTakToe** | X always loses. Always. |
| 🪢 **Hangman** | Correct guesses add a body part |

---

## Build & Run

Requires [raylib](https://www.raylib.com/) installed.

```bash
git clone https://github.com/Jam1nna/RadoGames
cd RadoGames/<game-name>
gcc main.c -lraylib -lm -o game && ./game
```

---

## Stack

- **C** — no engine
- **raylib** — window, input, drawing
- intentionally wrong

---

*© RadoGames — all wrongs reserved*
