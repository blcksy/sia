# pylint: disable-msg=C0103
'''
Utilities for manipulating CNF formulas
'''

'''
Using a class for holding CNF formula info doesn't make any sense at the
moment. We are only working with 3 values: n, m, and clauses
Anyway, it was decided to be maintained.
'''
class CNF:
    '''Class containing CNF formula values'''

    def __init__(self, n, m, clauses):
        self.n = n
        self.m = m
        self.clauses = clauses
        self.var_to_clause = [[] for _ in range(n+1)]
        for i, clause in enumerate(clauses):
            for literal in clause:
                self.var_to_clause[abs(literal)].append(i)

# Check


def empty_clauses(clauses):
    '''Check if there exist an empty clause'''
    return any(not clause for clause in clauses)


def unit_clauses(clauses):
    '''Check if there exists an unit clauses'''
    return any(len(clause) == 1 for clause in clauses)


# Propagation


def propagate_literal(clauses, literal):
    '''Propagates a given literal'''
    return [[l for l in clause if l != -literal]
            for clause in clauses if not literal in clause]


def propagate(clauses, assumptions):
    ''' Propagates a given list of assumptions'''
    aux = clauses
    for a in assumptions:
        aux = propagate_literal(aux, a)
    return aux


def unit_propagation(clauses):
    '''Propagates unit literals of a given clause list'''
    unit = [clause[0] for clause in clauses if len(clause) == 1]
    aux = propagate(clauses, unit)
    return aux


def reduce_clauses(clauses):
    '''Applies unit propagation until it isn't possible'''
    aux = clauses
    #print('\nReduce clauses:\t', aux)
    while unit_clauses(aux):
        #print('\nInside:\t', aux)
        aux = unit_propagation(aux)
        if empty_clauses(aux):
            return False
    return aux


def pure_literal(n, clauses):
    '''Returns an array of size number of vars, where:
        *  0: no variable appearence
        *  1: positive pure literal
        * -1: negative pure literal
        *  2: no pure literal
    '''
    check = [0 for _ in range(n)]
    for clause in clauses:
        for literal in clause:

            i = abs(literal)-1

            # Not checked case
            if check[i] == 0:
                check[i] = 1 if literal > 0 else -1
                continue

            # Not pure case
            if check[i] == 1 if literal < 0 else -1:
                check[i] = 2
                continue
    return check


def get_pure_literal(n, clauses):
    '''Returns an assumption list from a given pure literal check list'''
    pure = pure_literal(n, clauses)
    ans = []
    # ans = [i+1 if pure[i] == 1 else -
    #       (i+1) for i in range(n) if pure[i] != 0 or pure[i] != 2]
    for i in range(n):
        if pure[i] == 1:
            ans.append(i+1)
        elif pure[i] == -1:
            ans.append(-(i+1))
    return ans


def simplification(n, clauses):
    '''Aplies unit propagation and pure literal rule until it isn't possible'''
    # TODO
    pass

# Manipulation


def to_3_sat(n, clauses):
    '''Converts a CNF instance into a 3-CNF instance'''
    aux = []
    for clause in clauses:
        if len(clause) <= 3:
            aux.append(clause)
        else:
            total = len(clause)-4
            # Start
            aux.append(clause[0:2] + [-n-1])
            # Mid
            for i in range(total):
                aux.append([n+1, clause[2+i], -n-2])
                n += 1
            # End
            aux.append([n+1] + clause[-2:])
            n += 1
    return aux
