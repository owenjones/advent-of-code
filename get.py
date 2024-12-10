import urllib.request
from pathlib import Path
import json
import argparse

def fetch(year, day, cookie):
    pday = f"0{day}" if (int(day) < 10) else day
    url = f"https://adventofcode.com/{year}/day/{day}/input"
    dir = Path(__file__).parent / str(year) / f"{pday}"
    output = dir / "input.txt"

    request = urllib.request.Request(url)
    request.add_header("Cookie", f"session={cookie}")

    try:
        with urllib.request.urlopen(request) as response:
            input = response.read().decode("utf-8")
            with open(output, "w") as f:
                if not dir.exists():
                    dir.mkdir()

                f.write(input[:-1:])

    except urllib.error.HTTPError:
        print("HTTP Error")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("year")
    parser.add_argument("day")
    args = parser.parse_args()

    if not (0 <= int(args.day) <= 25):
        exit("Day not valid for Advent of Code")

    if not (2015 <= int(args.year)):
        exit("Year not valid for Advent of Code")

    config = Path(__file__).parent / "config.json"

    if not config.exists():
        exit("Config file not found")

    session = json.loads(open(config).read()).get("cookie", "")

    fetch(args.year, args.day, session)