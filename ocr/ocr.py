import easyocr
import pyautogui
import time
import torch
import os
import numpy as np
import ctypes
import mmap
import sys
from rapidfuzz import fuzz, process # type: ignore

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

# Known weapon names and acceptable variants
weapon_aliases = {
    "416C": ["416c", "416 C", "416-c", "416-C CARBINE", "416-€ CARBINE", "416-C CARBINEE", "416-@ CARBINE", "416"],
    "C8 SFW": ["c8 sfw", "c8-sfw", "c0 sfw", "c0-sfw", "c8", "c0"],
    "CAMRS": ["camrs", "cam-rs"],
    "MK17 CQB": ["mk17", "mk 17", "mk17 cqb", "MKT7 CQB", "MK 17 CQB", "MKT 7 CQB", "MKI7 CQB", "MK I7 CQB", "MK7 CQB"],
    "SR 25": ["sr25", "sr-25", "SR 25"],
    "Super 90": ["super90", "super 90", "s90", "super"],
    "SPAS 12": ["spas12", "spas-12"],
    "Commando 9": ["commando9", "commando-9", "COMMANDO", "COMMANDO 9"],
    "Mk1 9mm": ["mk1", "mk1 9mm", "mk1-9mm", "MK 1", "MK I", "MKI"],
    "Deagle": ["D-50", "D50", "D 50"]
}

# Create alias lookup table
alias_lookup = []
for canonical, aliases in weapon_aliases.items():
    for alias in aliases:
        alias_lookup.append((alias.lower().replace(" ", "").replace("-", ""), canonical))

def normalize(text):
    return text.lower().replace(" ", "").replace("-", "")

def match_weapon_name(ocr_text):
    norm_input = normalize(ocr_text)
    candidates = [a[0] for a in alias_lookup]
    match, score, _ = process.extractOne(norm_input, candidates, scorer=fuzz.ratio)
    if score >= 80:
        for alias, canonical in alias_lookup:
            if alias == match:
                return canonical
    return None

print(f"[INFO] Starting OCR loop (GPU: {'Yes' if use_gpu else 'No'})... Press Ctrl+C to stop.")

last_sent_name = ""  # Track last weapon name sent

try:
    while True:
        screenshot = pyautogui.screenshot(region=region)
        image_np = np.array(screenshot)
        results = reader.readtext(image_np)

        os.system('cls' if os.name == 'nt' else 'clear')

        matched = None
        if results:
            text = results[0][1]
            print(f"[RAW OCR] {text} ({results[0][2]:.2f})")
            matched = match_weapon_name(text)

        if matched:
            print(f"[MATCHED] {matched}")
            if matched != last_sent_name:
                flags = InternalFlags()
                text_bytes = matched.encode('utf-8')[:63]
                flags.weaponName = text_bytes + b'\x00' * (64 - len(text_bytes))
                mem.seek(0)
                mem.write(bytes(flags))
                last_sent_name = matched
            else:
                print("[INFO] Match unchanged, not writing to memory.")
        else:
            print("[IGNORED] No valid match")
            if last_sent_name != "":
                # Send empty string to clear last entry
                flags = InternalFlags()
                flags.weaponName = b'\x00' * 64
                mem.seek(0)
                mem.write(bytes(flags))
                last_sent_name = ""

        time.sleep(1)

except KeyboardInterrupt:
    print("\n[INFO] OCR loop stopped by user.")
finally:
    mem.close()
