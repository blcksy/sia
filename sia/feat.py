'''
This module interfaces the featsat C extension for a better control
'''

import featsat
from . import io


def modularity(file_name, mode='vig'):
    '''
    Computes de modularity of a CNF formula from a given file.
    It has VIG and CVIG mode.
    '''

    file_name_str = file_name.__str__()
    _, _, clause_num = io.get_header(file_name)

    if mode == 'vig':
        ans = featsat.modularity_vig(file_name_str, clause_num)
        return ans

    if mode == 'cvig':
        ans = featsat.modularity_cvig(file_name_str, clause_num)
        return ans

    raise ValueError(f'Argument mode={mode} not valid. Choose "vig" or "cvig"')


def self_similar(file_name, mode='vig'):
    '''
    Computes de fractal dimension of a CNF formula from a given file.
    It has VIG and CVIG mode.
    '''

    file_name_str = file_name.__str__()
    _, _, clause_num = io.get_header(file_name)

    if mode == 'vig':
        ans = featsat.self_similar_vig(file_name_str, clause_num)
        return ans

    if mode == 'cvig':
        ans = featsat.self_similar_cvig(file_name_str, clause_num)
        return ans

    raise ValueError(f'Argument mode={mode} not valid. Choose "vig" or "cvig"')


def scale_free(file_name, mode='var'):
    '''
    Computes de scale free exponent of a CNF formula from a given file.
    It has var and clause mode.
    '''

    file_name_str = file_name.__str__()
    _, _, clause_num = io.get_header(file_name)

    if mode == 'var':
        ans = featsat.scale_free_var(file_name_str, clause_num)
        return ans

    if mode == 'clause':
        ans = featsat.scale_free_clause(file_name_str, clause_num)
        return ans

    raise ValueError(f'Argument mode={mode} not valid. Choose "var" or "clause"')
