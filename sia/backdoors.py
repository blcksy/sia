# pylint: disable-msg=C0103
'''
Module that implements propositional formula backdoors related functions.
'''
import itertools as it

from . import cnf


class SynClass:
    '''
    Various functions for identifying syntactic classes of SAT formulas:

    Common syntactic classes:

        - k-sat formulas

    Polynomially solveable syntactic classes:

        - Horn formulas
        - Dual Horn formulas

    '''

    @staticmethod
    def is_k_sat(clauses, k):
        '''Checks if a formula is k sat'''
        return all(len(clause) <= k for clause in clauses)

    @staticmethod
    def is_horn(clauses):
        '''Checks if a formula belongs to the family of Horn formulas'''
        flag = False
        for clause in clauses:
            for literal in clause:
                if literal > 0:
                    if not flag:
                        flag = True
                    else:
                        return False
            flag = False
        return True

    @staticmethod
    def is_dual_horn(clauses):
        '''Checks if a formula belongs to the family of DualHorn formulas'''
        flag = False
        for clause in clauses:
            for literal in clause:
                if literal < 0:
                    if not flag:
                        flag = True
                    else:
                        return False
            flag = False
        return True

    @staticmethod
    def is_renameable_horn(clauses):
        '''Checks if a formula belongs to the family of RenameableHOrn formulas'''
        # TODO
        pass


def all_subsets(n, size=None):
    '''Computes al subsets a set of n elements'''
    if size:
        for e in it.combinations(range(1, n+1), size):
            yield e
    else:
        for i in range(1, n+1):
            for e in it.combinations(range(1, n+1), i):
                yield e


def barr(clauses, k):
    '''Prints a █ for each clause of length < 3'''
    print(f"[{''.join(['█' if len(clause) <= k else ' ' for clause in clauses])}]")
    