
import unittest
from mlog_logger import LogInfoMapBuilder, is_value_present, plain_address
#LogInfoMapBuilder

#pylint:disable=too-many-public-methods
#pylint:disable=protected-access

class TestMlogLogger(unittest.TestCase):
    def test_smoke(self):
        self.assertEqual(3, 1 + 2)

    def test_parse_log_info_fail(self):
        # No match at all.
        log_info = LogInfoMapBuilder._parse_log_string("foo")
        self.assertEqual(None, log_info)

        # Whitespace at end.
        log_info = LogInfoMapBuilder._parse_log_string("012f7e48 MLOG_FOO_ADAPTER_ADAPTER_II_cyclic_task_time_is___  ")
        self.assertEqual(None, log_info)

    def test_parse_log_info_ok(self):
        log_info = LogInfoMapBuilder._parse_log_string("12f7e48   MLOG_FOO_ADAPTER_ADAPTER_II_cyclic_task_time_is___")
        expected = LogInfoMapBuilder.LogInfo(component="FOO_ADAPTER_ADAPTER", text="cyclic_task_time_is", level="II", address=0x12f7e48)
        self.assertEqual(expected, log_info)

        log_info = LogInfoMapBuilder._parse_log_string("1234 MLOG_FOO_EE_this_is_an_error___")
        expected = LogInfoMapBuilder.LogInfo(component="FOO", text="this_is_an_error", level="EE", address=0x1234)
        self.assertEqual(expected, log_info)

        # Various whitespace at beginning.
        log_info = LogInfoMapBuilder._parse_log_string("012f7e48  MLOG_FOO_ADAPTER_ADAPTER_II_cyclic_task_time_is___")
        expected = LogInfoMapBuilder.LogInfo(component="FOO_ADAPTER_ADAPTER", text="cyclic_task_time_is", level="II", address=0x12f7e48)
        self.assertEqual(expected, log_info)

    def test_value_present(self):
        self.assertFalse(is_value_present(0))
        self.assertFalse(is_value_present(1000))
        self.assertTrue(is_value_present(0x40001234))
        self.assertEqual(0, plain_address(0))
        self.assertEqual(0x1234, plain_address(0x40001234))
        self.assertEqual(0x80001234, plain_address(0x80001234))


