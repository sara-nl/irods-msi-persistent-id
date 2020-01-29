##############################################################################
# Copyright 2018, SURFsara
# Author Stefan Wolfsheimer
#
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##############################################################################
import pytest
import unittest
import re

from util import exec_rule
from util import exec_rule_list
from util import configure_handle_service
from util import unconfigure_handle_service

process = None
PREFIX = "21.T12995"
PREFIX_PATTERN = re.compile(re.escape(PREFIX) + "/.+")


#########################################################
#
# tests
#
#########################################################
class TestPidMicroServices(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        configure_handle_service()

    @classmethod
    def tearDownClass(cls):
        unconfigure_handle_service()

    def test_handle_life_time(self):
        # #####################################
        # Before Create
        # #####################################
        path1 = "/tempZone/home/rods/example1.txt"
        path2 = "/tempZone/home/rods/example2.txt"
        path3 = "/tempZone/home/rods/example3.txt"

        assert exec_rule_list("rule_lookup.r", path=path1) == []
        with pytest.raises(Exception):
            exec_rule("rule_lookup_one.r", path=path1)
        assert exec_rule_list("rule_lookup_key.r",
                              value="http://localhost:80{0}".format(path1),
                              key="URL") == []

        # #####################################
        # Create
        # #####################################
        pid1 = exec_rule("rule_create.r",
                         path=path1,
                         key_values_inp="a,1,b,2")
        assert PREFIX_PATTERN.match(pid1)

        pid2 = exec_rule("rule_create.r",
                         path=path2)
        assert PREFIX_PATTERN.match(pid2)

        pid12 = exec_rule_list("rule_lookup.r",
                               path="/tempZone/home/rods/*")
        assert len(pid12) == 2
        assert pid1 in pid12
        assert pid2 in pid12

        with pytest.raises(Exception):
            exec_rule("rule_lookup_one.r",
                      path="/tempZone/home/rods/*")
        pid12 = exec_rule_list("rule_lookup_key.r",
                               value="http://localhost:80/*",
                               key="URL")
        assert len(pid12) == 2
        assert pid1 in pid12
        assert pid2 in pid12

        # #####################################
        # Get / GetHandle
        # #####################################
        assert exec_rule("rule_get.r", path=path1)
        assert exec_rule("rule_get.r", path=path1, key="a") == "1"
        assert exec_rule("rule_get.r", path=path1, key="b") == "2"
        assert exec_rule("rule_get_handle.r", handle=pid1, key="a") == "1"

        # #####################################
        # Move
        # #####################################
        assert exec_rule("rule_move.r", pathOld=path1, pathNew=path3)

        with pytest.raises(Exception):
            assert exec_rule("rule_get.r", path=path1)
        assert exec_rule("rule_get.r", path=path3, key="a") == "1"
        assert exec_rule("rule_get.r", path=path3, key="b") == "2"
        assert exec_rule("rule_get_handle.r", handle=pid1, key="a") == "1"

        # #####################################
        # MoveHandle
        # #####################################
        assert exec_rule("rule_move_handle.r", handle=pid1, pathNew=path1)

        # #####################################
        # Set / SetHandle
        # #####################################
        assert exec_rule("rule_set.r", path=path1, key="b", value="3") == pid1
        exec_rule("rule_set_handle.r", handle=pid1, key="c", value="4")
        assert exec_rule("rule_get.r", path=path1, key="a") == "1"
        assert exec_rule("rule_get.r", path=path1, key="b") == "3"
        assert exec_rule("rule_get.r", path=path1, key="c") == "4"

        assert exec_rule("rule_set.r", path=path2, mvalue="a,1,b,2,c,3")
        assert exec_rule("rule_get.r", path=path2, key="a") == "1"
        assert exec_rule("rule_get.r", path=path2, key="b") == "2"
        assert exec_rule("rule_get.r", path=path2, key="c") == "3"

        exec_rule("rule_set_handle.r", handle=pid1, mvalue="a,1,b,2,c,3")
        assert exec_rule("rule_get.r", path=path1, key="a") == "1"
        assert exec_rule("rule_get.r", path=path1, key="b") == "2"
        assert exec_rule("rule_get.r", path=path1, key="c") == "3"

        # #####################################
        # Lookup key
        # #####################################
        pid12 = exec_rule_list("rule_lookup_key.r",
                               value="1",
                               key="a")
        assert len(pid12) == 2
        assert pid1 in pid12
        assert pid2 in pid12

        # #####################################
        # Unset / UnsetHandle
        # #####################################
        assert exec_rule("rule_unset.r", path=path1, key="a") == pid1
        assert exec_rule("rule_get.r", path=path1, key="a") == ""
        assert exec_rule("rule_unset_handle.r", handle=pid2, key="a") == pid2
        assert exec_rule("rule_get.r", path=path2, key="a") == ""

        # #####################################
        # Delete / DeleteHandle
        # #####################################
        assert exec_rule("rule_delete.r", path=path1)
        assert exec_rule("rule_delete_handle.r", handle=pid2)
        pid12 = exec_rule_list("rule_lookup.r",
                               path="/tempZone/home/rods/*")
        assert len(pid12) == 0
