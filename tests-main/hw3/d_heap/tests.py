from curricula_grade.shortcuts import *
from curricula_grade.setup.common import check_file_exists
from curricula_grade.test.correctness.common import ProcessCompareExitCodeTest
from curricula_grade_cpp.setup.common.gpp import gpp_compile_object
from curricula.library.files import delete_file
from pathlib import Path

root = Path(__file__).absolute().parent
grader = Grader()

GPP = "-std=c++11",


@grader.register(graded=False, tags={"sanity"})
def check_heap(submission: Submission) -> SetupResult:
    """Check the heap header is in the directory."""

    return check_file_exists(submission.problem_path.joinpath("MinHeap.h"))


@grader.register(graded=False, tags={"sanity"}, passing={"check_heap"})
def build_heap_harness(submission: Submission, resources: dict) -> SetupResult:
    """Build the harness with the heap file."""

    result, resources["heap"] = gpp_compile_object(
        root.joinpath("harness.cpp"),
        destination_path=submission.problem_path.joinpath("harness"),
        gpp_options=(f"-I{submission.problem_path}", *GPP),
        timeout=30)
    return result


class HarnessTest(ProcessCompareExitCodeTest):
    """Testing occurs in the C++ method."""

    executable_name = "heap"
    expected_code = 0
    timeout = 15


for i in range(22 + 1):
    grader.register[CorrectnessResult](passing={"build_heap_harness"}, name=f"case{i}")(
        HarnessTest(args=(str(i),)))


@grader.register[CleanupResult](graded=False, passing={"build_heap_harness"}, tags={"sanity"})
def clean_heap_harness(heap: ExecutableFile):
    """Delete the compiled harness file."""

    delete_file(heap.path)
