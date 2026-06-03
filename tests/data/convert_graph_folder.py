#!/usr/bin/env python3
"""
convert_graph_folder.py

Scan a directory of files where each filename contains an integer (the expected circumference)
and the file contains one or more graph6 strings (one per line or whitespace-separated).
Produce a single output file where each line is: <graph6> <expected>

Usage:
  python convert_graph_folder.py <input_dir> <output_file>

If the filename contains multiple numbers, the first is used.
Blank lines and lines starting with '#' are ignored.
"""
import sys
import re
from pathlib import Path

if len(sys.argv) < 3:
    print("Usage: python convert_graph_folder.py <input_dir> <output_file>")
    sys.exit(2)

indir = Path(sys.argv[1])
outfile = Path(sys.argv[2])

if not indir.is_dir():
    print(f"Error: {indir} is not a directory")
    sys.exit(2)

num_re = re.compile(r"(-?\d+)")
# circumference pattern: letter 'c' or 'C' followed by digits, e.g. _c5 or -c10
circ_re = re.compile(r"[cC](\d+)")
# claimed count pattern: digits before '_graphs' like 'list_11_graphs'
count_re = re.compile(r"(\d+)(?=_graphs)")
valid_chars = set(chr(i) for i in range(63, 127)) | {"\n", "\r"}

lines_out = []
strict = False
if '--strict' in sys.argv:
    strict = True

for file in sorted(indir.iterdir()):
    if not file.is_file():
        continue
    # determine circumference from 'cN' pattern first
    c_m = circ_re.search(file.name)
    if c_m:
        expected = c_m.group(1)
    else:
        # fallback: first number in the filename
        m = num_re.search(file.name)
        if not m:
            print(f"Skipping {file.name}: no number found in filename")
            continue
        expected = m.group(1)

    # optional: claimed count in filename like list_11_graphs_c5
    claimed_count = None
    cnt_m = count_re.search(file.name)
    if cnt_m:
        claimed_count = int(cnt_m.group(1))
    tokens = []
    with file.open("r", encoding="utf-8", errors="ignore") as fh:
        for raw in fh:
            s = raw.strip()
            if not s or s.startswith("#"):
                continue
            # split into whitespace-separated tokens; pick tokens that look like graph6
            for token in s.split():
                t = token
                if t.startswith(">"):
                    # allow header tokens starting with '>'
                    pass
                if all((ch in valid_chars or ch == '>') for ch in t):
                    tokens.append(t)

    if not tokens:
        print(f"Skipping {file.name}: no graph tokens found")
        continue

    # if a claimed count exists, check it matches actual number of tokens
    actual_count = len(tokens)
    if claimed_count is not None and claimed_count != actual_count:
        msg = f"Count mismatch in {file.name}: claimed={claimed_count} actual={actual_count}"
        if strict:
            print("ERROR: " + msg)
            sys.exit(1)
        else:
            print("WARNING: " + msg + " — skipping file")
            continue

    for t in tokens:
        lines_out.append(f"{t} {expected}")

if not lines_out:
    print("No graphs found.")
    sys.exit(1)

with outfile.open("w", encoding="utf-8") as fh:
    fh.write("\n".join(lines_out))

print(f"Wrote {len(lines_out)} entries to {outfile}")
