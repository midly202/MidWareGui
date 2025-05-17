import easyocr
import pyautogui
import time
import torch
import os
import numpy as np
import ctypes
import mmap
import sys

# Define the C-compatible structure
class InternalFlags(ctypes.Structure):
    _fields_ = [
        ("glowESP", ctypes.c_bool),
        ("runShoot", ctypes.c_bool),
        ("boltScript", ctypes.c_bool),
        ("infGadget", ctypes.c_bool),
        ("goOutside", ctypes.c_bool),
        ("exit", ctypes.c_bool),
        ("_pad", ctypes.c_ubyte * 2),
        ("runShootAddy1", ctypes.c_uint64),
        ("runShootAddy2", ctypes.c_uint64),
        ("weaponName", ctypes.c_char * 64)
    ]

# Try to open the existing shared memory created by MidWareExternal
try:
    mem = mmap.mmap(
        -1,
        ctypes.sizeof(InternalFlags),
        tagname="MySharedMemory",
        access=mmap.ACCESS_WRITE
    )
except Exception as e:
    print(f"[ERROR] Failed to open shared memory: {e}")
    sys.exit(1)

# OCR setup
use_gpu = torch.cuda.is_available()
reader = easyocr.Reader(['en'], gpu=use_gpu)

region = (1550, 1000, 250, 30)

print(f"[INFO] Starting OCR loop (GPU: {'Yes' if use_gpu else 'No'})... Press Ctrl+C to stop.")

try:
    while True:
        screenshot = pyautogui.screenshot(region=region)
        image_np = np.array(screenshot)
        results = reader.readtext(image_np)

        os.system('cls' if os.name == 'nt' else 'clear')

        # Read the first detected string and write it to shared memory
        if results:
            text = results[0][1]  # Get just the text
            print(f"{text} ({results[0][2]:.2f})")

            # Create struct and write to shared memory
            flags = InternalFlags()

            # Properly assign encoded text to fixed-size C char array
            text_bytes = text.encode('utf-8')[:63]
            flags.weaponName = text_bytes + b'\x00' * (64 - len(text_bytes))


            mem.seek(0)
            mem.write(bytes(flags))

        else:
            print("[INFO] No text detected.")

        time.sleep(1)

except KeyboardInterrupt:
    print("\n[INFO] OCR loop stopped by user.")
finally:
    mem.close()
