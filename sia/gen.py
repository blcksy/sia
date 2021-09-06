'''
Module for generating some common familes of SAT formulas
'''

# pylint: disable-msg=C0103
import collections
import itertools as it
import random


def random_formula_check(k, n, m, seed):
    '''Check restrictions on random formulas'''

    assert k > 0, 'Number k must be positive.'
    assert k <= n, 'Number k must be smaller than variable num.'
    assert n > 0, 'Variable num must be greater than 0.'
    assert m > 0, 'Clause num must be greater than 0.'
    assert isinstance(
        0, collections.Hashable), 'Seed: {} is not hashable.'.format(seed)


def random_k_cnf(k, n, m, seed=None):
    '''Random K-CNF formulas with no repeating literals per clause'''

    random_formula_check(k, n, m, seed)

    variables = range(1, n+1)
    formula = []

    if seed:
        random.seed(seed)

    for _ in range(m):
        pre = random.sample(variables, k)
        clause = [random.choice([1, -1])*literal for literal in pre]
        formula.append(clause)

    return formula


def random_horn(k, n, m, seed=None):
    ''' Random Horn formulas with clauses of size k'''

    random_formula_check(k, n, m, seed)

    variables = range(-1, -n-1, -1)
    formula = []

    if seed:
        random.seed(seed)

    for _ in range(m):
        clause = random.sample(variables, k)
        if random.uniform(-1, 1) > 0:
            clause[random.randint(0, k-1)] *= -1
        formula.append(clause)

    return formula


def pigeon_hole(n):
    '''Pigeonhole Principle encoding for n pigeons and n-1 holes'''

    clauses = []

    # At Least One clauses
    for i in range(n):
        clause = []
        for j in range(n-1):
            clause.append(i*n-i+j+1)
        clauses.append(clause)

    # At Most One clauses
    for j in range(n-1):
        for pair in it.combinations([n*i-i+j+1 for i in range(n)], 2):
            clauses.append([-pair[0], -pair[1]])

    return clauses


def InverseGOL(state):
    '''
    Computes de previous state of a given Game of life game state as the
    satisfying assignment of the game problem stated as a CNF formula
    '''
    # TODO: Future update
    pass
