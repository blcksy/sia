'''
sia.backdoors module testing script for pytest
'''

from pathlib import Path

import pytest

from sia.backdoors import SynClass


def test_cnfbaseclass_is_horn():
    '''Test if formula is horn'''
    horn = [[-1, -2, 3], [2], [-4, 5], [-2], [-3, -4]]
    assert SynClass.is_horn(horn) == True


def test_cnfbaseclass_is_dual_horn():
    '''Test if formula is horn'''
    dual_horn = [[-1, 2, 3], [2], [-4, 5], [-2], [3, 4]]
    assert SynClass.is_dual_horn(dual_horn) == True


def test_cnfbaseclass_is_dual_horn():
    '''Test if formula is horn'''
    k_sat = [[-1, 2, 3], [2], [-4, 5], [-2], [3, 4]]
    assert SynClass.is_k_sat(k_sat, 3) == True
    assert SynClass.is_k_sat(k_sat, 2) == False
