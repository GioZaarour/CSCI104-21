from pathlib import Path

root = Path(__file__).absolute().parent
for folder in ("output",):
    for case_path in root.joinpath(folder).glob("*.*"):
        lines = list(filter(None, case_path.read_text().strip().split("\n")))
        steps = int(lines[0]) - 2
        expansions = lines[1]
        case_path.write_text(f"{steps}\n{expansions}\n")
