
import os
is_mouse=os.environ.get("mouse")

if not is_mouse:
	from evdev import InputDevice, list_devices, ecodes
	import threading
from pathlib import Path
import pygame
import time
import tkinter as tk
import wave

NOTES = ["DO", "RE", "MI", "FA", "SOL", "LA", "SI", "DOH"]
SOUND_DIR = Path.home() / "games" / "active" / "solfegiu"

DURATION = 1  # seconds
ROW_H = 45
BOTTOM_PAD = 120
ROW_BOX_H = 40
TEXT_Y = 20
SHADOW_Y = 21
CLEANUP_Y=2*ROW_BOX_H

pygame.mixer.init()

SOUNDS = {}
PCM = {}

with wave.open(str(SOUND_DIR / NOTES[0])+".wav", "rb") as w:
	channels = w.getnchannels()
	sample_width = w.getsampwidth()   # bytes/sample
	sample_rate = w.getframerate()
for note in NOTES:
    path = SOUND_DIR / f"{note}.wav"
    SOUNDS[note] = pygame.mixer.Sound(str(path))
    with wave.open(str(path), "rb") as w:
        PCM[note] = w.readframes(w.getnframes())

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
      self.progress = 1 # also used at press and drawings

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
		self.recording = []
		self.duration_state = DURATION

		self.topbar = tk.Frame(self.canvas, bg="black")
		self.topbar.place(relx=0, rely=0, relwidth=1, height=50)

		self.duration_label = tk.Label(
			self.topbar,
			text="0.00 s",
			bg="black",
			fg="white",
			font=("Arial", 24, "bold")
		)
		self.duration_label.place(relx=0.5, rely=0.5, anchor="center")
		self.total_duration = 0.0
		self.active_row = None

		self.button_bar = tk.Frame(self.topbar, bg="black")
		self.button_bar.pack(side="right", padx=10, pady=5)

		self.save_button = tk.Button(
			self.button_bar,
			text="Save as a.wav",
			command=self.save_recording
		)
		self.save_button.pack(side="left", padx=5)

		self.reset_button = tk.Button(
		    self.button_bar,
		    text="Reset",
		    command=self.reset
		)
		self.reset_button.pack(side="left", padx=5)

		self.duration_button = tk.Button(
		    self.button_bar,
		    text="DUR: 1s",
		    command=self.toggle_duration
		)
		self.duration_button.pack(side="left", padx=5)

		self.minimize_button = tk.Button(
		    self.button_bar,
		    text="Minimize",
		    command=self.root.iconify
		)
		self.minimize_button.pack(side="left", padx=5)

		self.exit_button = tk.Button(
			self.button_bar,
			text="Exit",
			command=self.root.destroy
		)
		self.exit_button.pack(side="left", padx=5)

		self.bottom_frame = tk.Frame(root, height=100)
		self.bottom_frame.pack(side="bottom", fill="x")

		self.create_buttons()

		self.root.after(30, self.loop)

	#these are for touches
	def find_touch_device(self):
	    #return InputDevice("/dev/input/event6") this is changing between restarts # xinput list # xinput list-props N
	    for path in list_devices():
	        dev = InputDevice(path)
	        if dev.name == "SYNA7508:00 06CB:12A4":
	            return dev
	    raise RuntimeError("Touch device not found")
	def evdev_loop(self):
		for event in self.touch_device.read_loop():
			# TOUCH DOWN - is one touch delayed, EV_ABS is coming after this
			#if event.type == ecodes.EV_KEY and event.code == 330 and event.value == 1:
			#	self.handle_touch_down()
			if event.type == ecodes.EV_ABS:
				if event.code == ecodes.ABS_X:
					if not self.pressed:
						self.handle_touch_down(event.value)
						self.pressed=True
			# TOUCH UP
			elif event.type == ecodes.EV_KEY and event.code == 330 and event.value == 0:
				self.pressed=False
	def handle_touch_down(self,touch_x):
	    #if not hasattr(self, "touch_x"):
	    #    return
	    # normalize 0..1
	    norm = (touch_x - self.min_x) / (self.max_x - self.min_x)
	    #
	    #norm = max(0.0, min(1.0, norm))
	    index = int(norm * len(NOTES))
	    #if index >= len(NOTES):
	    #    index = len(NOTES) - 1
	    note = NOTES[index]
	    self.root.after(0, lambda: self.press(note))


	def create_buttons(self):
		if not is_mouse:
			#also need to sudo usermod -aG input $USER , else, with sudo audo will not work. again, user must be in 'input' group, test with 'groups' after reboot
			self.touch_device = self.find_touch_device()
			absinfo = self.touch_device.absinfo(ecodes.ABS_X)
			self.min_x = absinfo.min
			self.max_x = absinfo.max
			self.pressed=False
			self.touch_thread = threading.Thread(target=self.evdev_loop, daemon=True)
			self.touch_thread.start()

		for note in NOTES:
			b = tk.Button(
				self.bottom_frame,
				text=note,
				height=4,
				#command=lambda n=note: self.press(n)
			)
			if is_mouse:
				b.bind("<ButtonPress-1>", lambda e, n=note: self.press(n))
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
			real_duration = self.active_row.progress * DURATION
			self.total_duration += real_duration
			self.recording.append(
				(self.active_row.note, real_duration)
			)

			self.active_row = None
			self.duration_label.config(
				text=f"{self.total_duration:.2f} s"
			)

	def save_recording(self):
		print("Saving:")
		print("channels "+str(channels)+", rate "+str(sample_rate)+", width "+str(sample_width))

		pcm_out = bytearray()
		frame_size = channels * sample_width
		for note, duration in self.recording:
			print(note, duration)

			wanted_frames = int(sample_rate * duration)
			pcm = PCM[note]
			pcm_out.extend(
				pcm[:wanted_frames * frame_size]
			)

		with wave.open("a.wav", "wb") as w:
			w.setnchannels(channels)
			w.setsampwidth(sample_width)
			w.setframerate(sample_rate)

			w.writeframes(pcm_out)

	def reset(self):
	    self.duration_update()  # optional, to reset active_row

	    for r in self.rows:
	        self.canvas.delete(r.bg)
	        self.canvas.delete(r.fill)
	        self.canvas.delete(r.text)
	        self.canvas.delete(r.shadow)

	    self.rows.clear()
	    self.recording.clear()

	    self.total_duration = 0.0
	    self.duration_label.config(text="0.00 s")

	def toggle_duration(self):
	    self.duration_state = 2 if self.duration_state == 1 else 1
	    global DURATION
	    DURATION = self.duration_state

	    self.duration_button.config(text=f"DUR: {DURATION}s")

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
