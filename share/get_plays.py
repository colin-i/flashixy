#!/usr/bin/env python3

import os
import sys
import json

path = os.path.join(
    os.environ["HOME"],
    "stats",
    "k",
    sys.argv[1]
)

with open(path, "r", encoding="utf-8") as f:
    data = json.load(f)

print(data["gameplays_count"])
