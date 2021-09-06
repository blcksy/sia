'''
sia.io module testing script for pytest
'''

from pathlib import Path

import pytest

import sia


TEST_DIR = Path('tests/data')


def test_from_file_empty_file():
    '''Empty file test, no header'''
    with pytest.raises(ValueError):
        d, n, m, c = sia.io.from_file(TEST_DIR / 'empty_file.cnf')


def test_from_file_empty_formula():
    '''Empty formula test, no variables, no clauses'''
    _, n, m, c = sia.io.from_file(TEST_DIR / 'empty_formula.cnf')
    assert n == 0
    assert m == 0
    assert c == []


def test_from_file_1_clause_no_var():
    '''1 clause, no variables'''
    _, n, m, c = sia.io.from_file(TEST_DIR / '1_clause_0_var.cnf')
    assert n == 0
    assert m == 1
    assert c == [[]]
