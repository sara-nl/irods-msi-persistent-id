##############################################################################
# Copyright 2019, SURFsara
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

from util import exec_rule_py
from util import exec_rule
from util import configure_handle_service
from util import unconfigure_handle_service
from util import create_user_and_group
from util import delete_user_and_group


process = None
PREFIX = "21.T12995"
PREFIX_PATTERN = re.compile(re.escape(PREFIX) + "/.+")


#########################################################
#
# tests
#
#########################################################
class TestPidMicroServicesPermissions(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        create_user_and_group('tst_user', 'tst_group')
        configure_handle_service()

    @classmethod
    def tearDownClass(cls):
        delete_user_and_group('tst_user', 'tst_group')
        unconfigure_handle_service()

    def test_handle_lookup(self):
        # lookup is allowed for all users and groups
        exec_rule_py("rule_lookup.r", "tst_user")

    def test_handle_create(self):
        # creating is not allowed
        with pytest.raises(Exception):
            exec_rule_py("rule_create.r", "tst_user")

    def test_handle_set(self):
        # create by user rods
        exec_rule("rule_create.r")
        # update as user auth through group membership
        # "users_write": ["rods#tempZone"],
        # "groups_write": ["tst_group"],
        # setting is allowed for user
        exec_rule_py("rule_set.r", "tst_user")
