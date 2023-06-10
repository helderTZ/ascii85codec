import subprocess
import pathlib

def test_enc():
    p = subprocess.Popen(["bazel-bin/ascii85enc", "tests/test3.txt"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    out, err = p.communicate()

    expected = pathlib.Path("tests/test4.txt").read_text()

    assert out == expected

def test_dec():
    p = subprocess.Popen(["bazel-bin/ascii85enc", "tests/test4.txt"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    out, err = p.communicate()

    expected = pathlib.Path("tests/test3.txt").read_text()

    assert out == expected
