import urllib.request
from pathlib import Path
import json
import argparse

def fetch(year, day, cookie):
    pday = f"0{day}" if (int(day) < 10) else day
    dir = Path(__file__).parent / str(year)

    if not dir.exists():
        dir.mkdir()

    dir = dir / f"{pday}"

    if not dir.exists():
        dir.mkdir()

    output = dir / "input.txt"

    url = f"https://adventofcode.com/{year}/day/{day}/input"
    request = urllib.request.Request(url)
    request.add_header("Cookie", f"session={cookie}")

    try:
        with urllib.request.urlopen(request) as response:
            input = response.read().decode("utf-8")
            with open(output, "w") as f:
                f.write(input[:-1:])

    except urllib.error.HTTPError:
        print("HTTP Error")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("year", type=int)
    parser.add_argument("day", type=int)
    args = parser.parse_args()

    config = Path(__file__).parent / "config.json"
    if not config.exists():
        exit("Config file not found")

    session = json.loads(open(config).read()).get("cookie", "")

    if not (2015 <= args.year):
        exit("Year not valid for Advent of Code")

    if not (0 <= args.day <= 25):
        exit("Day not valid for Advent of Code")

    fetch(args.year, args.day, session)