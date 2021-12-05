from curricula_grade.shortcuts import *
from curricula_grade.setup.common import check_file_exists
from curricula_grade.test.correctness.common import *
from curricula_grade_cpp.setup.common.makefile import *
from curricula.library.files import delete_file

from pathlib import Path

root = Path(__file__).absolute().parent
grader = Grader()


@grader.register(tags={"sanity"}, graded=False)
def check_makefile(submission: Submission) -> SetupResult:
    """Check there is a Makefile in the submission."""

    return check_makefile_exists(submission.problem_path)


@grader.register(tags={"sanity"}, graded=False, passing={"check_makefile"})
def make_interpreter(submission: Submission) -> SetupResult:
    """Make sure we can build permutations.cpp with our own main function."""

    return makefile_execute(submission.problem_path, timeout=10)


@grader.register(tags={"sanity"}, graded=False, passing={"make_interpreter"})
def check_interpreter(submission: Submission, resources: dict) -> SetupResult:
    """Check the interpreter executable has been created."""

    path = submission.problem_path.joinpath("interpreter")
    resources["interpreter"] = ExecutableFile(path)
    return check_file_exists(path)


class InterpreterTest(ProcessCompareStreamTest):
    """Pretty much the same as startup companies."""

    # Adding this because the resources are designed poorly
    prefix = "Enter BASIC program file name: "

    executable_name = "interpreter"
    timeout = 10.0
    out_transform = staticmethod(bytes.strip)

    def __init__(self, case_name: str):
        super().__init__(
            stdin=(str(root.joinpath("in", f"{case_name}.txt")) + "\n").encode(),
            test_out_lines=(
                self.prefix.encode() +
                root.joinpath("out", f"{case_name}.txt").read_bytes()).strip().splitlines(keepends=False))

    @classmethod
    def register(cls, case_name: str, description: str = "test interpreter", tags: set = None):
        grader.register(
            name=f"test_{case_name}",
            description=description,
            passing={"check_interpreter"},
            tags=tags,
        )(InterpreterTest(case_name=case_name))


CASES = (
    "example1",
    "example2",
    "example3",
    "end",
    "gosub",
    "goto",
    "if",
    "let",
    "let_print",
    "math",
    "math_other",
    "no_let",
    "null",
    "printall")


for i, case in enumerate(CASES):
    InterpreterTest.register(case, tags={"sanity"} if i < 3 else set())


@grader.register[CleanupResult](tags={"sanity"}, graded=False, passing={"check_interpreter"})
def cleanup_interpreter(interpreter: ExecutableFile):
    """Delete interpreter."""

    # delete_file(interpreter.path)
