import urllib.request
from pathlib import Path
from dotenv import dotenv_values
import argparse

def fetch(year, day):
    pday = f"0{day}" if (int(day) < 10) else day
    url = f"https://adventofcode.com/{year}/day/{day}/input"
    dir = Path(__file__).parent / str(year) / f"{pday}"
    output = dir / "input.txt"

    if not dir.exists():
        dir.mkdir()

    request = urllib.request.Request(url)
    cookie = dotenv_values(".env")["SESSION"]
    request.add_header("Cookie", f"session={cookie}")

    try:
        with urllib.request.urlopen(request) as response:
            input = response.read().decode("utf-8")
            with open(output, "w") as f:
                f.write(input)

    except urllib.error.HTTPError:
        print("HTTP Error")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("year")
    parser.add_argument("day")
    args = parser.parse_args()

    fetch(args.year, args.day)