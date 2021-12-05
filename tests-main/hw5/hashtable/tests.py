from curricula_grade.shortcuts import *
from curricula_grade.setup.common import check_file_exists
from curricula_grade.common.process import ProcessExecutor, ProcessExitCodeConnector
from curricula_grade.test.correctness.common import CorrectnessRunnable, CompareExitCodeEvaluator
from curricula_grade_cpp.setup.common.gpp import gpp_compile_object
from curricula_grade_cpp import Paths
from curricula.library import files

from pathlib import Path

root = Path(__file__).absolute().parent
grader = Grader()

GPP_OPTIONS = ("-std=c++17",)


class CurriculaTest(ProcessExecutor, ProcessExitCodeConnector, CompareExitCodeEvaluator, CorrectnessRunnable):
    """Simple correctness test."""

    expected_code = 0


class CorrectnessTest(CurriculaTest):
    """Simple correctness test."""

    expected_code = 0
    executable_name = "hashtable"
    timeout = 5


@grader.register(tags={"sanity"}, graded=False)
def check_hashtable_header(submission: Submission, resources: dict) -> SetupResult:
    resources["hashtable_header_path"] = path = submission.problem_path.joinpath("Hashtable.h")
    return check_file_exists(path)


@grader.register(passing={"check_hashtable_header"}, graded=False)
def build_hashtable(submission: Submission, resources: dict) -> SetupResult:
    """Build the harness with the heap file."""

    result, resources["hashtable"] = gpp_compile_object(
        root.joinpath("tests.cpp"),
        destination_path=submission.problem_path.joinpath("hashtable_harness"),
        gpp_options=(f"-I{Paths.INCLUDE}", f"-I{submission.problem_path}", *GPP_OPTIONS),
        timeout=10)
    return result


TEST_NAMES = (
    "CheckHash_a",
    "CheckHash_b",
    "CheckHash_l",
    "CheckHash_test",
    "CheckHash_mediumlength",
    # "CheckHash_antidisestablishmentarianism",
    "AddCheckCount_a",
    "AddCheckCount_a_b",
    "AddCheckCount_a_b_test",
    "AddLinearProbingCheckCount_a_l",
    "AddLinearProbingCheckCount_a_aa_l",
    "ForceResize_Linear")

for name in TEST_NAMES:
    grader.register.register(
        CorrectnessTest(args=(name,)),
        details=dict(
            name=name,
            passing={"build_hashtable"}))


@grader.register[CleanupResult](passing={"build_hashtable"}, graded=False)
def delete_hashtable(hashtable: ExecutableFile):
    files.delete_file(hashtable.path)
