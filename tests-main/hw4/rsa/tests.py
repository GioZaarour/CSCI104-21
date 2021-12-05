from curricula_grade.shortcuts import *
from curricula_grade.setup.common import check_file_exists
from curricula_grade.common.process import verify_runtime
from curricula_grade.grader.task import Error
from curricula_grade_cpp.setup.common.makefile import check_makefile_exists, makefile_execute
from curricula.library.files import delete_file
from pathlib import Path

root = Path(__file__).absolute().parent
grader = Grader()

GPP = "-std=c++11",


@grader.register(graded=False, tags={"sanity"})
def check_rsa_makefile(submission: Submission) -> SetupResult:
    """Assert a Makefile is present in the target."""

    return check_makefile_exists(submission.problem_path)


@grader.register(graded=False, tags={"sanity"}, passing={"check_rsa_makefile"})
def build_rsa(submission: Submission) -> SetupResult:
    """Run the Makefile."""

    return makefile_execute(submission.problem_path, timeout=10)


@grader.register(graded=False, tags={"sanity"}, passing={"build_rsa"})
def check_rsa(submission: Submission, resources: dict) -> SetupResult:
    """Check whether the binary exists."""

    rsa_path = submission.problem_path.joinpath("rsa")
    resources["rsa"] = ExecutableFile(rsa_path)
    return check_file_exists(rsa_path)


@grader.register(tags={"sanity"}, passing={"check_rsa"})
def test_rsa_encrypt(rsa: ExecutableFile) -> CorrectnessResult:
    """Check the output of encryption."""

    out = Path("out.txt")
    runtime = rsa.execute(
        "9733", "9739",
        stdin=f"ENCRYPT {out} 94789687 fight on\nEXIT\n".encode(),
        timeout=2)

    try:
        contents = out.read_bytes().strip()
        delete_file(out)
    except FileNotFoundError:
        return CorrectnessResult.shorthand(passing=False, error=Error("cannot find expected output file"))

    verify_runtime(runtime, CorrectnessResult)
    if contents == b"8378493 5033969":
        return CorrectnessResult.shorthand(runtime=runtime.dump())

    else:
        return CorrectnessResult.shorthand(
            passing=False,
            expected="8378493 5033969",
            actual=contents.decode(errors="replace"),
            error=Error("incorrect encrypted values"),
            runtime=runtime.dump())


@grader.register(tags={"sanity"}, passing={"check_rsa"})
def test_rsa_decrypt(rsa: ExecutableFile) -> CorrectnessResult:
    """Check the output of decryption."""

    out = Path("out.txt")
    out.write_bytes(b"8378493 5033969")
    plain = Path("plain.txt")

    runtime = rsa.execute(
        "9733", "9739",
        stdin=f"DECRYPT {out} {plain}\nEXIT\n".encode(),
        timeout=2)

    try:
        contents = plain.read_bytes().strip()
        delete_file(plain)
    except FileNotFoundError:
        return CorrectnessResult.shorthand(passing=False, error=Error("cannot find expected output file"))

    delete_file(out)

    verify_runtime(runtime, CorrectnessResult)
    if contents == b"fight on":
        return CorrectnessResult.shorthand(runtime=runtime.dump())

    else:
        return CorrectnessResult.shorthand(
            passing=False,
            expected="fight on",
            actual=contents.decode(errors="replace"),
            error=Error("incorrect decrypted string"),
            runtime=runtime.dump())


@grader.register(tags={"sanity"}, passing={"check_rsa"})
def test_rsa_encrypt_decrypt(rsa: ExecutableFile) -> CorrectnessResult:
    """See if encrypting then decrypting returns the same text."""

    out = Path("out.txt")
    plain = Path("plain.txt")

    runtime = rsa.execute(
        "9733", "9739",
        stdin=f"ENCRYPT {out} 94789687 tommy trojan\nDECRYPT {out} {plain}\nEXIT\n".encode(),
        timeout=2)

    try:
        contents = plain.read_bytes().strip()
        delete_file(plain)
    except FileNotFoundError:
        return CorrectnessResult.shorthand(passing=False, error=Error("cannot find expected output file"))

    delete_file(out)

    verify_runtime(runtime, CorrectnessResult)
    if contents == b"tommy trojan":
        return CorrectnessResult.shorthand(runtime=runtime.dump())

    else:
        return CorrectnessResult.shorthand(
            passing=False,
            expected="tommy trojan",
            actual=contents.decode(errors="replace"),
            error=Error("incorrect decrypted string"),
            runtime=runtime.dump())


@grader.register(tags={"sanity"}, passing={"check_rsa"})
def test_rsa_encrypt_decrypt_alternate(rsa: ExecutableFile) -> CorrectnessResult:
    """See if encrypting then decrypting returns the same text."""

    out = Path("out.txt")
    plain = Path("plain.txt")

    runtime = rsa.execute(
        "9733", "9949",
        stdin=f"ENCRYPT {out} 96833617 tommy trojan\nDECRYPT {out} {plain}\nEXIT\n".encode(),
        timeout=2)

    try:
        contents = plain.read_bytes().strip()
        delete_file(plain)
    except FileNotFoundError:
        return CorrectnessResult.shorthand(passing=False, error=Error("cannot find expected output file"))

    delete_file(out)

    verify_runtime(runtime, CorrectnessResult)
    if contents == b"tommy trojan":
        return CorrectnessResult.shorthand(runtime=runtime.dump())

    else:
        return CorrectnessResult.shorthand(
            passing=False,
            expected="tommy trojan",
            actual=contents.decode(errors="replace"),
            error=Error("incorrect decrypted string"),
            runtime=runtime.dump())


@grader.register[CleanupResult](passing={"check_rsa"})
def clean_learning(rsa: ExecutableFile):
    """Delete the binary."""

    delete_file(rsa.path)
