
from pathlib import Path
import pygame
import time
import tkinter as tk

NOTES = ["DO", "RE", "MI", "FA", "SOL", "LA", "SI", "DOH"]
SOUND_DIR = Path.home() / "games" / "active" / "solfegiu"

DURATION = 2  # seconds
ROW_H = 45
BOTTOM_PAD = 120
ROW_BOX_H = 40
TEXT_Y = 20
SHADOW_Y = 21
CLEANUP_Y=2*ROW_BOX_H

pygame.mixer.init()

SOUNDS = {
    note: pygame.mixer.Sound(
        str(SOUND_DIR / f"{note}.wav")
    )
    for note in NOTES
}

class Row:
    def __init__(self, canvas, note):
        self.canvas = canvas
        self.note = note
        self.start = time.time()
        self.progress = 0.0
        #y is set later

        self.bg = canvas.create_rectangle(
            0, 0, 800, ROW_BOX_H,
            fill="#222", outline=""
        )

        self.fill = canvas.create_rectangle(
            0, 0, 0, ROW_BOX_H,
            fill="#00ff66", outline=""
        )

        # optional readability enhancement (fake shadow)
        self.shadow = canvas.create_text(
            0, SHADOW_Y,
            text=note,
            fill="black",
            font=("Arial", 14, "bold"),
            anchor="center"
        )

        # center x will be set in layout, but we create placeholder
        self.text = canvas.create_text(
            0, TEXT_Y,
            text=note,
            fill="white",
            font=("Arial", 14, "bold"),
            anchor="center"
        )

    def update(self, width):
     t = time.time() - self.start
     self.progress = t / DURATION
     b = False
     if self.progress >= 1:
      b = True
      self.progress = 1 # also used at press recalculations

     self.canvas.coords(
        self.fill,
        0,
        self.y,
        width * self.progress,
        self.y + ROW_BOX_H
     )
     return b

class App:
    def __init__(self, root):
        self.root = root
        self.root.attributes("-fullscreen", True)

        self.canvas = tk.Canvas(root, bg="black")
        self.canvas.pack(fill="both", expand=True)

        self.rows = []

        self.bottom_frame = tk.Frame(root, height=100)
        self.bottom_frame.pack(side="bottom", fill="x")

        self.create_buttons()

        self.duration_text = self.canvas.create_text(
         100, 20,
         text="0.00 s",
         fill="white",
         font=("Arial", 24, "bold"),
         anchor="n"
        )
        self.total_duration = 0.0
        self.active_row = None

        self.root.after(30, self.loop)

    def create_buttons(self):
        for note in NOTES:
            b = tk.Button(
                self.bottom_frame,
                text=note,
                height=4,
                command=lambda n=note: self.press(n)
            )
            b.pack(side="left", fill="x", expand=True)

    def press(self, note):
     SOUNDS[note].play()

     # create new active row
     new_row = Row(self.canvas, note)  # y will be recalculated

     self.duration_update()
     self.active_row = new_row

     self.rows.append(new_row)

     # re-layout EVERYTHING

     h = self.canvas.winfo_height()
     w = self.canvas.winfo_width()

     base_y = h - BOTTOM_PAD
     cx = w / 2

     for i, r in enumerate(self.rows):
        y = base_y - (len(self.rows) - 1 - i) * ROW_H

        r.y = y #for update, and with this use also for cleanup

        # full width background
        self.canvas.coords(r.bg, 0, y, w, y + ROW_BOX_H)

        # full width progress fill
        self.canvas.coords(r.fill, 0, y, w * r.progress, y + ROW_BOX_H)

        # centered text
        self.canvas.coords(r.shadow, cx + 1, y + SHADOW_Y)
        self.canvas.coords(r.text, cx, y + TEXT_Y)

     if self.rows[0].y < CLEANUP_Y:
      r = self.rows.pop(0)

      self.canvas.delete(r.bg)
      self.canvas.delete(r.fill)
      self.canvas.delete(r.text)
      self.canvas.delete(r.shadow)

    def duration_update(self):
         if self.active_row:
          self.total_duration += self.active_row.progress * DURATION
          self.active_row = None
          self.canvas.itemconfig(
           self.duration_text, text=f"{self.total_duration:.2f} s"
          )

    def loop(self):
     w = self.canvas.winfo_width()

     # update only active row (fast)
     if self.rows:
        if self.rows[-1].update(w):
         self.duration_update()

     self.root.after(100, self.loop)


if __name__ == "__main__":
    root = tk.Tk()
    app = App(root)
    root.mainloop()
