#!/usr/bin/env python
# -*- coding: utf-8 -*-

import unittest
from run_clang_tidy import Diagnostic, Deduplication
from run_clang_tidy import ParseClangTidyDiagnostics, _is_valid_diag_match


class TestDiagnostics(unittest.TestCase):
    """Test fingerprinting diagnostic messages"""

    def test_construction(self):
        d = Diagnostic("/home/user/project/my_file.h", 24, 4,
                       "warning: Do not do this thing [warning-category]")
        self.assertIsNotNone(d)
        self.assertEqual(str(d),
                         "/home/user/project/my_file.h:24:4: warning: Do not do this thing [warning-category]")

        d.add_additional_line("      MyCodePiece();")
        d.add_additional_line("      ^")

        self.assertEqual(str(d),
                         "/home/user/project/my_file.h:24:4: warning: Do not do this thing [warning-category]"
                         "\n      MyCodePiece();"
                         "\n      ^")


class TestDeduplication(unittest.TestCase):
    """Test the `DiagEssence` based deduplication of diagnostic messages."""

    def test_construction(self):
        self.assertIsNotNone(Deduplication())

    def test_dedup(self):
        dedup = Deduplication()
        d = Diagnostic("/home/user/project/my_file.h", 24, 4,
                       "warning: Do not do this thing [warning-category]")
        self.assertTrue(dedup.insert_and_query(d))
        self.assertFalse(dedup.insert_and_query(d))

        d2 = Diagnostic("/home/user/project/my_file.h", 24, 4,
                        "warning: Do not do this thing [warning-category]")
        d2.add_additional_line("      MyCodePiece();")
        d2.add_additional_line("      ^")
        self.assertTrue(dedup.insert_and_query(d2))
        self.assertFalse(dedup.insert_and_query(d2))

        d3 = Diagnostic("/home/user/project/my_file.h", 24, 4,
                        "warning: Do not do this thing [warning-category]")
        self.assertFalse(dedup.insert_and_query(d3))

class TestLinewiseParsing(unittest.TestCase):
    def test_construction(self):
        self.assertIsNotNone(ParseClangTidyDiagnostics())

    def test_valid_diags_regex(self):
        pp = ParseClangTidyDiagnostics()

        warning = "/home/user/project/my_file.h:123:1: warning: don't do it [no]"
        m = pp._diag_re.match(warning)
        self.assertTrue(m)
        self.assertTrue(_is_valid_diag_match(m.groups()))

        error = "/home/user/project/my_file.h:1:110: error: wrong! [not-ok]"
        m = pp._diag_re.match(error)
        self.assertTrue(m)
        self.assertTrue(_is_valid_diag_match(m.groups()))

        hybrid = "/home/user/project/boo.cpp:30:42: error: wrong! [not-ok,bad]"
        m = pp._diag_re.match(hybrid)
        self.assertTrue(m)
        self.assertTrue(_is_valid_diag_match(m.groups()))

        note = "/home/user/project/my_file.h:1:110: note: alksdj"
        m = pp._diag_re.match(note)
        self.assertFalse(m)

        garbage = "not a path:not_a_number:110: gibberish"
        m = pp._diag_re.match(garbage)
        self.assertFalse(m)

    def test_single_diagnostics(self):
        pp = ParseClangTidyDiagnostics()
        example_warning = [
            "/project/git/Source/kwsys/Terminal.c:53:21: warning: use of a signed integer operand with a binary bitwise operator [hicpp-signed-bitwise]",
        ]
        pp._parse_lines(example_warning)
        self.assertEqual(
            str(pp.get_diags()[0]),
            "/project/git/Source/kwsys/Terminal.c:53:21: warning: use of a signed integer operand with a binary bitwise operator [hicpp-signed-bitwise]"
        )

    def test_no_diag(self):
        pp = ParseClangTidyDiagnostics()
        garbage_lines = \
"""
    hicpp-no-array-decay
    hicpp-no-assembler
    hicpp-no-malloc
    hicpp-noexcept-move
    hicpp-signed-bitwise
    hicpp-special-member-functions
    hicpp-static-assert
    hicpp-undelegated-constructor
    hicpp-use-auto
    hicpp-use-emplace
    hicpp-use-equals-default
    hicpp-use-equals-delete
    hicpp-use-noexcept
    hicpp-use-nullptr
    hicpp-use-override
    hicpp-vararg

clang-apply-replacements version 8.0.0
18 warnings generated.
36 warnings generated.
Suppressed 26 warnings (26 in non-user code).
Use -header-filter=.* to display errors from all non-system headers. Use -system-headers to display errors from system headers as well.

61 warnings generated.
122 warnings generated.
Suppressed 122 warnings (122 in non-user code).
Use -header-filter=.* to display errors from all non-system headers. Use -system-headers to display errors from system headers as well.

clang-tidy -header-filter=^/project/git/.* -checks=-*,hicpp-* -export-fixes /tmp/tmpH8MVt0/tmpErKPl_.yaml -p=/project/git /project/git/Source/kwsys/Terminal.c
"""
        pp.parse_string(garbage_lines)
        self.assertEqual(len(pp.get_diags()), 0)

    def test_deduplicate_basic_multi_line_warning(self):
        pp = ParseClangTidyDiagnostics()
        example_warning = [
            "/project/git/Source/kwsys/Terminal.c:53:21: warning: use of a signed integer operand with a binary bitwise operator [hicpp-signed-bitwise]",
            "int default_tty = color & kwsysTerminal_Color_AssumeTTY;",
            "                    ^",
        ]

        pp._parse_lines(example_warning + example_warning)
        diags = pp.get_diags()

        self.assertEqual(len(diags), 1)
        self.assertEqual(
            str(diags[0]),
            "/project/git/Source/kwsys/Terminal.c:53:21: warning: use of a signed integer operand with a binary bitwise operator [hicpp-signed-bitwise]"
            "\nint default_tty = color & kwsysTerminal_Color_AssumeTTY;"
            "\n                    ^")

    def test_real_diags(self):
        pp = ParseClangTidyDiagnostics()
        excerpt = \
"""/project/git/Source/kwsys/Base64.c:54:35: warning: use of a signed integer operand with a binary bitwise operator [hicpp-signed-bitwise]
  dest[0] = kwsysBase64EncodeChar((src[0] >> 2) & 0x3F);
                                  ^
/project/git/Source/kwsys/Base64.c:54:36: warning: use of a signed integer operand with a binary bitwise operator [hicpp-signed-bitwise]
  dest[0] = kwsysBase64EncodeChar((src[0] >> 2) & 0x3F);
                                   ^
/project/git/Source/kwsys/Base64.c:56:27: warning: use of a signed integer operand with a binary bitwise operator [hicpp-signed-bitwise]
    kwsysBase64EncodeChar(((src[0] << 4) & 0x30) | ((src[1] >> 4) & 0x0F));
                          ^
/project/git/Source/kwsys/Base64.c:56:28: warning: use of a signed integer operand with a binary bitwise operator [hicpp-signed-bitwise]
    kwsysBase64EncodeChar(((src[0] << 4) & 0x30) | ((src[1] >> 4) & 0x0F));
                           ^
/project/git/Source/kwsys/Base64.c:54:35: warning: use of a signed integer operand with a binary bitwise operator [hicpp-signed-bitwise]
  dest[0] = kwsysBase64EncodeChar((src[0] >> 2) & 0x3F);
                                  ^
/project/git/Source/kwsys/Base64.c:54:36: warning: use of a signed integer operand with a binary bitwise operator [hicpp-signed-bitwise]
  dest[0] = kwsysBase64EncodeChar((src[0] >> 2) & 0x3F);
                                   ^
/project/git/Source/kwsys/Base64.c:56:27: warning: use of a signed integer operand with a binary bitwise operator [hicpp-signed-bitwise]
    kwsysBase64EncodeChar(((src[0] << 4) & 0x30) | ((src[1] >> 4) & 0x0F));
                          ^
/project/git/Source/kwsys/Base64.c:56:28: warning: use of a signed integer operand with a binary bitwise operator [hicpp-signed-bitwise]
    kwsysBase64EncodeChar(((src[0] << 4) & 0x30) | ((src[1] >> 4) & 0x0F));
                           ^
/project/git/Source/kwsys/testCommandLineArguments.cxx:16:10: warning: inclusion of deprecated C++ header 'string.h'; consider using 'cstring' instead [hicpp-deprecated-headers]
#include <string.h> /* strcmp */
         ^~~~~~~~~~
         <cstring>
/project/git/Source/kwsys/testFStream.cxx:10:10: warning: inclusion of deprecated C++ header 'string.h'; consider using 'cstring' instead [hicpp-deprecated-headers]
#include <string.h>
         ^~~~~~~~~~
         <cstring>
/project/git/Source/kwsys/testFStream.cxx:77:47: warning: do not implicitly decay an array into a pointer; consider using gsl::array_view or an explicit cast instead [hicpp-no-array-decay]
      out.write(reinterpret_cast<const char*>(expected_bom_data[i] + 1),
                                              ^
/project/git/Source/kwsys/testFStream.cxx:78:18: warning: do not implicitly decay an array into a pointer; consider using gsl::array_view or an explicit cast instead [hicpp-no-array-decay]
                *expected_bom_data[i]);
                 ^
/project/git/Source/kwsys/testFStream.cxx:109:3: warning: use of a signed integer operand with a binary bitwise operator [hicpp-signed-bitwise]
  ret |= testNoFile();
  ^
/project/git/Source/kwsys/testFStream.cxx:110:3: warning: use of a signed integer operand with a binary bitwise operator [hicpp-signed-bitwise]
  ret |= testBOM();
  ^
/project/git/Source/kwsys/testSystemInformation.cxx:83:36: warning: use of a signed integer operand with a binary bitwise operator [hicpp-signed-bitwise]
    if (info.DoesCPUSupportFeature(static_cast<long int>(1) << i)) {
                                   ^"""
        pp.parse_string(excerpt)
        self.assertEqual(len(pp.get_diags()), 11)

        self.maxDiff = None
        generated_diag = "\n".join(str(diag) for diag in pp.get_diags())
        # It is not identical because of deduplication.
        self.assertNotEqual(generated_diag, excerpt)

        # The first 11 lines are duplicated diags but the rest is identical.
        self.assertEqual(generated_diag,
                         "\n".join(excerpt.splitlines()[12:]))

        # Pretend that the next clang-tidy invocation returns its data
        # and the parser shall deduplicate this one as well. This time
        # no new data is expected.
        pp.reset_parser()
        pp.parse_string(excerpt)
        self.assertEqual(len(pp.get_diags()), 0)

    def test_log_files(self):
        pp = ParseClangTidyDiagnostics()
        pp._parse_file("test_input/out_csa_cmake.log")
        self.assertEqual(len(pp.get_diags()), 5)

        pp.reset_parser()
        pp._parse_file("test_input/out_csa_cmake.log")
        self.assertEqual(len(pp.get_diags()), 0)

        pp.reset_parser()
        pp._parse_file("test_input/out_performance_cmake.log")
        self.assertEqual(len(pp.get_diags()), 24)

        pp.reset_parser()
        pp._parse_file("test_input/out_performance_cmake.log")
        self.assertEqual(len(pp.get_diags()), 0)


if __name__ == "__main__":
    unittest.main()
