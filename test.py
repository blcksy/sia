'''
Handy test.py script for fast tests related to C Extensions testing.
'''
from pathlib import Path

import featsat
import sia


def header(text):
	'''Formats header titles for printing tests'''
	n = 80
	long_line = n*'-'
	short_line = 30*' '

	print(long_line)
	print(f'{short_line} {text}')
	print(long_line)


def test_featsat():

	file, max_clauses = 'HCP-529-420.cnf', 319490
	#file, max_clauses = 'UNSAT_P_sat_snake_p05.pddl_30.cnf', 239054681
	#file, max_clauses = 'graph.cnf', 7

	header('PYTHON TESTS')
		
	header('modularity_vig')
	featsat.modularity_vig(file, max_clauses)
	
	header('modularity_cvig')
	featsat.modularity_cvig(file, max_clauses)

	header('scale_free_var')
	featsat.scale_free_var(file, max_clauses)
	
	header('scale_free_clause')
	featsat.scale_free_clause(file, max_clauses)
	
	header('self_similar_vig')
	featsat.self_similar_vig(file, max_clauses)

	header('self_similar_cvig')
	featsat.self_similar_cvig(file, max_clauses)

	#header('compute_all')
	#featsat.compute_all(file, max_clauses)


if __name__ == '__main__':

	file = 'tests/data/HCP-529-420.cnf'
	file = 'tests/data/1_clause_0_var.cnf'
	file = 'tests/data/php_100_101.cnf'
	
	print('Launch modularity')
	ans = sia.feat.modularity(Path(file), mode='vig')
	print(ans)
	print('Launch self similarity')
	ans = sia.feat.self_similar(Path(file), mode='vig')
	print(ans)
	print('Launch scale free')
	ans = sia.feat.scale_free(Path(file), mode='var')
	print(ans)