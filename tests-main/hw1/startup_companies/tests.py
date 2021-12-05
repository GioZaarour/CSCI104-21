from curricula_grade.shortcuts import *
from curricula_grade.setup.common import check_file_exists
from curricula_grade_cpp.setup.common.gpp import *
from curricula_grade.test.correctness.common import *
from curricula.library.files import delete_file

from pathlib import Path

GPP_OPTIONS = ("-std=c++17",)


root = Path(__file__).absolute().parent
grader = Grader()


@grader.register(tags={"sanity"}, graded=False)
def check_companies_header(submission: Submission, resources: dict) -> SetupResult:
    """Check companies.hpp is in the submission folder."""

    resources["companies_header_path"] = path = submission.problem_path.joinpath("company.hpp")
    return check_file_exists(path)


@grader.register(tags={"sanity"}, graded=False)
def check_companies_source(submission: Submission, resources: dict) -> SetupResult:
    """Check companies.cpp is in the submission folder."""

    resources["companies_source_path"] = path = submission.problem_path.joinpath("company.cpp")
    return check_file_exists(path)


@grader.register(tags={"sanity"}, graded=False, passing={"check_companies_header", "check_companies_source"})
def build_companies_main(submission: Submission, companies_source_path: Path, resources: dict) -> SetupResult:
    """Make sure we can build companies.cpp with our own main function."""

    result, resources["companies"] = gpp_compile_object(
        root.joinpath("main.cpp"),
        companies_source_path,
        destination_path=submission.problem_path.joinpath("companies_main"),
        gpp_options=(f"-I{submission.problem_path}", *GPP_OPTIONS),
        timeout=15)
    return result


class StartupCompaniesTest(ProcessCompareStreamTest):
    """You mad?"""

    matcher = staticmethod(lines_match_unordered)
    executable_name = "companies"
    timeout = 10.0
    out_transform = staticmethod(bytes.strip)

    def __init__(self, case_name: str):
        super().__init__(
            args=(str(root.joinpath("input", f"{case_name}.txt")),),
            test_out_lines=root.joinpath("output", f"{case_name}.txt").read_bytes().strip().splitlines(keepends=False))

    @classmethod
    def register(cls, case_name: str, description: str = "test startup company"):
        grader.register(
            name=f"test_{case_name}",
            description=description,
            passing={"build_companies_main"}
        )(StartupCompaniesTest(case_name=case_name))


for i in range(1, 25 + 1):
    StartupCompaniesTest.register(case_name=f"case{i}")


@grader.register[CleanupResult](tags={"sanity"}, graded=False, passing={"build_companies_main"})
def cleanup_companies_main(companies: ExecutableFile):
    """Delete companies."""

    delete_file(companies.path)
