# SIA

A Python library providing fast computation of various statistical and graph
theoretic features for instances of the Satisfiability problem; and some other
common operations for manipulating such formulas.

## Installation

Use the following commands for entering the dev mode, in which you can build
and install the package, and be able to edit de package code in order to test
it 

```bash
make dev
```

And the following one for cleaning the build files, and uninstall it from your
environment:

```bash
make clean_build
```

Or you can just make a regular install:

```bash
make install

```

## Usage

You can start computing the modularity of one of the provided test formulas:

```python
>>> from pathlib import Path
>>> import sia
>>> file = Path('tests/data/graph.cnf')
>>> q = sia.feat.modularity(file)
>>> print(q)
0.3163265306122447
```