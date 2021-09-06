'''
Setup file
'''

import os
import pathlib
import platform

try:
    from setuptools import setup
    from setuptools import Extension

except ImportError:
    from distutils.core import setup
    from distutils.extension import Extension


# Load Description

directory = pathlib.Path(__file__).parent.resolve()
with open(directory / 'README.md', encoding='utf-8') as file:
    long_description = file.read()

# Check OS

if platform.system() == 'Linux':
    os.environ["CC"] = 'g++'


# C Extensions: featsat

featsat_sources = ['extensions/featsat.cpp']

featsat_ext = Extension(
    'featsat',
    sources=featsat_sources,
    include_dirs=['extensions'],
    extra_compile_args=['-Wall'])

# Setup Configuration

CONFIGURATION = {
    'name':              'python-sia',
    'packages':          ['sia'],
    'version':           '0.1.0',
    'description':       'SAT instance analysis tool',
    'long_description':  long_description,
    #'url':               'NOT YET',
    'author':            'Joseba Celaya',
    'author_email':      'josebacel@gmail.com',
    'license':           'LICENSE',
    'classifiers':       ["Programming Language :: Python :: 3"],
    'ext_modules':       [featsat_ext],
    'test_suite':        'tests',
}

setup(**CONFIGURATION)
