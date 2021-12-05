from curricula_grade.shortcuts import *
from curricula_grade.setup.common import check_file_exists
from curricula_grade.test.correctness.common import *
from curricula_grade_cpp.setup.common.gpp import *
from curricula.library.files import delete_file

from pathlib import Path

GPP_OPTIONS = ("-std=c++17",)


root = Path(__file__).absolute().parent
grader = Grader()


@grader.register(tags={"sanity"}, graded=False)
def check_permutations_source(submission: Submission, resources: dict) -> SetupResult:
    """Check permutations.cpp is in the submission folder."""

    resources["permutations_source_path"] = path = submission.problem_path.joinpath("permutations.cpp")
    return check_file_exists(path)


@grader.register(tags={"sanity"}, graded=False, passing={"check_permutations_source"})
def build_permutations_main(submission: Submission, permutations_source_path: Path, resources: dict) -> SetupResult:
    """Make sure we can build permutations.cpp with our own main function."""

    result, resources["permutations"] = gpp_compile_object(
        root.joinpath("main.cpp"),
        permutations_source_path,
        destination_path=submission.problem_path.joinpath("permutations_main"),
        gpp_options=(f"-I{submission.problem_path}", *GPP_OPTIONS),
        timeout=5)
    return result


class PermutationsTest(ProcessCompareStreamTest):
    """Pretty much the same as startup companies."""

    matcher = staticmethod(lines_match_unordered)
    executable_name = "permutations"
    timeout = 10.0
    out_transform = staticmethod(bytes.strip)

    def __init__(self, case_name: str):
        super().__init__(
            args=(root.joinpath("input", f"{case_name}.txt").read_text().strip(),),
            test_out_lines=root.joinpath("output", f"{case_name}.txt").read_bytes().strip().splitlines(keepends=False))

    @classmethod
    def register(cls, case_name: str, description: str = "test startup company"):
        grader.register(
            name=f"test_{case_name}",
            description=description,
            passing={"build_permutations_main"}
        )(PermutationsTest(case_name=case_name))


for i in range(1, 10 + 1):
    PermutationsTest.register(case_name=f"case{i}")


@grader.register[CleanupResult](tags={"sanity"}, graded=False, passing={"build_permutations_main"})
def cleanup_permutations_main(permutations: ExecutableFile):
    """Delete permutations."""

    delete_file(permutations.path)
