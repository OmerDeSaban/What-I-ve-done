from setuptools import Extension, setup

module = Extension('symnmfmod',
    sources=['symnmfmodule.c', 'symnmf.c'],
    include_dirs=['.'])

setup(name='symnmfmod',
    version='1.0',
    description='Python wrapper for symNMF algorithm',
    ext_modules=[module])