from curricula_grade.shortcuts import *
from curricula_grade.setup.common import check_file_exists
from curricula_grade.test.correctness.common import ProcessCompareStreamTest
from curricula_grade_cpp.setup.common.makefile import check_makefile_exists, makefile_execute
from curricula.library.files import delete_file
from pathlib import Path

root = Path(__file__).absolute().parent
grader = Grader()

GPP = "-std=c++11",


@grader.register(graded=False, tags={"sanity"})
def check_doublet_makefile(submission: Submission) -> SetupResult:
    """Assert a Makefile is present in the target."""

    return check_makefile_exists(submission.problem_path)


@grader.register(graded=False, tags={"sanity"}, passing={"check_doublet_makefile"})
def build_doublet(submission: Submission) -> SetupResult:
    """Run the Makefile."""

    return makefile_execute(submission.problem_path, timeout=10)


@grader.register(graded=False, tags={"sanity"}, passing={"build_doublet"})
def check_doublet(submission: Submission, resources: dict) -> SetupResult:
    """Check whether the binary exists."""

    doublet_path = submission.problem_path.joinpath("doublet")
    resources["doublet"] = ExecutableFile(doublet_path)
    return check_file_exists(doublet_path)


class DoubletTest(ProcessCompareStreamTest):
    executable_name = "doublet"
    timeout = 5
    out_transform = staticmethod(bytes.strip)


test_args = (
    ("head", "tail", root.joinpath("words", "words1.txt"), root.joinpath("output", "case1.txt")),
    ("ape", "man", root.joinpath("words", "words2.txt"), root.joinpath("output", "case2.txt")),
    ("flour", "bread", root.joinpath("words", "words3.txt"), root.joinpath("output", "case3.txt")),
    ("one", "two", root.joinpath("words", "words4.txt"), root.joinpath("output", "case4.txt")),
    ("black", "white", root.joinpath("words", "words5.txt"), root.joinpath("output", "case5.txt")),
    ("blue", "pink", root.joinpath("words", "words6.txt"), root.joinpath("output", "case6.txt")),
    ("river", "shore", root.joinpath("words", "words7.txt"), root.joinpath("output", "case7.txt")),
    ("witch", "fairy", root.joinpath("words", "words8.txt"), root.joinpath("output", "case8.txt")),
    ("sleep", "dream", root.joinpath("words", "words9.txt"), root.joinpath("output", "case9.txt")),
    ("river", "gotee", root.joinpath("words", "words10.txt"), root.joinpath("output", "case10.txt")))


for i, (fr, to, read, write) in enumerate(test_args, start=1):
    grader.register(name=f"case{i}", passing={"check_doublet"})(DoubletTest(
        args=(fr, to, str(read)),
        test_out_lines=write.read_bytes().splitlines(keepends=False),
    ))


@grader.register[CleanupResult](passing={"check_doublet"})
def clean_learning(doublet: ExecutableFile):
    """Delete the binary."""

    delete_file(doublet.path)
