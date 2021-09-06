# pylint: disable-msg=C0103
'''
Utilities for reading and writting CNF formulas.
'''

import os
from pathlib import Path
import tarfile
from typing import Iterable, Iterator, List, Tuple


__all__ = [

    'parse_dimacs',
    'from_file',
    'read_tar'
]


def parse_dimacs(file: Iterable[str]) -> Tuple[str, int, int, List[List[int]]]:
    '''
    Parses iterable object of strings describing a DIMACS cnf file
    '''

    # Variables
    comments = []
    clauses = []
    n_aux = None
    m_aux = None

    # Check variables
    #n_count = 0
    m_count = 0

    # Processing the file
    for line in file:

        aux = line.strip()
        # print(line)
        # print(aux)

        if not aux:
            continue

        if aux.startswith('c'):
            comments.append(aux[1:])

        elif aux.startswith('p cnf'):
            _, _, n_aux, m_aux = aux.split()

            if not n_aux.isdigit() or not m_aux.isdigit():
                raise ValueError('Wrong value in header')

            n, m = int(n_aux), int(m_aux)

        elif aux.endswith('0'):
            literals = aux.split()
            clauses.append([int(literal) for literal in literals[:-1]])
            m_count += 1

        else:
            # print(line)
            raise ValueError('CNF file has wrong format')

    # Check parsing process
    if n_aux is None or m_aux is None:
        raise ValueError('No header in file')

    if m != m_count:
        raise ValueError('Header and number of clauses mismatch')

    description = '\n'.join(comments)

    return description, n, m, clauses


def from_file(in_file: Path) -> Tuple[str, int, int, List[List[int]]]:
    '''
    Parse dimacs file

    Parameters
    ----------
    in_file: Path object
            dimacs file

    Returns:
    ----------
    a tuple of str, int, int, List[List[int]]
    '''

    # Check file integrity
    if not os.path.isfile(in_file) and os.path.getsize(in_file):
        raise IOError('Not a file')

    # if not (in_file.endswith('.dimacs') or in_file.endswith('.cnf')):
    if not (in_file.suffix == '.dimacs' or in_file.suffix == '.cnf'):
        raise IOError('File must end in .dimacs or .cnf')

    # Parse files
    with open(in_file, 'r') as file:
        return parse_dimacs(file)


def get_header(in_file: Path) -> Tuple[str, int, int, List[List[int]]]:
    '''
    Parse dimacs file header

    Parameters
    ----------
    in_file: Path object
            dimacs file

    Returns:
    ----------
    a tuple of str, int, int, List[List[int]]
    '''

    # Check file integrity
    if not os.path.isfile(in_file) and os.path.getsize(in_file) > 0:
        raise IOError('Not a file')

    # if not (in_file.endswith('.dimacs') or in_file.endswith('.cnf')):
    if not (in_file.suffix == '.dimacs' or in_file.suffix == '.cnf'):
        raise IOError('File must end in .dimacs or .cnf')

    # Parse files
    with open(in_file, 'r') as file:

        # Variables
        comments = []
        n_aux = None
        m_aux = None
        n = None
        m = None

        # Processing the file
        for line in file:

            aux = line.strip()
            # print(line)
            # print(aux)

            if not aux:
                continue

            if aux.startswith('c'):
                comments.append(aux[1:])

            elif aux.startswith('p cnf'):
                _, _, n_aux, m_aux = aux.split()

                if not n_aux.isdigit() or not m_aux.isdigit():
                    raise ValueError('Wrong value in header')

                n, m = int(n_aux), int(m_aux)
                break

        if n is None or m is None:
            raise ValueError('No variable or clause number available')

        description = '\n'.join(comments)
        return description, n, m


def read_tar(file_path: Path) -> Iterator[str]:
    '''Reads a tar file containing dimacs files'''
    with tarfile.open(file_path, 'r') as tf:
        for file in tf:
            # Check file extension
            if not file.name.endswith('.cnf'):
                continue
            f = tf.extractfile(file)
            yield ''.join([line_byte.decode('utf-8') for line_byte in f])
